/**
 * Subreddit.cpp - Subreddit implementation
 */

#include "Subreddit.h"
#include "../ESPraw.h"

Subreddit::Subreddit(ESPraw* espraw, const String& name)
    : RedditBase(espraw, JsonObject()), _subscribers(0), _activeUsers(0),
      _over18(false), _userIsSubscriber(false) {
    _displayName = name;
}

Subreddit::Subreddit(ESPraw* espraw, JsonObject data)
    : RedditBase(espraw, data), _subscribers(0), _activeUsers(0),
      _over18(false), _userIsSubscriber(false) {
}

Subreddit::~Subreddit() {
}

void Subreddit::parseData(JsonObject data) {
    // Parse base fields
    RedditBase::parseData(data);
    
    // Parse subreddit-specific fields
    _displayName = extractString(data, "display_name");
    _title = extractString(data, "title");
    _description = extractString(data, "description");
    _publicDescription = extractString(data, "public_description");
    
    _subscribers = extractInt(data, "subscribers");
    _activeUsers = extractInt(data, "active_user_count");
    
    _over18 = extractBool(data, "over18");
    _userIsSubscriber = extractBool(data, "user_is_subscriber");
}

bool Subreddit::hot(DynamicJsonDocument& doc, int limit) {
    return fetchPosts(doc, "hot", "limit=" + String(limit));
}

bool Subreddit::new_(DynamicJsonDocument& doc, int limit) {
    return fetchPosts(doc, "new", "limit=" + String(limit));
}

bool Subreddit::top(DynamicJsonDocument& doc, const String& timeFilter, int limit) {
    String params = "limit=" + String(limit) + "&t=" + timeFilter;
    return fetchPosts(doc, "top", params);
}

bool Subreddit::rising(DynamicJsonDocument& doc, int limit) {
    return fetchPosts(doc, "rising", "limit=" + String(limit));
}

bool Subreddit::controversial(DynamicJsonDocument& doc, const String& timeFilter, int limit) {
    String params = "limit=" + String(limit) + "&t=" + timeFilter;
    return fetchPosts(doc, "controversial", params);
}

bool Subreddit::fetchPosts(DynamicJsonDocument& doc, const String& sort, const String& params) {
    if (!_espraw || _displayName.isEmpty()) {
        return false;
    }
    
    String endpoint = "/r/" + _displayName + "/" + sort;
    ESPrawResponse response = _espraw->get(endpoint, params);
    
    if (!response.success) {
        return false;
    }
    
    DeserializationError error = deserializeJson(doc, response.body);
    return !error;
}

bool Subreddit::submitText(const String& title, const String& text) {
    if (!_espraw || _displayName.isEmpty() || title.isEmpty()) {
        return false;
    }
    
    String body = "sr=" + _displayName + 
                  "&kind=self" +
                  "&title=" + title +
                  "&text=" + text;
    
    ESPrawResponse response = _espraw->post("/api/submit", body);
    return response.success;
}

bool Subreddit::submitLink(const String& title, const String& url) {
    if (!_espraw || _displayName.isEmpty() || title.isEmpty() || url.isEmpty()) {
        return false;
    }
    
    String body = "sr=" + _displayName + 
                  "&kind=link" +
                  "&title=" + title +
                  "&url=" + url;
    
    ESPrawResponse response = _espraw->post("/api/submit", body);
    return response.success;
}

bool Subreddit::subscribe() {
    return performSubscriptionAction("sub");
}

bool Subreddit::unsubscribe() {
    return performSubscriptionAction("unsub");
}

bool Subreddit::performSubscriptionAction(const String& action) {
    if (!_espraw || _displayName.isEmpty()) {
        return false;
    }
    
    String body = "action=" + action + "&sr_name=" + _displayName;
    ESPrawResponse response = _espraw->post("/api/subscribe", body);
    
    if (response.success) {
        _userIsSubscriber = (action == "sub");
    }
    
    return response.success;
}

bool Subreddit::fetch() {
    if (!_espraw || _displayName.isEmpty()) {
        return false;
    }
    
    String endpoint = "/r/" + _displayName + "/about";
    ESPrawResponse response = _espraw->get(endpoint);
    
    if (!response.success) {
        return false;
    }
    
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, response.body);
    
    if (error) {
        return false;
    }
    
    if (doc.containsKey("data")) {
        JsonObject data = doc["data"].as<JsonObject>();
        parseData(data);
        return true;
    }
    
    return false;
}
