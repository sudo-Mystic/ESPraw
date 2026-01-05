/**
 * FetchHotPosts.ino - Fetch hot posts example for ESPraw
 * 
 * This example demonstrates how to fetch hot posts from a subreddit
 * using ESPraw library.
 */

#include <WiFi.h>
#include <ESPraw.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Reddit API credentials (for read-only, you can use simpler credentials)
const char* clientId = "YOUR_CLIENT_ID";
const char* clientSecret = "YOUR_CLIENT_SECRET";

// Create ESPraw instance
ESPraw reddit;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\nESPraw Fetch Hot Posts Example");
    Serial.println("===============================\n");
    
    // Connect to WiFi
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nWiFi connected!");
    
    // Configure ESPraw for read-only access
    ESPrawAuthConfig config;
    config.clientId = clientId;
    config.clientSecret = clientSecret;
    config.userAgent = "ESPraw/0.1.0 (ESP32) FetchHotPosts Example";
    config.readOnlyMode = true; // Read-only mode doesn't need username/password
    
    // Initialize
    Serial.println("Initializing ESPraw...");
    if (reddit.begin(config)) {
        Serial.println("ESPraw initialized successfully!");
        
        // Fetch hot posts from r/esp32
        Serial.println("\nFetching hot posts from r/esp32...\n");
        
        Subreddit* esp32 = reddit.subreddit("esp32");
        
        if (esp32 != nullptr) {
            // Create a JSON document to store the results
            DynamicJsonDocument doc(16384); // 16KB buffer
            
            if (esp32->hot(doc, 5)) { // Fetch 5 hot posts
                Serial.println("Hot Posts from r/esp32:");
                Serial.println("=======================\n");
                
                // Parse and display posts
                if (doc.containsKey("data") && doc["data"].containsKey("children")) {
                    JsonArray children = doc["data"]["children"].as<JsonArray>();
                    
                    int postNum = 1;
                    for (JsonObject child : children) {
                        JsonObject data = child["data"];
                        
                        Serial.printf("Post #%d\n", postNum++);
                        Serial.println("-------");
                        Serial.print("Title: ");
                        Serial.println(data["title"].as<String>());
                        Serial.print("Author: u/");
                        Serial.println(data["author"].as<String>());
                        Serial.print("Score: ");
                        Serial.println(data["score"].as<int>());
                        Serial.print("Comments: ");
                        Serial.println(data["num_comments"].as<int>());
                        Serial.print("URL: https://reddit.com");
                        Serial.println(data["permalink"].as<String>());
                        Serial.println();
                    }
                } else {
                    Serial.println("No posts found or unexpected response format");
                }
                
            } else {
                Serial.println("Failed to fetch hot posts");
            }
            
            delete esp32;
        }
        
    } else {
        Serial.println("Failed to initialize ESPraw!");
    }
}

void loop() {
    // Nothing to do in loop for this example
    delay(1000);
}
