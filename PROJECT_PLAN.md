# ESPraw: ESP32 Reddit API Wrapper

## Project Overview

ESPraw is a 1:1 Reddit API wrapper for ESP32 microcontrollers, modeled after the Python Reddit API Wrapper (PRAW). This library enables ESP32 devices to interact with Reddit's API, allowing IoT projects to read posts, submit content, reply to comments, and more.

## Architecture Design

### Core Design Principles

1. **PRAW Compatibility**: Mirror PRAW's API structure and naming conventions where possible
2. **Memory Efficiency**: Optimize for ESP32's limited RAM (520KB)
3. **Non-Blocking Operations**: Support asynchronous patterns where beneficial
4. **Modular Design**: Allow users to include only needed components
5. **Error Resilience**: Robust error handling and retry logic

### Component Architecture

```
ESPraw/
├── src/
│   ├── ESPraw.h                    # Main library header
│   ├── ESPraw.cpp                  # Main library implementation
│   ├── ESPrawClient.h              # HTTP client wrapper
│   ├── ESPrawClient.cpp
│   ├── ESPrawAuth.h                # OAuth2 authentication
│   ├── ESPrawAuth.cpp
│   ├── ESPrawConfig.h              # Configuration structures
│   ├── models/
│   │   ├── RedditBase.h            # Base class for all models
│   │   ├── RedditBase.cpp
│   │   ├── Submission.h            # Post/submission model
│   │   ├── Submission.cpp
│   │   ├── Comment.h               # Comment model
│   │   ├── Comment.cpp
│   │   ├── Subreddit.h             # Subreddit model
│   │   ├── Subreddit.cpp
│   │   ├── Redditor.h              # User/redditor model
│   │   ├── Redditor.cpp
│   │   └── mixins/
│   │       ├── Votable.h           # Voting mixin
│   │       ├── Replyable.h         # Reply mixin
│   │       ├── Editable.h          # Edit mixin
│   │       └── Savable.h           # Save/unsave mixin
│   └── util/
│       ├── ESPrawJSON.h            # JSON parsing utilities
│       ├── ESPrawJSON.cpp
│       ├── ESPrawUtil.h            # General utilities
│       └── ESPrawUtil.cpp
├── examples/
│   ├── BasicAuth/
│   │   └── BasicAuth.ino
│   ├── FetchHotPosts/
│   │   └── FetchHotPosts.ino
│   ├── SubmitPost/
│   │   └── SubmitPost.ino
│   └── ReplyToComment/
│       └── ReplyToComment.ino
├── test/
│   ├── test_auth/
│   ├── test_client/
│   ├── test_models/
│   └── test_integration/
└── library.properties
```

## API Mapping: PRAW to ESPraw

### Core Reddit Class

| PRAW (Python) | ESPraw (C++) | Status |
|---------------|--------------|--------|
| `praw.Reddit()` | `ESPraw()` | Planned |
| `reddit.read_only` | `espraw.setReadOnly()` | Planned |
| `reddit.user.me()` | `espraw.user.me()` | Planned |
| `reddit.subreddit(name)` | `espraw.subreddit(name)` | Planned |
| `reddit.submission(id)` | `espraw.submission(id)` | Planned |
| `reddit.redditor(name)` | `espraw.redditor(name)` | Planned |

### Subreddit Operations

| PRAW (Python) | ESPraw (C++) | Status |
|---------------|--------------|--------|
| `subreddit.hot()` | `subreddit.hot()` | Planned |
| `subreddit.new()` | `subreddit.new_()` | Planned |
| `subreddit.top()` | `subreddit.top()` | Planned |
| `subreddit.submit()` | `subreddit.submit()` | Planned |
| `subreddit.subscribe()` | `subreddit.subscribe()` | Planned |

### Submission Operations

| PRAW (Python) | ESPraw (C++) | Status |
|---------------|--------------|--------|
| `submission.upvote()` | `submission.upvote()` | Planned |
| `submission.downvote()` | `submission.downvote()` | Planned |
| `submission.reply()` | `submission.reply()` | Planned |
| `submission.edit()` | `submission.edit()` | Planned |
| `submission.save()` | `submission.save()` | Planned |
| `submission.comments` | `submission.getComments()` | Planned |

## Technical Specifications

### Dependencies

- **ArduinoJson** (v6.x): Efficient JSON parsing
- **WiFiClientSecure**: HTTPS support
- **HTTPClient**: HTTP operations
- **ESP32 Arduino Core**: Base platform support

### Memory Considerations

- **Streaming JSON**: Parse large responses in chunks
- **String Pooling**: Reuse common strings
- **Selective Loading**: Only load needed fields
- **Comment Pagination**: Limit comments loaded at once

### Authentication

ESPraw will support multiple authentication methods:

1. **Read-Only Mode**: Access public content without authentication
2. **Script Authentication**: Username/password + client credentials
3. **OAuth2 Device Flow**: For interactive applications (future)

### Rate Limiting

- Respect Reddit's rate limits (60 requests/minute)
- Implement exponential backoff on errors
- Queue requests when necessary
- Track request timing

## Testing Strategy

### Unit Tests

- Authentication flow
- HTTP client operations
- JSON parsing
- Model instantiation
- Error handling

### Integration Tests

- Mock Reddit API server
- Full workflow tests
- Error scenario tests
- Rate limit handling

### Hardware Tests

- ESP32 DevKit C
- ESP32-S2
- ESP32-C3

## CI/CD Pipeline

### GitHub Actions Workflows

1. **Build Check**: Compile for multiple ESP32 variants
2. **Unit Tests**: Run unit tests on x86 platform
3. **Integration Tests**: Run with mock server
4. **Linting**: Check code style
5. **Release**: Automated library packaging

## Development Phases

### Phase 1: Foundation (Week 1-2)
- Project structure setup
- Basic HTTP client
- OAuth2 implementation
- Configuration system

### Phase 2: Core Models (Week 3-4)
- Base model classes
- Submission model
- Comment model
- Subreddit model
- Redditor model

### Phase 3: Operations (Week 5-6)
- Read operations (fetch posts, comments)
- Write operations (submit, reply)
- Vote operations
- Save operations

### Phase 4: Testing (Week 7-8)
- Unit test suite
- Integration test suite
- CI/CD setup
- Documentation

### Phase 5: Polish (Week 9-10)
- Performance optimization
- Memory optimization
- Example sketches
- Final documentation

## Success Metrics

1. **Functional Completeness**: Cover 80% of common PRAW operations
2. **Memory Efficiency**: Run on ESP32 with <200KB heap usage
3. **Reliability**: 99% success rate on valid operations
4. **Performance**: <2s for simple GET operations
5. **Documentation**: Complete API documentation and 5+ examples

## Future Enhancements

1. **Async Operations**: Full async/await support
2. **Caching**: Local cache for frequently accessed data
3. **Advanced Features**: Moderation tools, flair management
4. **WebSocket Support**: Real-time updates
5. **Multi-platform**: Port to other microcontrollers

## References

- PRAW Documentation: https://praw.readthedocs.io/
- Reddit API Documentation: https://www.reddit.com/dev/api/
- ESP32 Arduino Core: https://github.com/espressif/arduino-esp32
- ArduinoJson: https://arduinojson.org/

## License

ESPraw will be released under the BSD 2-Clause License, matching PRAW's licensing.
