/**
 * Comment.h - Reddit comment model
 * 
 * Represents a Reddit comment
 */

#ifndef COMMENT_H
#define COMMENT_H

#include "RedditBase.h"
#include <Arduino.h>
#include <ArduinoJson.h>

/**
 * Comment - Represents a Reddit comment
 */
class Comment : public RedditBase {
public:
    /**
     * Constructor
     * @param espraw Pointer to ESPraw instance
     * @param data JSON data for comment
     */
    Comment(ESPraw* espraw, JsonObject data);
    
    /**
     * Destructor
     */
    ~Comment();
    
    /**
     * Parse comment data from JSON
     * @param data JSON object
     */
    void parseData(JsonObject data) override;
    
    // Getters
    String getBody() const { return _body; }
    String getAuthor() const { return _author; }
    String getSubreddit() const { return _subreddit; }
    String getParentId() const { return _parentId; }
    String getLinkId() const { return _linkId; }
    String getPermalink() const { return _permalink; }
    int getScore() const { return _score; }
    int getDepth() const { return _depth; }
    bool isSubmitter() const { return _isSubmitter; }
    bool isScoreHidden() const { return _scoreHidden; }
    
    /**
     * Upvote this comment
     * @return true if successful
     */
    bool upvote();
    
    /**
     * Downvote this comment
     * @return true if successful
     */
    bool downvote();
    
    /**
     * Remove vote from this comment
     * @return true if successful
     */
    bool clearVote();
    
    /**
     * Save this comment
     * @return true if successful
     */
    bool save();
    
    /**
     * Unsave this comment
     * @return true if successful
     */
    bool unsave();
    
    /**
     * Reply to this comment
     * @param text Reply text
     * @return true if successful
     */
    bool reply(const String& text);
    
    /**
     * Edit this comment
     * @param text New text
     * @return true if successful
     */
    bool edit(const String& text);
    
    /**
     * Delete this comment
     * @return true if successful
     */
    bool deleteComment();

private:
    String _body;
    String _author;
    String _subreddit;
    String _parentId;
    String _linkId;
    String _permalink;
    int _score;
    int _depth;
    bool _isSubmitter;
    bool _scoreHidden;
    
    /**
     * Perform vote action
     * @param dir Vote direction (-1, 0, 1)
     * @return true if successful
     */
    bool vote(int dir);
};

#endif // COMMENT_H
