/**
 * ESPrawConfig.h - Configuration structures for ESPraw
 * 
 * This file contains configuration structures and constants for the ESPraw library.
 */

#ifndef ESPRAW_CONFIG_H
#define ESPRAW_CONFIG_H

#include <Arduino.h>

// API Configuration
#define ESPRAW_VERSION "0.1.0"
#define ESPRAW_USER_AGENT_FORMAT "ESPraw/" ESPRAW_VERSION " (ESP32)"
#define ESPRAW_API_BASE_URL "https://oauth.reddit.com"
#define ESPRAW_AUTH_URL "https://www.reddit.com/api/v1/access_token"

// Rate Limiting
#define ESPRAW_RATE_LIMIT_REQUESTS 60  // requests per minute
#define ESPRAW_RATE_LIMIT_WINDOW 60000 // 60 seconds in milliseconds

// Memory Configuration
#define ESPRAW_MAX_RESPONSE_SIZE 16384  // 16KB max response
#define ESPRAW_JSON_BUFFER_SIZE 8192     // 8KB JSON buffer
#define ESPRAW_MAX_RETRIES 3
#define ESPRAW_RETRY_DELAY 1000         // 1 second

// Timeouts
#define ESPRAW_CONNECT_TIMEOUT 10000    // 10 seconds
#define ESPRAW_REQUEST_TIMEOUT 30000    // 30 seconds

/**
 * Authentication configuration structure
 */
struct ESPrawAuthConfig {
    String clientId;
    String clientSecret;
    String username;
    String password;
    String userAgent;
    bool readOnlyMode;
    
    ESPrawAuthConfig() : readOnlyMode(false) {}
};

/**
 * Request configuration structure
 */
struct ESPrawRequestConfig {
    int maxRetries;
    int retryDelay;
    int connectTimeout;
    int requestTimeout;
    
    ESPrawRequestConfig() 
        : maxRetries(ESPRAW_MAX_RETRIES)
        , retryDelay(ESPRAW_RETRY_DELAY)
        , connectTimeout(ESPRAW_CONNECT_TIMEOUT)
        , requestTimeout(ESPRAW_REQUEST_TIMEOUT) {}
};

#endif // ESPRAW_CONFIG_H
