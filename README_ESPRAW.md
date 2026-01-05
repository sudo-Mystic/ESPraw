# ESPraw: ESP32 Reddit API Wrapper

[![Arduino Library](https://img.shields.io/badge/Arduino-Library-blue.svg)](https://www.arduino.cc/reference/en/libraries/)
[![ESP32](https://img.shields.io/badge/ESP32-Compatible-green.svg)](https://www.espressif.com/en/products/socs/esp32)
[![License](https://img.shields.io/badge/License-BSD%202--Clause-orange.svg)](LICENSE.txt)

ESPraw is a comprehensive Reddit API wrapper for ESP32 microcontrollers, modeled after the popular [PRAW (Python Reddit API Wrapper)](https://github.com/praw-dev/praw). This library enables ESP32 devices to interact with Reddit's API, allowing IoT projects to read posts, submit content, reply to comments, and more.

## Features

- 🔐 **OAuth2 Authentication**: Full support for Reddit's OAuth2 authentication
- 📖 **Read-Only Mode**: Access public content without user credentials
- 📝 **Full Reddit API**: Submit posts, reply to comments, vote, save, and more
- 🎯 **PRAW-Compatible API**: Familiar API design for PRAW users
- 💾 **Memory Efficient**: Optimized for ESP32's limited resources
- ⚡ **Rate Limiting**: Built-in rate limit handling
- 🔄 **Automatic Token Refresh**: Handles token expiration automatically

## Supported Operations

### Subreddit Operations
- Fetch hot, new, top, rising, and controversial posts
- Submit text and link posts
- Subscribe/unsubscribe to subreddits
- Get subreddit information

### Post/Submission Operations
- Fetch submission details
- Vote (upvote/downvote)
- Save/unsave posts
- Reply to posts
- Edit post text (for self posts)
- Delete posts

### Comment Operations
- Vote on comments
- Reply to comments
- Edit comments
- Delete comments
- Save/unsave comments

### User Operations
- Get user information
- Fetch user's submissions and comments
- Get authenticated user info

## Installation

### Arduino IDE

1. Download the latest release from the [releases page](https://github.com/sudo-Mystic/ESPraw/releases)
2. In Arduino IDE, go to `Sketch` → `Include Library` → `Add .ZIP Library`
3. Select the downloaded ZIP file

### PlatformIO

Add to your `platformio.ini`:

```ini
lib_deps =
    ESPraw
    bblanchon/ArduinoJson@^6.21.0
```

### Manual Installation

1. Clone this repository into your Arduino libraries folder:
```bash
cd ~/Arduino/libraries
git clone https://github.com/sudo-Mystic/ESPraw.git
```

## Dependencies

- **ArduinoJson** (v6.x) - JSON parsing and serialization
- **WiFiClientSecure** - HTTPS support (included with ESP32 core)
- **HTTPClient** - HTTP operations (included with ESP32 core)

## Quick Start

### 1. Create a Reddit App

1. Go to https://www.reddit.com/prefs/apps
2. Click "create another app..."
3. Select "script" as the app type
4. Note your `client_id` (under the app name) and `client_secret`

### 2. Basic Example

```cpp
#include <WiFi.h>
#include <ESPraw.h>

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

ESPraw reddit;

void setup() {
    Serial.begin(115200);
    
    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    
    // Configure authentication
    ESPrawAuthConfig config;
    config.clientId = "your_client_id";
    config.clientSecret = "your_client_secret";
    config.username = "your_username";
    config.password = "your_password";
    config.userAgent = "ESPraw/0.1.0 (ESP32)";
    
    // Initialize and authenticate
    if (reddit.begin(config)) {
        Serial.println("Authenticated!");
        
        // Fetch hot posts from r/esp32
        Subreddit* sub = reddit.subreddit("esp32");
        DynamicJsonDocument doc(16384);
        
        if (sub->hot(doc, 5)) {
            // Process posts...
        }
        
        delete sub;
    }
}

void loop() {
    delay(1000);
}
```

### 3. Read-Only Mode

For read-only access (no username/password needed):

```cpp
ESPrawAuthConfig config;
config.clientId = "your_client_id";
config.clientSecret = "your_client_secret";
config.userAgent = "ESPraw/0.1.0 (ESP32)";
config.readOnlyMode = true;

reddit.begin(config);
```

## API Reference

### ESPraw Class

#### Initialization

```cpp
bool begin(const ESPrawAuthConfig& authConfig);
bool authenticate();
bool isAuthenticated();
```

#### Getting Reddit Objects

```cpp
Subreddit* subreddit(const String& name);
Submission* submission(const String& id);
Submission* submissionByUrl(const String& url);
Comment* comment(const String& id);
Redditor* redditor(const String& username);
Redditor* me();  // Get current authenticated user
```

#### Read-Only Mode

```cpp
bool setReadOnly(bool readOnly);
bool isReadOnly();
```

### Subreddit Class

```cpp
bool hot(DynamicJsonDocument& doc, int limit = 25);
bool new_(DynamicJsonDocument& doc, int limit = 25);
bool top(DynamicJsonDocument& doc, const String& timeFilter = "day", int limit = 25);
bool rising(DynamicJsonDocument& doc, int limit = 25);
bool controversial(DynamicJsonDocument& doc, const String& timeFilter = "day", int limit = 25);

bool submitText(const String& title, const String& text);
bool submitLink(const String& title, const String& url);

bool subscribe();
bool unsubscribe();
bool fetch();  // Get subreddit info
```

### Submission Class

```cpp
// Getters
String getTitle();
String getAuthor();
String getSubreddit();
String getSelftext();
String getUrl();
int getScore();
int getNumComments();

// Actions
bool upvote();
bool downvote();
bool clearVote();
bool save();
bool unsave();
bool reply(const String& text);
bool edit(const String& text);
bool deleteSubmission();
bool getComments(DynamicJsonDocument& doc, int limit = 10);
```

### Comment Class

```cpp
// Getters
String getBody();
String getAuthor();
int getScore();

// Actions
bool upvote();
bool downvote();
bool clearVote();
bool save();
bool unsave();
bool reply(const String& text);
bool edit(const String& text);
bool deleteComment();
```

### Redditor Class

```cpp
// Getters
String getUsername();
int getLinkKarma();
int getCommentKarma();

// Actions
bool fetch();  // Get user info
bool getSubmissions(DynamicJsonDocument& doc, int limit = 25);
bool getComments(DynamicJsonDocument& doc, int limit = 25);
```

## Examples

The library includes several example sketches:

- **BasicAuth** - Demonstrates authentication and basic API usage
- **FetchHotPosts** - Fetches and displays hot posts from a subreddit
- **SubmitPost** - Submits a post to a subreddit
- **ReplyToComment** - Replies to comments

See the [examples](examples/) directory for complete code.

## Memory Considerations

ESP32 has limited RAM (~520KB total, ~300KB available for applications). Keep these tips in mind:

1. **Use smaller JSON buffers** when possible
2. **Fetch fewer items** per request (use the `limit` parameter)
3. **Delete objects** after use to free memory
4. **Process items incrementally** rather than loading everything at once

Example:
```cpp
DynamicJsonDocument doc(8192);  // Adjust size based on your needs
if (sub->hot(doc, 5)) {  // Fetch only 5 posts
    // Process posts
}
```

## Rate Limiting

Reddit's API has rate limits (60 requests per minute). ESPraw automatically:
- Tracks request timing
- Waits when rate limit is reached
- Handles `429 (Too Many Requests)` responses

## Troubleshooting

### Authentication Fails
- Verify your client_id and client_secret
- Check that your Reddit app is set to "script" type
- Ensure username and password are correct
- Check that WiFi is connected

### Out of Memory Errors
- Reduce JSON buffer sizes
- Fetch fewer items per request
- Delete objects after use
- Enable PSRAM if available on your board

### SSL/HTTPS Errors
- Ensure your ESP32 has enough free heap
- Consider using `setInsecure()` for testing (not recommended for production)

## API Compatibility with PRAW

ESPraw aims to provide a 1:1 API mapping with PRAW where possible:

| PRAW (Python) | ESPraw (C++) | Status |
|---------------|--------------|--------|
| `praw.Reddit()` | `ESPraw()` | ✅ |
| `reddit.subreddit(name)` | `reddit.subreddit(name)` | ✅ |
| `subreddit.hot()` | `subreddit.hot()` | ✅ |
| `submission.upvote()` | `submission.upvote()` | ✅ |
| `submission.reply()` | `submission.reply()` | ✅ |

See [PROJECT_PLAN.md](PROJECT_PLAN.md) for detailed API mapping.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

ESPraw is released under the BSD 2-Clause License. See [LICENSE.txt](LICENSE.txt) for details.

## Acknowledgments

- Inspired by [PRAW (Python Reddit API Wrapper)](https://github.com/praw-dev/praw)
- Uses [ArduinoJson](https://arduinojson.org/) for JSON parsing
- Built on the [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)

## Support

- 📚 [Documentation](PROJECT_PLAN.md)
- 🐛 [Issue Tracker](https://github.com/sudo-Mystic/ESPraw/issues)
- 💬 [Discussions](https://github.com/sudo-Mystic/ESPraw/discussions)

---

**Note**: This library is not affiliated with or endorsed by Reddit, Inc.
