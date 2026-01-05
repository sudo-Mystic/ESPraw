/**
 * Redditor.h - Reddit user (redditor) model
 * 
 * Represents a Reddit user
 */

#ifndef REDDITOR_H
#define REDDITOR_H

#include "RedditBase.h"
#include <Arduino.h>
#include <ArduinoJson.h>

/**
 * Redditor - Represents a Reddit user
 */
class Redditor : public RedditBase {
public:
    /**
     * Constructor
     * @param espraw Pointer to ESPraw instance
     * @param username Username
     */
    Redditor(ESPraw* espraw, const String& username);
    
    /**
     * Constructor from JSON data
     * @param espraw Pointer to ESPraw instance
     * @param data JSON data for redditor
     */
    Redditor(ESPraw* espraw, JsonObject data);
    
    /**
     * Destructor
     */
    ~Redditor();
    
    /**
     * Parse redditor data from JSON
     * @param data JSON object
     */
    void parseData(JsonObject data) override;
    
    // Getters
    String getUsername() const { return _username; }
    int getLinkKarma() const { return _linkKarma; }
    int getCommentKarma() const { return _commentKarma; }
    bool hasVerifiedEmail() const { return _hasVerifiedEmail; }
    bool isGold() const { return _isGold; }
    bool isMod() const { return _isMod; }
    bool isEmployee() const { return _isEmployee; }
    
    /**
     * Fetch full user info from Reddit
     * @return true if successful
     */
    bool fetch();
    
    /**
     * Get user's submitted posts
     * @param doc JSON document to store results
     * @param limit Maximum number of posts
     * @return true if successful
     */
    bool getSubmissions(DynamicJsonDocument& doc, int limit = 25);
    
    /**
     * Get user's comments
     * @param doc JSON document to store results
     * @param limit Maximum number of comments
     * @return true if successful
     */
    bool getComments(DynamicJsonDocument& doc, int limit = 25);

private:
    String _username;
    int _linkKarma;
    int _commentKarma;
    bool _hasVerifiedEmail;
    bool _isGold;
    bool _isMod;
    bool _isEmployee;
    
    /**
     * Fetch user content
     * @param doc JSON document to store results
     * @param type Content type ("submitted" or "comments")
     * @param limit Maximum number of items
     * @return true if successful
     */
    bool fetchUserContent(DynamicJsonDocument& doc, const String& type, int limit);
};

#endif // REDDITOR_H
