/**
 * Redditor.cpp - Redditor implementation
 */

#include "Redditor.h"
#include "../ESPraw.h"

Redditor::Redditor(ESPraw* espraw, const String& username)
    : RedditBase(espraw, JsonObject()), _linkKarma(0), _commentKarma(0),
      _hasVerifiedEmail(false), _isGold(false), _isMod(false), _isEmployee(false) {
    _username = username;
}

Redditor::Redditor(ESPraw* espraw, JsonObject data)
    : RedditBase(espraw, data), _linkKarma(0), _commentKarma(0),
      _hasVerifiedEmail(false), _isGold(false), _isMod(false), _isEmployee(false) {
}

Redditor::~Redditor() {
}

void Redditor::parseData(JsonObject data) {
    // Parse base fields
    RedditBase::parseData(data);
    
    // Parse redditor-specific fields
    _username = extractString(data, "name");
    _linkKarma = extractInt(data, "link_karma");
    _commentKarma = extractInt(data, "comment_karma");
    
    _hasVerifiedEmail = extractBool(data, "has_verified_email");
    _isGold = extractBool(data, "is_gold");
    _isMod = extractBool(data, "is_mod");
    _isEmployee = extractBool(data, "is_employee");
}

bool Redditor::fetch() {
    if (!_espraw || _username.isEmpty()) {
        return false;
    }
    
    String endpoint = "/user/" + _username + "/about";
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

bool Redditor::getSubmissions(DynamicJsonDocument& doc, int limit) {
    return fetchUserContent(doc, "submitted", limit);
}

bool Redditor::getComments(DynamicJsonDocument& doc, int limit) {
    return fetchUserContent(doc, "comments", limit);
}

bool Redditor::fetchUserContent(DynamicJsonDocument& doc, const String& type, int limit) {
    if (!_espraw || _username.isEmpty()) {
        return false;
    }
    
    String endpoint = "/user/" + _username + "/" + type;
    String params = "limit=" + String(limit);
    ESPrawResponse response = _espraw->get(endpoint, params);
    
    if (!response.success) {
        return false;
    }
    
    DeserializationError error = deserializeJson(doc, response.body);
    return !error;
}
