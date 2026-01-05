/**
 * ESPrawClient.cpp - HTTP client implementation for ESPraw
 */

#include "ESPrawClient.h"

ESPrawClient::ESPrawClient() 
    : _requestCount(0), _lastCleanup(0) {
    memset(_requestTimes, 0, sizeof(_requestTimes));
}

ESPrawClient::~ESPrawClient() {
    // Cleanup
}

bool ESPrawClient::begin(const ESPrawRequestConfig& config) {
    _config = config;
    // SECURITY WARNING: Certificate validation is currently disabled
    // This is a known security issue and should be addressed before production use
    // TODO: Implement proper certificate validation
    // For production, consider:
    // 1. Loading Reddit's root CA certificate
    // 2. Using certificate fingerprint validation
    // 3. Implementing certificate bundle validation
    _secureClient.setInsecure();
    return true;
}

void ESPrawClient::setAccessToken(const String& token) {
    _accessToken = token;
}

String ESPrawClient::getAccessToken() const {
    return _accessToken;
}

void ESPrawClient::setUserAgent(const String& userAgent) {
    _userAgent = userAgent;
}

ESPrawResponse ESPrawClient::get(const String& endpoint, const String& params) {
    String url = buildUrl(endpoint, params);
    return performRequest(ESPrawRequestMethod::GET, url);
}

ESPrawResponse ESPrawClient::post(const String& endpoint, const String& body, 
                                 const String& contentType) {
    String url = buildUrl(endpoint);
    return performRequest(ESPrawRequestMethod::POST, url, body, contentType);
}

ESPrawResponse ESPrawClient::put(const String& endpoint, const String& body) {
    String url = buildUrl(endpoint);
    return performRequest(ESPrawRequestMethod::PUT, url, body, 
                         "application/x-www-form-urlencoded");
}

ESPrawResponse ESPrawClient::delete_(const String& endpoint) {
    String url = buildUrl(endpoint);
    return performRequest(ESPrawRequestMethod::DELETE_METHOD, url);
}

bool ESPrawClient::checkRateLimit() {
    cleanupRequestLog();
    return _requestCount < ESPRAW_RATE_LIMIT_REQUESTS;
}

unsigned long ESPrawClient::timeUntilNextRequest() {
    if (checkRateLimit()) {
        return 0;
    }
    
    cleanupRequestLog();
    
    if (_requestCount == 0) {
        return 0;
    }
    
    unsigned long oldestRequest = _requestTimes[0];
    unsigned long timeElapsed = millis() - oldestRequest;
    
    if (timeElapsed >= ESPRAW_RATE_LIMIT_WINDOW) {
        return 0;
    }
    
    return ESPRAW_RATE_LIMIT_WINDOW - timeElapsed;
}

ESPrawResponse ESPrawClient::performRequest(ESPrawRequestMethod method, 
                                           const String& url,
                                           const String& body,
                                           const String& contentType) {
    ESPrawResponse response;
    
    // Check rate limit
    if (!checkRateLimit()) {
        unsigned long waitTime = timeUntilNextRequest();
        if (waitTime > 0) {
            Serial.printf("Rate limit reached, waiting %lu ms\n", waitTime);
            delay(waitTime);
        }
    }
    
    // Retry loop
    for (int attempt = 0; attempt <= _config.maxRetries; attempt++) {
        if (attempt > 0) {
            Serial.printf("Retry attempt %d/%d\n", attempt, _config.maxRetries);
            // Exponential backoff: delay increases exponentially with each retry
            unsigned long backoffDelay = _config.retryDelay * (1 << attempt); // 2^attempt
            if (backoffDelay > 30000) backoffDelay = 30000; // Cap at 30 seconds
            delay(backoffDelay);
        }
        
        HTTPClient http;
        http.setTimeout(_config.requestTimeout);
        
        if (!http.begin(_secureClient, url)) {
            response.error = "Failed to begin HTTP connection";
            continue;
        }
        
        addHeaders(http);
        
        // Set content type for POST/PUT
        if ((method == ESPrawRequestMethod::POST || method == ESPrawRequestMethod::PUT) 
            && contentType.length() > 0) {
            http.addHeader("Content-Type", contentType);
        }
        
        // Perform request
        int httpCode = -1;
        switch (method) {
            case ESPrawRequestMethod::GET:
                httpCode = http.GET();
                break;
            case ESPrawRequestMethod::POST:
                httpCode = http.POST(body);
                break;
            case ESPrawRequestMethod::PUT:
                httpCode = http.PUT(body);
                break;
            case ESPrawRequestMethod::DELETE_METHOD:
                httpCode = http.sendRequest("DELETE");
                break;
        }
        
        response.statusCode = httpCode;
        
        if (httpCode > 0) {
            response.body = http.getString();
            
            if (httpCode >= 200 && httpCode < 300) {
                response.success = true;
                recordRequest();
                http.end();
                return response;
            } else if (httpCode == 401) {
                response.error = "Unauthorized - token may be expired";
                http.end();
                return response; // Don't retry auth errors
            } else if (httpCode == 429) {
                response.error = "Rate limit exceeded";
                // Extract retry-after if available
                String retryAfter = http.header("Retry-After");
                if (retryAfter.length() > 0) {
                    delay(retryAfter.toInt() * 1000);
                }
            } else {
                response.error = "HTTP error: " + String(httpCode);
            }
        } else {
            response.error = "Connection error: " + http.errorToString(httpCode);
        }
        
        http.end();
    }
    
    return response;
}

String ESPrawClient::buildUrl(const String& endpoint, const String& params) {
    String url = String(ESPRAW_API_BASE_URL) + endpoint;
    
    if (params.length() > 0) {
        url += "?" + params;
    }
    
    return url;
}

void ESPrawClient::addHeaders(HTTPClient& http) {
    // User agent
    if (_userAgent.length() > 0) {
        http.addHeader("User-Agent", _userAgent);
    } else {
        http.addHeader("User-Agent", ESPRAW_USER_AGENT_FORMAT);
    }
    
    // Authorization
    if (_accessToken.length() > 0) {
        http.addHeader("Authorization", "Bearer " + _accessToken);
    }
    
    // Accept JSON
    http.addHeader("Accept", "application/json");
}

void ESPrawClient::recordRequest() {
    cleanupRequestLog();
    
    if (_requestCount < ESPRAW_RATE_LIMIT_REQUESTS) {
        _requestTimes[_requestCount] = millis();
        _requestCount++;
    }
}

void ESPrawClient::cleanupRequestLog() {
    unsigned long now = millis();
    
    // Only cleanup every second
    if (now - _lastCleanup < 1000) {
        return;
    }
    
    _lastCleanup = now;
    
    // Remove requests older than the window
    int validRequests = 0;
    for (int i = 0; i < _requestCount; i++) {
        if (now - _requestTimes[i] < ESPRAW_RATE_LIMIT_WINDOW) {
            _requestTimes[validRequests] = _requestTimes[i];
            validRequests++;
        }
    }
    
    _requestCount = validRequests;
}
