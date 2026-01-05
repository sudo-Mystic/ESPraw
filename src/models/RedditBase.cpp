/**
 * RedditBase.cpp - Base class implementation
 */

#include "RedditBase.h"
#include "../ESPraw.h"

RedditBase::RedditBase(ESPraw* espraw, JsonObject data) 
    : _espraw(espraw), _created(0), _createdUtc(0), _valid(false) {
    if (!data.isNull()) {
        parseData(data);
    }
}

RedditBase::~RedditBase() {
}

String RedditBase::getId() const {
    return _id;
}

String RedditBase::getFullname() const {
    return _fullname;
}

String RedditBase::getKind() const {
    return _kind;
}

unsigned long RedditBase::getCreated() const {
    return _created;
}

unsigned long RedditBase::getCreatedUtc() const {
    return _createdUtc;
}

void RedditBase::parseData(JsonObject data) {
    _id = extractString(data, "id");
    _kind = extractString(data, "kind");
    _fullname = extractString(data, "name");
    _created = extractULong(data, "created");
    _createdUtc = extractULong(data, "created_utc");
    
    _valid = !_id.isEmpty();
}

bool RedditBase::isValid() const {
    return _valid;
}

String RedditBase::extractString(JsonObject data, const char* key, const String& defaultValue) const {
    if (data.containsKey(key) && !data[key].isNull()) {
        return data[key].as<String>();
    }
    return defaultValue;
}

int RedditBase::extractInt(JsonObject data, const char* key, int defaultValue) const {
    if (data.containsKey(key) && !data[key].isNull()) {
        return data[key].as<int>();
    }
    return defaultValue;
}

bool RedditBase::extractBool(JsonObject data, const char* key, bool defaultValue) const {
    if (data.containsKey(key) && !data[key].isNull()) {
        return data[key].as<bool>();
    }
    return defaultValue;
}

unsigned long RedditBase::extractULong(JsonObject data, const char* key, unsigned long defaultValue) const {
    if (data.containsKey(key) && !data[key].isNull()) {
        return data[key].as<unsigned long>();
    }
    return defaultValue;
}
