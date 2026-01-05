/**
 * ESPraw.h - Main Reddit API wrapper class
 * 
 * This is the main entry point for the ESPraw library.
 * Provides convenient access to Reddit's API from ESP32.
 */

#ifndef ESPRAW_H
#define ESPRAW_H

#include <Arduino.h>
#include <WiFi.h>
#include "ESPrawConfig.h"
#include "ESPrawClient.h"
#include "ESPrawAuth.h"

// Forward declarations
class Subreddit;
class Submission;
class Comment;
class Redditor;

/**
 * ESPraw - Main Reddit API wrapper class
 * 
 * This class is the gateway to interacting with Reddit's API.
 * 
 * Example usage:
 * ```cpp
 * ESPraw reddit;
 * ESPrawAuthConfig config;
 * config.clientId = "your_client_id";
 * config.clientSecret = "your_client_secret";
 * config.username = "your_username";
 * config.password = "your_password";
 * config.userAgent = "ESPraw/0.1.0 (ESP32)";
 * 
 * reddit.begin(config);
 * ```
 */
class ESPraw {
public:
    /**
     * Constructor
     */
    ESPraw();
    
    /**
     * Destructor
     */
    ~ESPraw();
    
    /**
     * Initialize ESPraw with authentication configuration
     * @param authConfig Authentication configuration
     * @return true if successful
     */
    bool begin(const ESPrawAuthConfig& authConfig);
    
    /**
     * Initialize ESPraw with custom request configuration
     * @param authConfig Authentication configuration
     * @param requestConfig Request configuration
     * @return true if successful
     */
    bool begin(const ESPrawAuthConfig& authConfig, const ESPrawRequestConfig& requestConfig);
    
    /**
     * Authenticate with Reddit
     * @return true if successful
     */
    bool authenticate();
    
    /**
     * Check if authenticated
     * @return true if authenticated
     */
    bool isAuthenticated() const;
    
    /**
     * Get a subreddit object
     * @param name Subreddit name (without r/)
     * @return Subreddit pointer (caller must delete)
     */
    Subreddit* subreddit(const String& name);
    
    /**
     * Get a submission object by ID
     * @param id Submission ID (without prefix)
     * @return Submission pointer (caller must delete)
     */
    Submission* submission(const String& id);
    
    /**
     * Get a submission object by URL
     * @param url Full Reddit URL
     * @return Submission pointer (caller must delete)
     */
    Submission* submissionByUrl(const String& url);
    
    /**
     * Get a comment object by ID
     * @param id Comment ID (without prefix)
     * @return Comment pointer (caller must delete)
     */
    Comment* comment(const String& id);
    
    /**
     * Get a redditor object
     * @param username Username (without u/)
     * @return Redditor pointer (caller must delete)
     */
    Redditor* redditor(const String& username);
    
    /**
     * Get the currently authenticated user
     * @return Redditor pointer for current user (caller must delete)
     */
    Redditor* me();
    
    /**
     * Set read-only mode
     * @param readOnly true for read-only mode
     * @return true if successful
     */
    bool setReadOnly(bool readOnly);
    
    /**
     * Check if in read-only mode
     * @return true if read-only
     */
    bool isReadOnly() const;
    
    /**
     * Get the HTTP client
     * @return Reference to client
     */
    ESPrawClient& getClient();
    
    /**
     * Get the auth handler
     * @return Reference to auth handler
     */
    ESPrawAuth& getAuth();
    
    /**
     * Perform a GET request to Reddit API
     * @param endpoint API endpoint
     * @param params Query parameters
     * @return Response object
     */
    ESPrawResponse get(const String& endpoint, const String& params = "");
    
    /**
     * Perform a POST request to Reddit API
     * @param endpoint API endpoint
     * @param body Request body
     * @return Response object
     */
    ESPrawResponse post(const String& endpoint, const String& body);
    
    /**
     * Check WiFi connection and reconnect if needed
     * @return true if connected
     */
    bool checkWiFi();
    
private:
    ESPrawAuthConfig _authConfig;
    ESPrawRequestConfig _requestConfig;
    ESPrawClient _client;
    ESPrawAuth _auth;
    bool _initialized;
    bool _readOnly;
    
    /**
     * Extract submission ID from Reddit URL
     * @param url Reddit URL
     * @return Submission ID or empty string
     */
    String extractSubmissionId(const String& url);
};

#endif // ESPRAW_H
