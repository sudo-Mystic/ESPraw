/**
 * Comment.cpp - Comment implementation
 */

#include "Comment.h"
#include "../ESPraw.h"

Comment::Comment(ESPraw* espraw, JsonObject data)
    : RedditBase(espraw, data), _score(0), _depth(0), 
      _isSubmitter(false), _scoreHidden(false) {
}

Comment::~Comment() {
}

void Comment::parseData(JsonObject data) {
    // Parse base fields
    RedditBase::parseData(data);
    
    // Parse comment-specific fields
    _body = extractString(data, "body");
    _author = extractString(data, "author");
    _subreddit = extractString(data, "subreddit");
    _parentId = extractString(data, "parent_id");
    _linkId = extractString(data, "link_id");
    _permalink = extractString(data, "permalink");
    
    _score = extractInt(data, "score");
    _depth = extractInt(data, "depth");
    
    _isSubmitter = extractBool(data, "is_submitter");
    _scoreHidden = extractBool(data, "score_hidden");
}

bool Comment::upvote() {
    return vote(1);
}

bool Comment::downvote() {
    return vote(-1);
}

bool Comment::clearVote() {
    return vote(0);
}

bool Comment::vote(int dir) {
    if (!_espraw || _fullname.isEmpty()) {
        return false;
    }
    
    String body = "id=" + _fullname + "&dir=" + String(dir);
    ESPrawResponse response = _espraw->post("/api/vote", body);
    
    return response.success;
}

bool Comment::save() {
    if (!_espraw || _fullname.isEmpty()) {
        return false;
    }
    
    String body = "id=" + _fullname;
    ESPrawResponse response = _espraw->post("/api/save", body);
    
    return response.success;
}

bool Comment::unsave() {
    if (!_espraw || _fullname.isEmpty()) {
        return false;
    }
    
    String body = "id=" + _fullname;
    ESPrawResponse response = _espraw->post("/api/unsave", body);
    
    return response.success;
}

bool Comment::reply(const String& text) {
    if (!_espraw || _fullname.isEmpty() || text.isEmpty()) {
        return false;
    }
    
    String body = "thing_id=" + _fullname + "&text=" + text;
    ESPrawResponse response = _espraw->post("/api/comment", body);
    
    return response.success;
}

bool Comment::edit(const String& text) {
    if (!_espraw || _fullname.isEmpty() || text.isEmpty()) {
        return false;
    }
    
    String body = "thing_id=" + _fullname + "&text=" + text;
    ESPrawResponse response = _espraw->post("/api/editusertext", body);
    
    return response.success;
}

bool Comment::deleteComment() {
    if (!_espraw || _fullname.isEmpty()) {
        return false;
    }
    
    String body = "id=" + _fullname;
    ESPrawResponse response = _espraw->post("/api/del", body);
    
    return response.success;
}
