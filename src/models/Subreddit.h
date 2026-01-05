/**
 * Subreddit.h - Reddit subreddit model
 * 
 * Represents a Reddit subreddit
 */

#ifndef SUBREDDIT_H
#define SUBREDDIT_H

#include "RedditBase.h"
#include <Arduino.h>
#include <ArduinoJson.h>

/**
 * Subreddit - Represents a Reddit subreddit
 */
class Subreddit : public RedditBase {
public:
    /**
     * Constructor
     * @param espraw Pointer to ESPraw instance
     * @param name Subreddit name
     */
    Subreddit(ESPraw* espraw, const String& name);
    
    /**
     * Constructor from JSON data
     * @param espraw Pointer to ESPraw instance
     * @param data JSON data for subreddit
     */
    Subreddit(ESPraw* espraw, JsonObject data);
    
    /**
     * Destructor
     */
    ~Subreddit();
    
    /**
     * Parse subreddit data from JSON
     * @param data JSON object
     */
    void parseData(JsonObject data) override;
    
    // Getters
    String getName() const { return _displayName; }
    String getDisplayName() const { return _displayName; }
    String getTitle() const { return _title; }
    String getDescription() const { return _description; }
    String getPublicDescription() const { return _publicDescription; }
    int getSubscribers() const { return _subscribers; }
    int getActiveUsers() const { return _activeUsers; }
    bool isOver18() const { return _over18; }
    bool isSubscriber() const { return _userIsSubscriber; }
    
    /**
     * Fetch hot posts from subreddit
     * @param doc JSON document to store results
     * @param limit Maximum number of posts
     * @return true if successful
     */
    bool hot(DynamicJsonDocument& doc, int limit = 25);
    
    /**
     * Fetch new posts from subreddit
     * @param doc JSON document to store results
     * @param limit Maximum number of posts
     * @return true if successful
     */
    bool new_(DynamicJsonDocument& doc, int limit = 25);
    
    /**
     * Fetch top posts from subreddit
     * @param doc JSON document to store results
     * @param timeFilter Time filter (hour, day, week, month, year, all)
     * @param limit Maximum number of posts
     * @return true if successful
     */
    bool top(DynamicJsonDocument& doc, const String& timeFilter = "day", int limit = 25);
    
    /**
     * Fetch rising posts from subreddit
     * @param doc JSON document to store results
     * @param limit Maximum number of posts
     * @return true if successful
     */
    bool rising(DynamicJsonDocument& doc, int limit = 25);
    
    /**
     * Fetch controversial posts from subreddit
     * @param doc JSON document to store results
     * @param timeFilter Time filter (hour, day, week, month, year, all)
     * @param limit Maximum number of posts
     * @return true if successful
     */
    bool controversial(DynamicJsonDocument& doc, const String& timeFilter = "day", int limit = 25);
    
    /**
     * Submit a text post to this subreddit
     * @param title Post title
     * @param text Post text
     * @return true if successful
     */
    bool submitText(const String& title, const String& text);
    
    /**
     * Submit a link post to this subreddit
     * @param title Post title
     * @param url Link URL
     * @return true if successful
     */
    bool submitLink(const String& title, const String& url);
    
    /**
     * Subscribe to this subreddit
     * @return true if successful
     */
    bool subscribe();
    
    /**
     * Unsubscribe from this subreddit
     * @return true if successful
     */
    bool unsubscribe();
    
    /**
     * Fetch full subreddit info from Reddit
     * @return true if successful
     */
    bool fetch();

private:
    String _displayName;
    String _title;
    String _description;
    String _publicDescription;
    int _subscribers;
    int _activeUsers;
    bool _over18;
    bool _userIsSubscriber;
    
    /**
     * Fetch posts with a specific sorting
     * @param doc JSON document to store results
     * @param sort Sort type (hot, new, top, etc.)
     * @param params Additional parameters
     * @return true if successful
     */
    bool fetchPosts(DynamicJsonDocument& doc, const String& sort, const String& params = "");
    
    /**
     * Perform subscription action
     * @param action "sub" or "unsub"
     * @return true if successful
     */
    bool performSubscriptionAction(const String& action);
};

#endif // SUBREDDIT_H
