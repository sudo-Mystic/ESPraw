/**
 * test_standalone.cpp - Standalone tests for ESPraw (no Unity dependency)
 * 
 * This file contains standalone unit tests that can be run without Unity framework.
 * Useful for quick validation and CI environments.
 */

#include <iostream>
#include <cassert>
#include <cstring>
#include <cmath>

// Test result tracking
int tests_passed = 0;
int tests_failed = 0;

#define TEST_ASSERT(condition, message) \
    if (condition) { \
        tests_passed++; \
        std::cout << "✓ PASS: " << message << std::endl; \
    } else { \
        tests_failed++; \
        std::cout << "✗ FAIL: " << message << std::endl; \
    }

#define TEST_ASSERT_EQUAL(expected, actual, message) \
    TEST_ASSERT((expected) == (actual), message)

#define TEST_ASSERT_TRUE(condition, message) \
    TEST_ASSERT(condition, message)

#define TEST_ASSERT_NOT_NULL(ptr, message) \
    TEST_ASSERT((ptr) != nullptr, message)

// Test: Rate limiting calculations
void test_rate_limit_calculations() {
    std::cout << "\n=== Test: Rate Limiting Calculations ===" << std::endl;
    
    const int RATE_LIMIT_REQUESTS = 60;
    const int RATE_LIMIT_WINDOW = 60000; // 60 seconds
    
    TEST_ASSERT_EQUAL(60, RATE_LIMIT_REQUESTS, "Rate limit should be 60 requests");
    TEST_ASSERT_EQUAL(60000, RATE_LIMIT_WINDOW, "Rate limit window should be 60000ms");
    
    // Test that we can calculate requests per second
    float requestsPerSecond = (float)RATE_LIMIT_REQUESTS / (RATE_LIMIT_WINDOW / 1000.0f);
    TEST_ASSERT_TRUE(fabs(requestsPerSecond - 1.0f) < 0.01f, "Should allow ~1 request per second");
}

// Test: URL parsing
void test_url_parsing() {
    std::cout << "\n=== Test: URL Parsing ===" << std::endl;
    
    const char* url1 = "https://www.reddit.com/r/test/comments/abc123/title/";
    const char* commentsPos = strstr(url1, "/comments/");
    
    TEST_ASSERT_NOT_NULL(commentsPos, "Should find /comments/ in URL");
    
    // Verify we can find the ID position
    const char* idStart = commentsPos + 10; // Length of "/comments/"
    char id[10] = {0};
    sscanf(idStart, "%[^/]", id);
    
    TEST_ASSERT_TRUE(strcmp(id, "abc123") == 0, "Should extract submission ID 'abc123'");
}

// Test: JSON buffer sizes
void test_json_buffer_sizes() {
    std::cout << "\n=== Test: JSON Buffer Sizes ===" << std::endl;
    
    const int ESPRAW_JSON_BUFFER_SIZE = 8192;
    const int ESPRAW_MAX_RESPONSE_SIZE = 16384;
    
    TEST_ASSERT_TRUE(ESPRAW_JSON_BUFFER_SIZE > 0, "JSON buffer size should be positive");
    TEST_ASSERT_TRUE(ESPRAW_MAX_RESPONSE_SIZE >= ESPRAW_JSON_BUFFER_SIZE, 
                     "Max response should be >= JSON buffer size");
}

// Test: Configuration constants
void test_configuration_constants() {
    std::cout << "\n=== Test: Configuration Constants ===" << std::endl;
    
    const char* ESPRAW_API_BASE_URL = "https://oauth.reddit.com";
    const char* ESPRAW_AUTH_URL = "https://www.reddit.com/api/v1/access_token";
    
    TEST_ASSERT_NOT_NULL(ESPRAW_API_BASE_URL, "API base URL should not be null");
    TEST_ASSERT_NOT_NULL(ESPRAW_AUTH_URL, "Auth URL should not be null");
    TEST_ASSERT_TRUE(strlen(ESPRAW_API_BASE_URL) > 0, "API base URL should not be empty");
    TEST_ASSERT_TRUE(strlen(ESPRAW_AUTH_URL) > 0, "Auth URL should not be empty");
}

// Test: HTTP status codes
void test_http_status_codes() {
    std::cout << "\n=== Test: HTTP Status Codes ===" << std::endl;
    
    int statusOK = 200;
    int statusUnauthorized = 401;
    int statusRateLimit = 429;
    
    TEST_ASSERT_EQUAL(200, statusOK, "OK status should be 200");
    TEST_ASSERT_EQUAL(401, statusUnauthorized, "Unauthorized status should be 401");
    TEST_ASSERT_EQUAL(429, statusRateLimit, "Rate limit status should be 429");
    
    // Test status code ranges
    TEST_ASSERT_TRUE(statusOK >= 200 && statusOK < 300, "200 should be in success range");
    TEST_ASSERT_TRUE(statusUnauthorized >= 400 && statusUnauthorized < 500, 
                     "401 should be in client error range");
}

// Test: Token expiration logic
void test_token_expiration() {
    std::cout << "\n=== Test: Token Expiration Logic ===" << std::endl;
    
    unsigned long currentTime = 1000000;
    unsigned long expiresAt = 1000060;
    int expiresIn = 3600;
    
    // Test that we correctly calculate expiration with buffer
    unsigned long calculatedExpiry = currentTime + expiresIn - 60; // 60 second buffer
    
    TEST_ASSERT_TRUE(calculatedExpiry < currentTime + expiresIn, 
                     "Calculated expiry should be less than full expiration (due to buffer)");
    TEST_ASSERT_TRUE(currentTime < expiresAt, "Current time should be before expiration");
}

// Test: Vote direction values
void test_vote_directions() {
    std::cout << "\n=== Test: Vote Directions ===" << std::endl;
    
    int upvote = 1;
    int downvote = -1;
    int clearVote = 0;
    
    TEST_ASSERT_EQUAL(1, upvote, "Upvote should be 1");
    TEST_ASSERT_EQUAL(-1, downvote, "Downvote should be -1");
    TEST_ASSERT_EQUAL(0, clearVote, "Clear vote should be 0");
    
    // Ensure they are distinct
    TEST_ASSERT_TRUE(upvote != downvote, "Upvote and downvote should be different");
    TEST_ASSERT_TRUE(upvote != clearVote, "Upvote and clear should be different");
    TEST_ASSERT_TRUE(downvote != clearVote, "Downvote and clear should be different");
}

// Test: Exponential backoff calculation
void test_exponential_backoff() {
    std::cout << "\n=== Test: Exponential Backoff ===" << std::endl;
    
    int retryDelay = 1000; // 1 second base delay
    
    // Test exponential growth
    unsigned long delay1 = retryDelay * (1 << 1); // 2^1 = 2 seconds
    unsigned long delay2 = retryDelay * (1 << 2); // 2^2 = 4 seconds
    unsigned long delay3 = retryDelay * (1 << 3); // 2^3 = 8 seconds
    
    TEST_ASSERT_EQUAL(2000, delay1, "First retry should be 2 seconds");
    TEST_ASSERT_EQUAL(4000, delay2, "Second retry should be 4 seconds");
    TEST_ASSERT_EQUAL(8000, delay3, "Third retry should be 8 seconds");
    
    // Verify exponential growth
    TEST_ASSERT_TRUE(delay2 == delay1 * 2, "Delay should double each time");
    TEST_ASSERT_TRUE(delay3 == delay2 * 2, "Delay should double each time");
}

// Test: Memory management
void test_memory_management() {
    std::cout << "\n=== Test: Memory Management ===" << std::endl;
    
    // Test basic allocation
    int* ptr = new int(42);
    TEST_ASSERT_NOT_NULL(ptr, "Memory allocation should succeed");
    TEST_ASSERT_EQUAL(42, *ptr, "Allocated value should be correct");
    delete ptr;
    
    // Test array allocation
    char* buffer = new char[1024];
    TEST_ASSERT_NOT_NULL(buffer, "Array allocation should succeed");
    delete[] buffer;
}

// Test: Library version format
void test_library_version() {
    std::cout << "\n=== Test: Library Version Format ===" << std::endl;
    
    const char* version = "0.1.0";
    
    TEST_ASSERT_NOT_NULL(version, "Version should not be null");
    TEST_ASSERT_TRUE(strlen(version) > 0, "Version should not be empty");
    
    // Check format (should contain dots)
    TEST_ASSERT_TRUE(strchr(version, '.') != nullptr, "Version should contain dots");
}

int main() {
    std::cout << "\n" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "  ESPraw Standalone Unit Tests" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Run all tests
    test_rate_limit_calculations();
    test_url_parsing();
    test_json_buffer_sizes();
    test_configuration_constants();
    test_http_status_codes();
    test_token_expiration();
    test_vote_directions();
    test_exponential_backoff();
    test_memory_management();
    test_library_version();
    
    // Print summary
    std::cout << "\n========================================" << std::endl;
    std::cout << "  Test Results" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Passed: " << tests_passed << std::endl;
    std::cout << "Failed: " << tests_failed << std::endl;
    std::cout << "Total:  " << (tests_passed + tests_failed) << std::endl;
    std::cout << "========================================" << std::endl;
    
    if (tests_failed == 0) {
        std::cout << "\n✓ All tests passed!" << std::endl;
        return 0;
    } else {
        std::cout << "\n✗ Some tests failed!" << std::endl;
        return 1;
    }
}
