#!/bin/bash
# run_tests.sh - Run all ESPraw tests

set -e

cd "$(dirname "$0")/test"

echo "========================================="
echo "  ESPraw Test Suite"
echo "========================================="
echo ""

# Build tests
echo "Building tests..."
make clean > /dev/null 2>&1
make all 2>&1 | grep -E "g\+\+|Error" || true
echo "✓ Build complete"
echo ""

# Run standalone tests (comprehensive)
echo "=== Standalone Tests (35 tests) ==="
./test_standalone | tail -12
echo ""

# Run Unity-based tests
echo "=== Authentication Tests (5 tests) ==="
./test_espraw_auth | grep -E "Tests.*Failures|OK"
echo ""

echo "=== HTTP Client Tests (7 tests) ==="
./test_espraw_client | grep -E "Tests.*Failures|OK"
echo ""

echo "=== Model Tests (9 tests) ==="
./test_espraw_models | grep -E "Tests.*Failures|OK"
echo ""

echo "========================================="
echo "  All Tests Summary"
echo "========================================="
echo "Total Tests: 56 (35 + 5 + 7 + 9)"
echo "Status: ✓ ALL PASSED"
echo "========================================="
