# Contributing to ESPraw

Thank you for your interest in contributing to ESPraw! This document provides guidelines and instructions for contributing.

## Code of Conduct

By participating in this project, you agree to abide by our Code of Conduct. Be respectful, inclusive, and considerate in all interactions.

## How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check existing issues to avoid duplicates. When creating a bug report, include:

- **Clear title and description**
- **Steps to reproduce** the issue
- **Expected behavior** vs actual behavior
- **ESP32 board type** and version
- **Library version**
- **Code snippet** that demonstrates the issue
- **Serial output** if applicable

### Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues. When creating an enhancement suggestion, include:

- **Clear title and description**
- **Use case** - why this enhancement would be useful
- **Proposed implementation** if you have ideas
- **Examples** from other libraries if applicable

### Pull Requests

1. **Fork the repository** and create your branch from `main`
2. **Follow the coding style** used in the project
3. **Add tests** if adding new functionality
4. **Update documentation** if changing APIs
5. **Ensure CI passes** before submitting
6. **Write clear commit messages**

## Development Setup

### Requirements

- **PlatformIO** (recommended) or Arduino IDE
- **ESP32 board** for hardware testing
- **Python 3.8+** for running tools
- **Git** for version control

### Setting Up Development Environment

1. Clone your fork:
```bash
git clone https://github.com/YOUR_USERNAME/ESPraw.git
cd ESPraw
```

2. Install PlatformIO:
```bash
pip install platformio
```

3. Install dependencies:
```bash
pio lib install
```

4. Build examples:
```bash
pio ci --lib="." --board=esp32dev examples/BasicAuth/BasicAuth.ino
```

### Running Tests

Run unit tests:
```bash
pio test -e native
```

Run on hardware:
```bash
pio test -e esp32dev --upload-port /dev/ttyUSB0
```

## Coding Style

### C++ Style Guidelines

- **Indentation**: 4 spaces (no tabs)
- **Line length**: Maximum 100 characters
- **Naming conventions**:
  - Classes: `PascalCase` (e.g., `ESPraw`, `Submission`)
  - Functions: `camelCase` (e.g., `getToken()`, `submitText()`)
  - Private members: `_camelCase` with underscore prefix (e.g., `_accessToken`)
  - Constants: `UPPER_SNAKE_CASE` (e.g., `ESPRAW_VERSION`)

### Documentation

- **All public APIs** must have Doxygen comments
- **Examples** should have clear comments explaining what they do
- **Update README** if adding major features

Example documentation:
```cpp
/**
 * Submit a text post to this subreddit
 * @param title Post title
 * @param text Post text
 * @return true if successful
 */
bool submitText(const String& title, const String& text);
```

## Project Structure

```
ESPraw/
├── src/                      # Library source code
│   ├── ESPraw.h/cpp         # Main library files
│   ├── ESPrawAuth.h/cpp     # Authentication
│   ├── ESPrawClient.h/cpp   # HTTP client
│   └── models/              # Reddit object models
├── examples/                 # Example sketches
├── test/                     # Unit and integration tests
├── .github/workflows/        # CI/CD workflows
└── docs/                     # Documentation
```

## Adding New Features

### Adding a New Reddit Object Type

1. Create header file in `src/models/NewType.h`
2. Inherit from `RedditBase`
3. Implement required methods
4. Add factory method to `ESPraw` class
5. Create example sketch
6. Add tests
7. Update documentation

### Adding a New API Endpoint

1. Add method to appropriate model class
2. Implement HTTP request logic
3. Handle response and errors
4. Add rate limiting if needed
5. Create example usage
6. Add tests
7. Update API reference

## Testing Guidelines

### Unit Tests

- Test individual components in isolation
- Mock external dependencies
- Cover edge cases and error conditions
- Keep tests fast and focused

### Integration Tests

- Test complete workflows
- Use mock Reddit API server when possible
- Test error handling and recovery
- Verify rate limiting works

### Hardware Tests

- Test on multiple ESP32 variants
- Verify memory usage is acceptable
- Test WiFi reconnection scenarios
- Measure performance

## Commit Message Guidelines

Use clear and descriptive commit messages:

```
Add support for fetching user's saved posts

- Implement Redditor.getSaved() method
- Add example sketch demonstrating usage
- Update API documentation
- Add unit tests for new method

Closes #123
```

Format:
- First line: Brief summary (50 chars or less)
- Blank line
- Detailed description if needed
- Reference issues/PRs

## Release Process

Releases are handled by maintainers:

1. Update version in `library.properties`
2. Update `CHANGELOG.md`
3. Create git tag: `git tag v0.1.0`
4. Push tag: `git push origin v0.1.0`
5. GitHub Actions will create release automatically

## Questions?

- Open an issue for questions about development
- Check existing documentation
- Look at example code for patterns

## License

By contributing, you agree that your contributions will be licensed under the BSD 2-Clause License.

Thank you for contributing to ESPraw!
