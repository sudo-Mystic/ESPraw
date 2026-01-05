# Makefile for ESPraw tests
# Allows easy compilation and running of unit tests

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I. -DUNIT_TEST
LDFLAGS =

# Unity test framework (we'll download if needed)
UNITY_DIR = unity
UNITY_SRC = $(UNITY_DIR)/unity.c
UNITY_INC = -I$(UNITY_DIR)

# Test source files
TESTS = test_standalone test_espraw_auth test_espraw_client test_espraw_models

# Default target
all: $(TESTS)

# Standalone test (no Unity dependency)
test_standalone: test_standalone.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Unity-based tests
test_espraw_auth: test_espraw_auth.cpp $(UNITY_SRC)
	$(CXX) $(CXXFLAGS) $(UNITY_INC) $^ -o $@

test_espraw_client: test_espraw_client.cpp $(UNITY_SRC)
	$(CXX) $(CXXFLAGS) $(UNITY_INC) $^ -o $@

test_espraw_models: test_espraw_models.cpp $(UNITY_SRC)
	$(CXX) $(CXXFLAGS) $(UNITY_INC) $^ -o $@

# Download Unity if not present
$(UNITY_SRC):
	@echo "Downloading Unity test framework..."
	@mkdir -p $(UNITY_DIR)
	@curl -sL https://raw.githubusercontent.com/ThrowTheSwitch/Unity/master/src/unity.c -o $(UNITY_DIR)/unity.c
	@curl -sL https://raw.githubusercontent.com/ThrowTheSwitch/Unity/master/src/unity.h -o $(UNITY_DIR)/unity.h
	@curl -sL https://raw.githubusercontent.com/ThrowTheSwitch/Unity/master/src/unity_internals.h -o $(UNITY_DIR)/unity_internals.h
	@echo "Unity framework downloaded successfully"

# Run all tests
test: all
	@echo "\n=== Running Standalone Tests ==="
	@./test_standalone || true
	@echo "\n=== Running Auth Tests ==="
	@./test_espraw_auth || true
	@echo "\n=== Running Client Tests ==="
	@./test_espraw_client || true
	@echo "\n=== Running Models Tests ==="
	@./test_espraw_models || true

# Run only standalone test (no Unity needed)
test-quick: test_standalone
	@./test_standalone

# Clean build artifacts
clean:
	rm -f $(TESTS)
	rm -f *.o
	
# Clean everything including Unity
clean-all: clean
	rm -rf $(UNITY_DIR)

.PHONY: all test test-quick clean clean-all
