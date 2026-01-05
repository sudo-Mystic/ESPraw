/**
 * RedditBase.h - Base class for all Reddit objects
 * 
 * Provides common functionality for all Reddit API objects
 */

#ifndef REDDIT_BASE_H
#define REDDIT_BASE_H

#include <Arduino.h>
#include <ArduinoJson.h>

// Forward declaration
class ESPraw;

/**
 * RedditBase - Base class for all Reddit objects
 */
class RedditBase {
public:
    /**
     * Constructor
     * @param espraw Pointer to ESPraw instance
     * @param data JSON data for this object
     */
    RedditBase(ESPraw* espraw, JsonObject data);
    
    /**
     * Virtual destructor
     */
    virtual ~RedditBase();
    
    /**
     * Get the object's unique ID
     * @return Object ID
     */
    String getId() const;
    
    /**
     * Get the object's fullname (kind + id)
     * @return Fullname (e.g., "t3_abc123")
     */
    String getFullname() const;
    
    /**
     * Get the object's kind/type
     * @return Kind string
     */
    String getKind() const;
    
    /**
     * Get object creation time (Unix timestamp)
     * @return Creation timestamp
     */
    unsigned long getCreated() const;
    
    /**
     * Get object creation time in UTC
     * @return UTC creation timestamp
     */
    unsigned long getCreatedUtc() const;
    
    /**
     * Parse and store data from JSON
     * @param data JSON object
     */
    virtual void parseData(JsonObject data);
    
    /**
     * Check if object is valid
     * @return true if valid
     */
    bool isValid() const;
    
protected:
    ESPraw* _espraw;
    String _id;
    String _kind;
    String _fullname;
    unsigned long _created;
    unsigned long _createdUtc;
    bool _valid;
    
    /**
     * Extract string field from JSON
     * @param data JSON object
     * @param key Field key
     * @param defaultValue Default value if not found
     * @return Field value
     */
    String extractString(JsonObject data, const char* key, const String& defaultValue = "") const;
    
    /**
     * Extract integer field from JSON
     * @param data JSON object
     * @param key Field key
     * @param defaultValue Default value if not found
     * @return Field value
     */
    int extractInt(JsonObject data, const char* key, int defaultValue = 0) const;
    
    /**
     * Extract boolean field from JSON
     * @param data JSON object
     * @param key Field key
     * @param defaultValue Default value if not found
     * @return Field value
     */
    bool extractBool(JsonObject data, const char* key, bool defaultValue = false) const;
    
    /**
     * Extract unsigned long field from JSON
     * @param data JSON object
     * @param key Field key
     * @param defaultValue Default value if not found
     * @return Field value
     */
    unsigned long extractULong(JsonObject data, const char* key, unsigned long defaultValue = 0) const;
};

#endif // REDDIT_BASE_H
