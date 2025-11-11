#!/bin/bash

# RouterSwitch Test Script
# Tests basic functionality of the C implementation

set -e

echo "Starting RouterSwitch tests..."

# Test 1: Build the project
echo "Test 1: Building the project..."
make clean
make all

if [ ! -f "./router-switch" ]; then
    echo "FAIL: Binary not found after build"
    exit 1
fi
echo "PASS: Binary created successfully"

# Test 2: Help command
echo "Test 2: Testing help command..."
./router-switch --help > /dev/null
if [ $? -eq 0 ]; then
    echo "PASS: Help command works"
else
    echo "FAIL: Help command failed"
    exit 1
fi

# Test 3: Version command
echo "Test 3: Testing version command..."
./router-switch --version > /dev/null
if [ $? -eq 0 ]; then
    echo "PASS: Version command works"
else
    echo "FAIL: Version command failed"
    exit 1
fi

# Test 4: Configuration loading
echo "Test 4: Testing configuration loading..."
if [ ! -f "./config.json" ]; then
    echo "FAIL: config.json not found"
    exit 1
fi

# Test 5: Provider switching (dry run)
echo "Test 5: Testing provider switching..."
if [ -f "./config.json" ]; then
    # Just test that it doesn't error out
    ./router-switch --provider deepseek --model deepseek-chat > /tmp/test_output.txt 2>&1
    if [ $? -eq 0 ]; then
        echo "PASS: Provider switching works"
        echo "Sample output:"
        head -3 /tmp/test_output.txt
    else
        echo "FAIL: Provider switching failed"
        cat /tmp/test_output.txt
        exit 1
    fi
else
    echo "SKIP: Provider switching test (no config.json)"
fi

# Test 6: Error handling - missing provider
echo "Test 6: Testing error handling for missing provider..."
./router-switch --provider nonexistent > /tmp/error_output.txt 2>&1
if [ $? -ne 0 ]; then
    echo "PASS: Error handling works for missing provider"
else
    echo "FAIL: Should have failed for missing provider"
    exit 1
fi

# Test 7: Error handling - no provider specified
echo "Test 7: Testing error handling for no provider..."
./router-switch > /tmp/no_provider_output.txt 2>&1
if [ $? -ne 0 ]; then
    echo "PASS: Error handling works for no provider"
else
    echo "FAIL: Should have failed for no provider"
    exit 1
fi

# Cleanup
rm -f /tmp/test_output.txt /tmp/error_output.txt /tmp/no_provider_output.txt

echo "All tests passed! ✅"