/**
 * Submission.h - Reddit submission (post) model
 * 
 * Represents a Reddit submission/post
 */

#ifndef SUBMISSION_H
#define SUBMISSION_H

#include "RedditBase.h"
#include <Arduino.h>
#include <ArduinoJson.h>

/**
 * Submission - Represents a Reddit post
 */
class Submission : public RedditBase {
public:
    /**
     * Constructor
     * @param espraw Pointer to ESPraw instance
     * @param data JSON data for submission
     */
    Submission(ESPraw* espraw, JsonObject data);
    
    /**
     * Destructor
     */
    ~Submission();
    
    /**
     * Parse submission data from JSON
     * @param data JSON object
     */
    void parseData(JsonObject data) override;
    
    // Getters
    String getTitle() const { return _title; }
    String getAuthor() const { return _author; }
    String getSubreddit() const { return _subreddit; }
    String getSelftext() const { return _selftext; }
    String getUrl() const { return _url; }
    String getDomain() const { return _domain; }
    String getPermalink() const { return _permalink; }
    int getScore() const { return _score; }
    int getUpvoteRatio() const { return _upvoteRatio; }
    int getNumComments() const { return _numComments; }
    bool isOver18() const { return _over18; }
    bool isSpoiler() const { return _spoiler; }
    bool isLocked() const { return _locked; }
    bool isStickied() const { return _stickied; }
    bool isSelf() const { return _isSelf; }
    
    /**
     * Upvote this submission
     * @return true if successful
     */
    bool upvote();
    
    /**
     * Downvote this submission
     * @return true if successful
     */
    bool downvote();
    
    /**
     * Remove vote from this submission
     * @return true if successful
     */
    bool clearVote();
    
    /**
     * Save this submission
     * @return true if successful
     */
    bool save();
    
    /**
     * Unsave this submission
     * @return true if successful
     */
    bool unsave();
    
    /**
     * Reply to this submission
     * @param text Reply text
     * @return true if successful
     */
    bool reply(const String& text);
    
    /**
     * Edit submission text (self posts only)
     * @param text New text
     * @return true if successful
     */
    bool edit(const String& text);
    
    /**
     * Delete this submission
     * @return true if successful
     */
    bool deleteSubmission();
    
    /**
     * Get comments for this submission
     * @param limit Maximum number of comments
     * @return JSON document with comments (caller must handle)
     */
    bool getComments(DynamicJsonDocument& doc, int limit = 10);

private:
    String _title;
    String _author;
    String _subreddit;
    String _selftext;
    String _url;
    String _domain;
    String _permalink;
    int _score;
    int _upvoteRatio;
    int _numComments;
    bool _over18;
    bool _spoiler;
    bool _locked;
    bool _stickied;
    bool _isSelf;
    
    /**
     * Perform vote action
     * @param dir Vote direction (-1, 0, 1)
     * @return true if successful
     */
    bool vote(int dir);
};

#endif // SUBMISSION_H
