/**
 * ReplyToComment.ino - Reply to a comment example for ESPraw
 * 
 * This example demonstrates how to fetch a post and reply to its
 * first comment using ESPraw library.
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

// Post ID to reply to (change this to a real post ID from r/test)
const char* postId = "abc123";

// Create ESPraw instance
ESPraw reddit;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\nESPraw Reply to Comment Example");
    Serial.println("================================\n");
    
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
    config.userAgent = "ESPraw/0.1.0 (ESP32) ReplyToComment Example";
    config.readOnlyMode = false;
    
    // Initialize and authenticate
    Serial.println("Initializing ESPraw...");
    if (reddit.begin(config)) {
        Serial.println("ESPraw initialized successfully!");
        
        // Get the submission
        Serial.println("\nFetching submission...");
        Submission* post = reddit.submission(postId);
        
        if (post != nullptr && post->isValid()) {
            Serial.println("\nSubmission Details:");
            Serial.println("==================");
            Serial.print("Title: ");
            Serial.println(post->getTitle());
            Serial.print("Author: u/");
            Serial.println(post->getAuthor());
            Serial.print("Score: ");
            Serial.println(post->getScore());
            Serial.print("Comments: ");
            Serial.println(post->getNumComments());
            
            // Reply to the submission
            Serial.println("\n\nReplying to submission...");
            String replyText = "This is an automated reply from an ESP32 microcontroller using ESPraw!";
            
            if (post->reply(replyText)) {
                Serial.println("Reply posted successfully!");
            } else {
                Serial.println("Failed to post reply");
            }
            
            // You can also upvote the post
            Serial.println("\nUpvoting the post...");
            if (post->upvote()) {
                Serial.println("Post upvoted!");
            }
            
            // Get comments from the post
            Serial.println("\nFetching comments...");
            DynamicJsonDocument doc(16384);
            
            if (post->getComments(doc, 5)) {
                Serial.println("\nComments found!");
                
                // Parse comments from response
                if (doc.size() > 1) {  // Reddit returns [submission, comments]
                    JsonObject commentsListing = doc[1]["data"];
                    
                    if (commentsListing.containsKey("children")) {
                        JsonArray children = commentsListing["children"].as<JsonArray>();
                        
                        if (children.size() > 0) {
                            JsonObject firstComment = children[0]["data"];
                            String commentId = firstComment["name"].as<String>();
                            String commentBody = firstComment["body"].as<String>();
                            String commentAuthor = firstComment["author"].as<String>();
                            
                            Serial.println("\nFirst Comment:");
                            Serial.println("==============");
                            Serial.print("Author: u/");
                            Serial.println(commentAuthor);
                            Serial.print("Body: ");
                            Serial.println(commentBody);
                            
                            // Reply to the first comment
                            Serial.println("\nReplying to first comment...");
                            Comment* comment = reddit.comment(firstComment["id"].as<String>());
                            
                            if (comment != nullptr) {
                                comment->parseData(firstComment);
                                
                                String commentReply = "Hello from ESP32! This is a reply to your comment.";
                                if (comment->reply(commentReply)) {
                                    Serial.println("Comment reply posted successfully!");
                                } else {
                                    Serial.println("Failed to post comment reply");
                                }
                                
                                delete comment;
                            }
                        } else {
                            Serial.println("No comments found on this post");
                        }
                    }
                }
            } else {
                Serial.println("Failed to fetch comments");
            }
            
            delete post;
        } else {
            Serial.println("Failed to fetch submission or invalid post ID");
        }
        
    } else {
        Serial.println("Failed to initialize ESPraw!");
    }
}

void loop() {
    // Nothing to do in loop for this example
    delay(1000);
}
