/**
 * SubmitPost.ino - Submit a post example for ESPraw
 * 
 * This example demonstrates how to submit a post to a subreddit
 * using ESPraw library.
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
    
    Serial.println("\n\nESPraw Submit Post Example");
    Serial.println("==========================\n");
    
    // Connect to WiFi
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nWiFi connected!");
    
    // Configure ESPraw
    ESPrawAuthConfig config;
    config.clientId = clientId;
    config.clientSecret = clientSecret;
    config.username = username;
    config.password = redditPassword;
    config.userAgent = "ESPraw/0.1.0 (ESP32) SubmitPost Example";
    config.readOnlyMode = false; // Need write access
    
    // Initialize and authenticate
    Serial.println("Initializing ESPraw...");
    if (reddit.begin(config)) {
        Serial.println("ESPraw initialized successfully!");
        
        // Submit a text post to r/test
        Serial.println("\nSubmitting post to r/test...");
        
        Subreddit* testSub = reddit.subreddit("test");
        
        if (testSub != nullptr) {
            String title = "Test post from ESP32";
            String text = "This is a test post submitted from an ESP32 microcontroller using ESPraw library!\n\n";
            text += "ESPraw is a Reddit API wrapper for ESP32, modeled after PRAW.\n\n";
            text += "Posted at: " + String(millis() / 1000) + " seconds since boot.";
            
            if (testSub->submitText(title, text)) {
                Serial.println("\nPost submitted successfully!");
                Serial.println("Title: " + title);
                Serial.println("\nCheck r/test to see your post!");
            } else {
                Serial.println("\nFailed to submit post.");
                Serial.println("Make sure you have posting permissions in r/test");
            }
            
            delete testSub;
        }
        
        // You can also submit a link post
        Serial.println("\n\nSubmitting link post...");
        testSub = reddit.subreddit("test");
        
        if (testSub != nullptr) {
            String linkTitle = "ESP32 on Wikipedia";
            String url = "https://en.wikipedia.org/wiki/ESP32";
            
            if (testSub->submitLink(linkTitle, url)) {
                Serial.println("\nLink post submitted successfully!");
                Serial.println("Title: " + linkTitle);
                Serial.println("URL: " + url);
            } else {
                Serial.println("\nFailed to submit link post.");
            }
            
            delete testSub;
        }
        
    } else {
        Serial.println("Failed to initialize ESPraw!");
        Serial.println("Please check your credentials.");
    }
}

void loop() {
    // Nothing to do in loop for this example
    delay(1000);
}
