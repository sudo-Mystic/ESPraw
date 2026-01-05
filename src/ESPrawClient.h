/**
 * ESPrawClient.h - HTTP client wrapper for ESPraw
 * 
 * This class handles all HTTP communication with Reddit's API,
 * including authentication, rate limiting, and error handling.
 */

#ifndef ESPRAW_CLIENT_H
#define ESPRAW_CLIENT_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "ESPrawConfig.h"

/**
 * HTTP request methods
 */
enum class ESPrawRequestMethod {
    GET,
    POST,
    PUT,
    PATCH,
    DELETE_METHOD  // Avoid conflict with DELETE macro
};

/**
 * HTTP response structure
 */
struct ESPrawResponse {
    int statusCode;
    String body;
    String error;
    bool success;
    
    ESPrawResponse() : statusCode(0), success(false) {}
};

/**
 * ESPrawClient - HTTP client for Reddit API
 */
class ESPrawClient {
public:
    /**
     * Constructor
     */
    ESPrawClient();
    
    /**
     * Destructor
     */
    ~ESPrawClient();
    
    /**
     * Initialize the client with configuration
     * @param config Request configuration
     * @return true if successful
     */
    bool begin(const ESPrawRequestConfig& config);
    
    /**
     * Set the OAuth access token
     * @param token Access token
     */
    void setAccessToken(const String& token);
    
    /**
     * Get current access token
     * @return Access token
     */
    String getAccessToken() const;
    
    /**
     * Set user agent
     * @param userAgent User agent string
     */
    void setUserAgent(const String& userAgent);
    
    /**
     * Perform HTTP GET request
     * @param endpoint API endpoint (without base URL)
     * @param params Query parameters
     * @return Response object
     */
    ESPrawResponse get(const String& endpoint, const String& params = "");
    
    /**
     * Perform HTTP POST request
     * @param endpoint API endpoint (without base URL)
     * @param body Request body
     * @param contentType Content type (default: application/x-www-form-urlencoded)
     * @return Response object
     */
    ESPrawResponse post(const String& endpoint, const String& body, 
                       const String& contentType = "application/x-www-form-urlencoded");
    
    /**
     * Perform HTTP PUT request
     * @param endpoint API endpoint (without base URL)
     * @param body Request body
     * @return Response object
     */
    ESPrawResponse put(const String& endpoint, const String& body);
    
    /**
     * Perform HTTP DELETE request
     * @param endpoint API endpoint (without base URL)
     * @return Response object
     */
    ESPrawResponse delete_(const String& endpoint);
    
    /**
     * Check if rate limit allows a request
     * @return true if request is allowed
     */
    bool checkRateLimit();
    
    /**
     * Get time until next request is allowed (in milliseconds)
     * @return milliseconds to wait
     */
    unsigned long timeUntilNextRequest();
    
private:
    /**
     * Perform HTTP request with retry logic
     * @param method HTTP method
     * @param url Full URL
     * @param body Request body (optional)
     * @param contentType Content type (optional)
     * @return Response object
     */
    ESPrawResponse performRequest(ESPrawRequestMethod method, const String& url, 
                                  const String& body = "", 
                                  const String& contentType = "");
    
    /**
     * Build full URL from endpoint
     * @param endpoint API endpoint
     * @param params Query parameters
     * @return Full URL
     */
    String buildUrl(const String& endpoint, const String& params = "");
    
    /**
     * Add common headers to request
     * @param http HTTPClient object
     */
    void addHeaders(HTTPClient& http);
    
    /**
     * Record request for rate limiting
     */
    void recordRequest();
    
    /**
     * Clean up old request timestamps
     */
    void cleanupRequestLog();
    
    WiFiClientSecure _secureClient;
    String _accessToken;
    String _userAgent;
    ESPrawRequestConfig _config;
    
    // Rate limiting
    unsigned long _requestTimes[ESPRAW_RATE_LIMIT_REQUESTS];
    int _requestCount;
    unsigned long _lastCleanup;
};

#endif // ESPRAW_CLIENT_H
