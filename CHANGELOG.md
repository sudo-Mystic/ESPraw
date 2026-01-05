# Changelog

All notable changes to ESPraw will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Initial release of ESPraw
- Core Reddit API wrapper functionality
- OAuth2 authentication support (script and read-only modes)
- HTTP client with rate limiting
- Reddit object models:
  - Subreddit (fetch posts, submit, subscribe)
  - Submission (vote, reply, edit, delete)
  - Comment (vote, reply, edit, delete)
  - Redditor (fetch user info and content)
- Example sketches:
  - BasicAuth - Authentication example
  - FetchHotPosts - Fetch and display posts
  - SubmitPost - Submit text and link posts
  - ReplyToComment - Reply to posts and comments
- CI/CD workflows:
  - Build verification for ESP32 variants
  - Unit testing framework
  - Release automation
  - Documentation checks
- Comprehensive documentation:
  - README with quick start guide
  - API reference
  - Contributing guidelines
  - Project plan

### Changed
- N/A (initial release)

### Deprecated
- N/A (initial release)

### Removed
- N/A (initial release)

### Fixed
- N/A (initial release)

### Security
- Implemented secure OAuth2 token handling
- Added rate limiting to prevent API abuse
- HTTPS connections using WiFiClientSecure

## [0.1.0] - TBD

Initial alpha release.

### Features
- Basic Reddit API operations
- Read and write support
- Memory-optimized for ESP32
- PRAW-compatible API design

### Known Limitations
- Certificate validation is disabled (requires manual implementation)
- Limited to most common Reddit API operations
- No caching mechanism yet
- Manual memory management required for returned objects

### Supported Boards
- ESP32 DevKit
- ESP32-S2
- ESP32-C3

### Dependencies
- ArduinoJson ^6.21.0
- ESP32 Arduino Core

---

For more details, see the [GitHub releases page](https://github.com/sudo-Mystic/ESPraw/releases).
