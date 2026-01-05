/**
 * BasicAuth.ino - Basic authentication example for ESPraw
 * 
 * This example demonstrates how to authenticate with Reddit's API
 * using ESPraw library.
 * 
 * Before running this example:
 * 1. Create a Reddit app at https://www.reddit.com/prefs/apps
 * 2. Select "script" as the app type
 * 3. Note your client_id and client_secret
 * 4. Replace the credentials below with your own
 */

#include <WiFi.h>
#include <ESPraw.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Reddit API credentials
const char* clientId = "YOUR_CLIENT_ID";
const char* clientSecret = "YOUR_CLIENT_SECRET";
const char* username = "YOUR_REDDIT_USERNAME";
const char* redditPassword = "YOUR_REDDIT_PASSWORD";

// Create ESPraw instance
ESPraw reddit;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\nESPraw Basic Authentication Example");
    Serial.println("====================================\n");
    
    // Connect to WiFi
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Configure ESPraw
    ESPrawAuthConfig config;
    config.clientId = clientId;
    config.clientSecret = clientSecret;
    config.username = username;
    config.password = redditPassword;
    config.userAgent = "ESPraw/0.1.0 (ESP32) BasicAuth Example";
    config.readOnlyMode = false;
    
    // Initialize and authenticate
    Serial.println("\nInitializing ESPraw...");
    if (reddit.begin(config)) {
        Serial.println("ESPraw initialized and authenticated successfully!");
        
        // Get current user info
        Serial.println("\nFetching user info...");
        Redditor* me = reddit.me();
        
        if (me != nullptr && me->fetch()) {
            Serial.println("\nCurrent User Information:");
            Serial.println("========================");
            Serial.print("Username: ");
            Serial.println(me->getUsername());
            Serial.print("Link Karma: ");
            Serial.println(me->getLinkKarma());
            Serial.print("Comment Karma: ");
            Serial.println(me->getCommentKarma());
            Serial.print("Has Verified Email: ");
            Serial.println(me->hasVerifiedEmail() ? "Yes" : "No");
            Serial.print("Is Gold: ");
            Serial.println(me->isGold() ? "Yes" : "No");
            
            delete me;
        } else {
            Serial.println("Failed to fetch user info");
        }
        
        // Test fetching a subreddit
        Serial.println("\nFetching r/arduino info...");
        Subreddit* arduino = reddit.subreddit("arduino");
        
        if (arduino != nullptr && arduino->fetch()) {
            Serial.println("\nSubreddit Information:");
            Serial.println("=====================");
            Serial.print("Name: r/");
            Serial.println(arduino->getDisplayName());
            Serial.print("Title: ");
            Serial.println(arduino->getTitle());
            Serial.print("Subscribers: ");
            Serial.println(arduino->getSubscribers());
            Serial.print("Active Users: ");
            Serial.println(arduino->getActiveUsers());
            
            delete arduino;
        } else {
            Serial.println("Failed to fetch subreddit info");
        }
        
    } else {
        Serial.println("Failed to initialize ESPraw!");
        Serial.println("Please check your credentials and internet connection.");
    }
}

void loop() {
    // Nothing to do in loop for this example
    delay(1000);
}
