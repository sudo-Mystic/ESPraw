/**
 * ESPraw.cpp - Main ESPraw implementation
 */

#include "ESPraw.h"
#include "models/Subreddit.h"
#include "models/Submission.h"
#include "models/Comment.h"
#include "models/Redditor.h"

ESPraw::ESPraw() : _initialized(false), _readOnly(false) {
}

ESPraw::~ESPraw() {
}

bool ESPraw::begin(const ESPrawAuthConfig& authConfig) {
    ESPrawRequestConfig requestConfig;
    return begin(authConfig, requestConfig);
}

bool ESPraw::begin(const ESPrawAuthConfig& authConfig, const ESPrawRequestConfig& requestConfig) {
    _authConfig = authConfig;
    _requestConfig = requestConfig;
    _readOnly = authConfig.readOnlyMode;
    
    // Initialize HTTP client
    if (!_client.begin(requestConfig)) {
        Serial.println("Failed to initialize HTTP client");
        return false;
    }
    
    // Initialize authentication
    if (!_auth.begin(authConfig)) {
        Serial.println("Failed to initialize authentication");
        return false;
    }
    
    // Set user agent
    if (!authConfig.userAgent.isEmpty()) {
        _client.setUserAgent(authConfig.userAgent);
    }
    
    _initialized = true;
    
    // Authenticate immediately
    return authenticate();
}

bool ESPraw::authenticate() {
    if (!_initialized) {
        Serial.println("ESPraw not initialized");
        return false;
    }
    
    bool success = false;
    
    if (_readOnly || _authConfig.readOnlyMode) {
        success = _auth.authenticateReadOnly();
    } else {
        success = _auth.authenticateScript();
    }
    
    if (success) {
        ESPrawToken token = _auth.getToken();
        _client.setAccessToken(token.accessToken);
        Serial.println("Authentication successful");
    } else {
        Serial.println("Authentication failed");
    }
    
    return success;
}

bool ESPraw::isAuthenticated() const {
    return _auth.isAuthenticated();
}

Subreddit* ESPraw::subreddit(const String& name) {
    return new Subreddit(this, name);
}

Submission* ESPraw::submission(const String& id) {
    // Fetch submission from Reddit
    String endpoint = "/comments/" + id;
    ESPrawResponse response = get(endpoint);
    
    if (!response.success) {
        return nullptr;
    }
    
    DynamicJsonDocument doc(8192);
    DeserializationError error = deserializeJson(doc, response.body);
    
    if (error) {
        return nullptr;
    }
    
    // Extract submission data from response
    if (doc.is<JsonArray>() && doc.size() > 0) {
        JsonObject listing = doc[0]["data"];
        if (listing.containsKey("children") && listing["children"].size() > 0) {
            JsonObject submissionData = listing["children"][0]["data"];
            return new Submission(this, submissionData);
        }
    }
    
    return nullptr;
}

Submission* ESPraw::submissionByUrl(const String& url) {
    String id = extractSubmissionId(url);
    if (id.isEmpty()) {
        return nullptr;
    }
    return submission(id);
}

Comment* ESPraw::comment(const String& id) {
    // For now, return a basic comment object
    // Full implementation would fetch from Reddit
    DynamicJsonDocument doc(1024);
    JsonObject obj = doc.to<JsonObject>();
    obj["id"] = id;
    return new Comment(this, obj);
}

Redditor* ESPraw::redditor(const String& username) {
    return new Redditor(this, username);
}

Redditor* ESPraw::me() {
    if (!isAuthenticated() || _readOnly) {
        return nullptr;
    }
    
    ESPrawResponse response = get("/api/v1/me");
    
    if (!response.success) {
        return nullptr;
    }
    
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, response.body);
    
    if (error) {
        return nullptr;
    }
    
    JsonObject data = doc.as<JsonObject>();
    return new Redditor(this, data);
}

bool ESPraw::setReadOnly(bool readOnly) {
    if (_readOnly == readOnly) {
        return true;
    }
    
    _readOnly = readOnly;
    _authConfig.readOnlyMode = readOnly;
    
    // Re-authenticate with new mode
    return authenticate();
}

bool ESPraw::isReadOnly() const {
    return _readOnly;
}

ESPrawClient& ESPraw::getClient() {
    return _client;
}

ESPrawAuth& ESPraw::getAuth() {
    return _auth;
}

ESPrawResponse ESPraw::get(const String& endpoint, const String& params) {
    // Check if token is expired and refresh if needed
    if (_auth.isAuthenticated() && _auth.getToken().isExpired()) {
        Serial.println("Token expired, refreshing...");
        if (!_auth.refreshToken()) {
            ESPrawResponse response;
            response.error = "Failed to refresh token";
            return response;
        }
        _client.setAccessToken(_auth.getToken().accessToken);
    }
    
    return _client.get(endpoint, params);
}

ESPrawResponse ESPraw::post(const String& endpoint, const String& body) {
    // Check if token is expired and refresh if needed
    if (_auth.isAuthenticated() && _auth.getToken().isExpired()) {
        Serial.println("Token expired, refreshing...");
        if (!_auth.refreshToken()) {
            ESPrawResponse response;
            response.error = "Failed to refresh token";
            return response;
        }
        _client.setAccessToken(_auth.getToken().accessToken);
    }
    
    return _client.post(endpoint, body);
}

bool ESPraw::checkWiFi() {
    if (WiFi.status() == WL_CONNECTED) {
        return true;
    }
    
    Serial.println("WiFi not connected, attempting to reconnect...");
    
    // Wait for connection (up to 10 seconds)
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi reconnected");
        return true;
    }
    
    Serial.println("\nWiFi reconnection failed");
    return false;
}

String ESPraw::extractSubmissionId(const String& url) {
    // Extract submission ID from Reddit URL
    // Example: https://www.reddit.com/r/test/comments/abc123/title/
    
    int commentsPos = url.indexOf("/comments/");
    if (commentsPos == -1) {
        return "";
    }
    
    int idStart = commentsPos + 10; // Length of "/comments/"
    int idEnd = url.indexOf("/", idStart);
    
    if (idEnd == -1) {
        idEnd = url.length();
    }
    
    return url.substring(idStart, idEnd);
}
