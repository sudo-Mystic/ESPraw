/**
 * ESPrawAuth.h - OAuth2 authentication handler for ESPraw
 * 
 * Handles OAuth2 authentication flow with Reddit API
 */

#ifndef ESPRAW_AUTH_H
#define ESPRAW_AUTH_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "ESPrawConfig.h"

/**
 * OAuth2 token information
 */
struct ESPrawToken {
    String accessToken;
    String tokenType;
    String scope;
    unsigned long expiresAt;  // Unix timestamp
    bool isValid;
    
    ESPrawToken() : expiresAt(0), isValid(false) {}
    
    /**
     * Check if token is expired
     * @return true if expired
     */
    bool isExpired() const {
        return millis() / 1000 > expiresAt;
    }
    
    /**
     * Get remaining validity in seconds
     * @return seconds until expiration
     */
    unsigned long remainingValidity() const {
        unsigned long now = millis() / 1000;
        if (now >= expiresAt) {
            return 0;
        }
        return expiresAt - now;
    }
};

/**
 * ESPrawAuth - OAuth2 authentication handler
 */
class ESPrawAuth {
public:
    /**
     * Constructor
     */
    ESPrawAuth();
    
    /**
     * Destructor
     */
    ~ESPrawAuth();
    
    /**
     * Initialize authentication with configuration
     * @param config Authentication configuration
     * @return true if successful
     */
    bool begin(const ESPrawAuthConfig& config);
    
    /**
     * Authenticate using script (username/password) method
     * @return true if successful
     */
    bool authenticateScript();
    
    /**
     * Authenticate for read-only access (client credentials)
     * @return true if successful
     */
    bool authenticateReadOnly();
    
    /**
     * Get current access token
     * @return Token object
     */
    ESPrawToken getToken() const;
    
    /**
     * Check if currently authenticated
     * @return true if authenticated with valid token
     */
    bool isAuthenticated() const;
    
    /**
     * Refresh the access token if needed
     * @return true if successful
     */
    bool refreshToken();
    
    /**
     * Revoke current token
     * @return true if successful
     */
    bool revokeToken();
    
private:
    /**
     * Request access token from Reddit
     * @param grantType OAuth2 grant type
     * @param params Additional parameters
     * @return Token object
     */
    ESPrawToken requestToken(const String& grantType, const String& params);
    
    /**
     * Parse token response from Reddit
     * @param json JSON document
     * @return Token object
     */
    ESPrawToken parseTokenResponse(JsonDocument& json);
    
    /**
     * Create basic auth header for client credentials
     * @return Base64 encoded credentials
     */
    String createBasicAuth() const;
    
    /**
     * URL encode a string
     * @param str String to encode
     * @return Encoded string
     */
    String urlEncode(const String& str) const;
    
    ESPrawAuthConfig _config;
    ESPrawToken _token;
    WiFiClientSecure _secureClient;
};

#endif // ESPRAW_AUTH_H
