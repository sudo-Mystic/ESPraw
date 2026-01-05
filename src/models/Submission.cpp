/**
 * Submission.cpp - Reddit submission implementation
 */

#include "Submission.h"
#include "../ESPraw.h"

Submission::Submission(ESPraw* espraw, JsonObject data)
    : RedditBase(espraw, data), _score(0), _upvoteRatio(0), _numComments(0),
      _over18(false), _spoiler(false), _locked(false), _stickied(false), _isSelf(false) {
}

Submission::~Submission() {
}

void Submission::parseData(JsonObject data) {
    // Parse base fields
    RedditBase::parseData(data);
    
    // Parse submission-specific fields
    _title = extractString(data, "title");
    _author = extractString(data, "author");
    _subreddit = extractString(data, "subreddit");
    _selftext = extractString(data, "selftext");
    _url = extractString(data, "url");
    _domain = extractString(data, "domain");
    _permalink = extractString(data, "permalink");
    
    _score = extractInt(data, "score");
    _upvoteRatio = extractInt(data, "upvote_ratio", 50);
    _numComments = extractInt(data, "num_comments");
    
    _over18 = extractBool(data, "over_18");
    _spoiler = extractBool(data, "spoiler");
    _locked = extractBool(data, "locked");
    _stickied = extractBool(data, "stickied");
    _isSelf = extractBool(data, "is_self");
}

bool Submission::upvote() {
    return vote(1);
}

bool Submission::downvote() {
    return vote(-1);
}

bool Submission::clearVote() {
    return vote(0);
}

bool Submission::vote(int dir) {
    if (!_espraw || _fullname.isEmpty()) {
        return false;
    }
    
    String body = "id=" + _fullname + "&dir=" + String(dir);
    ESPrawResponse response = _espraw->post("/api/vote", body);
    
    return response.success;
}

bool Submission::save() {
    if (!_espraw || _fullname.isEmpty()) {
        return false;
    }
    
    String body = "id=" + _fullname;
    ESPrawResponse response = _espraw->post("/api/save", body);
    
    return response.success;
}

bool Submission::unsave() {
    if (!_espraw || _fullname.isEmpty()) {
        return false;
    }
    
    String body = "id=" + _fullname;
    ESPrawResponse response = _espraw->post("/api/unsave", body);
    
    return response.success;
}

bool Submission::reply(const String& text) {
    if (!_espraw || _fullname.isEmpty() || text.isEmpty()) {
        return false;
    }
    
    String body = "thing_id=" + _fullname + "&text=" + text;
    ESPrawResponse response = _espraw->post("/api/comment", body);
    
    return response.success;
}

bool Submission::edit(const String& text) {
    if (!_espraw || _fullname.isEmpty() || text.isEmpty() || !_isSelf) {
        return false;
    }
    
    String body = "thing_id=" + _fullname + "&text=" + text;
    ESPrawResponse response = _espraw->post("/api/editusertext", body);
    
    return response.success;
}

bool Submission::deleteSubmission() {
    if (!_espraw || _fullname.isEmpty()) {
        return false;
    }
    
    String body = "id=" + _fullname;
    ESPrawResponse response = _espraw->post("/api/del", body);
    
    return response.success;
}

bool Submission::getComments(DynamicJsonDocument& doc, int limit) {
    if (!_espraw || _id.isEmpty()) {
        return false;
    }
    
    String endpoint = "/comments/" + _id;
    String params = "limit=" + String(limit);
    ESPrawResponse response = _espraw->get(endpoint, params);
    
    if (!response.success) {
        return false;
    }
    
    DeserializationError error = deserializeJson(doc, response.body);
    return !error;
}
