/**
 * ESPrawAuth.cpp - OAuth2 authentication implementation
 */

#include "ESPrawAuth.h"
#include <base64.h>

ESPrawAuth::ESPrawAuth() {
}

ESPrawAuth::~ESPrawAuth() {
}

bool ESPrawAuth::begin(const ESPrawAuthConfig& config) {
    _config = config;
    _secureClient.setInsecure(); // For now, skip certificate validation
    return true;
}

bool ESPrawAuth::authenticateScript() {
    if (_config.clientId.isEmpty() || _config.clientSecret.isEmpty() ||
        _config.username.isEmpty() || _config.password.isEmpty()) {
        Serial.println("Missing required authentication parameters");
        return false;
    }
    
    String params = "grant_type=password&username=" + urlEncode(_config.username) +
                   "&password=" + urlEncode(_config.password);
    
    _token = requestToken("password", params);
    return _token.isValid;
}

bool ESPrawAuth::authenticateReadOnly() {
    if (_config.clientId.isEmpty() || _config.clientSecret.isEmpty()) {
        Serial.println("Missing client credentials");
        return false;
    }
    
    String params = "grant_type=client_credentials";
    _token = requestToken("client_credentials", params);
    return _token.isValid;
}

ESPrawToken ESPrawAuth::getToken() const {
    return _token;
}

bool ESPrawAuth::isAuthenticated() const {
    return _token.isValid && !_token.isExpired();
}

bool ESPrawAuth::refreshToken() {
    // Reddit doesn't support refresh tokens in the traditional sense
    // We need to re-authenticate
    if (_config.readOnlyMode) {
        return authenticateReadOnly();
    } else {
        return authenticateScript();
    }
}

bool ESPrawAuth::revokeToken() {
    HTTPClient http;
    
    if (!http.begin(_secureClient, "https://www.reddit.com/api/v1/revoke_token")) {
        return false;
    }
    
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Authorization", "Basic " + createBasicAuth());
    
    String body = "token=" + _token.accessToken + "&token_type_hint=access_token";
    
    int httpCode = http.POST(body);
    http.end();
    
    if (httpCode == 200 || httpCode == 204) {
        _token.isValid = false;
        _token.accessToken = "";
        return true;
    }
    
    return false;
}

ESPrawToken ESPrawAuth::requestToken(const String& grantType, const String& params) {
    ESPrawToken token;
    HTTPClient http;
    
    if (!http.begin(_secureClient, ESPRAW_AUTH_URL)) {
        Serial.println("Failed to connect to auth endpoint");
        return token;
    }
    
    // Set headers
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Authorization", "Basic " + createBasicAuth());
    
    if (_config.userAgent.length() > 0) {
        http.addHeader("User-Agent", _config.userAgent);
    } else {
        http.addHeader("User-Agent", ESPRAW_USER_AGENT_FORMAT);
    }
    
    // Perform request
    int httpCode = http.POST(params);
    
    if (httpCode == 200) {
        String response = http.getString();
        
        // Parse JSON response
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, response);
        
        if (!error) {
            token = parseTokenResponse(doc);
        } else {
            Serial.println("Failed to parse token response: " + String(error.c_str()));
        }
    } else {
        Serial.println("Auth request failed with code: " + String(httpCode));
        if (httpCode > 0) {
            Serial.println("Response: " + http.getString());
        }
    }
    
    http.end();
    return token;
}

ESPrawToken ESPrawAuth::parseTokenResponse(JsonDocument& json) {
    ESPrawToken token;
    
    if (json.containsKey("access_token")) {
        token.accessToken = json["access_token"].as<String>();
        token.tokenType = json["token_type"].as<String>();
        
        if (json.containsKey("scope")) {
            token.scope = json["scope"].as<String>();
        }
        
        if (json.containsKey("expires_in")) {
            int expiresIn = json["expires_in"].as<int>();
            // Set expiration time (current time + expires_in - 60 seconds buffer)
            token.expiresAt = (millis() / 1000) + expiresIn - 60;
        }
        
        token.isValid = true;
    }
    
    return token;
}

String ESPrawAuth::createBasicAuth() const {
    String credentials = _config.clientId + ":" + _config.clientSecret;
    return base64::encode(credentials);
}

String ESPrawAuth::urlEncode(const String& str) const {
    String encoded = "";
    char c;
    char code0;
    char code1;
    
    for (unsigned int i = 0; i < str.length(); i++) {
        c = str.charAt(i);
        if (c == ' ') {
            encoded += '+';
        } else if (isalnum(c)) {
            encoded += c;
        } else {
            code1 = (c & 0xf) + '0';
            if ((c & 0xf) > 9) {
                code1 = (c & 0xf) - 10 + 'A';
            }
            c = (c >> 4) & 0xf;
            code0 = c + '0';
            if (c > 9) {
                code0 = c - 10 + 'A';
            }
            encoded += '%';
            encoded += code0;
            encoded += code1;
        }
    }
    
    return encoded;
}
