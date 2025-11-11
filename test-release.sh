#!/bin/bash

# Test script for downloaded RouterSwitch binaries
# This script will be used to verify the GitHub Actions release

set -e

VERSION="v2.0.2"
BASE_URL="https://github.com/aokihu/claude-code-env-switcher/releases/download/${VERSION}"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}🧪 Testing RouterSwitch ${VERSION} Release${NC}"
echo "=================================="

# Test function
test_binary() {
    local binary_name="$1"
    local platform="$2"
    local download_url="${BASE_URL}/${binary_name}"
    local checksum_url="${download_url}.sha256"

    echo -e "\n${YELLOW}Testing ${platform} binary: ${binary_name}${NC}"
    echo "Download URL: ${download_url}"

    # Download binary
    echo "Downloading binary..."
    if curl -fsSL "${download_url}" -o "${binary_name}"; then
        echo -e " ✅ Binary downloaded successfully"
    else
        echo -e " ${RED}❌ Failed to download binary${NC}"
        return 1
    fi

    # Download checksum
    echo "Downloading checksum..."
    if curl -fsSL "${checksum_url}" -o "${binary_name}.sha256"; then
        echo -e " ✅ Checksum downloaded successfully"
    else
        echo -e " ${RED}❌ Failed to download checksum${NC}"
        return 1
    fi

    # Make executable
    chmod +x "${binary_name}"

    # Verify checksum
    echo "Verifying checksum..."
    if sha256sum -c "${binary_name}.sha256"; then
        echo -e " ✅ Checksum verification passed"
    else
        echo -e " ${RED}❌ Checksum verification failed${NC}"
        return 1
    fi

    # Test functionality
    echo "Testing binary functionality..."

    # Test version
    echo "  Testing --version..."
    if ./"${binary_name}" --version; then
        echo -e "   ✅ Version command works"
    else
        echo -e "   ${RED}❌ Version command failed${NC}"
        return 1
    fi

    # Test help
    echo "  Testing --help..."
    if ./"${binary_name}" --help > /dev/null; then
        echo -e "   ✅ Help command works"
    else
        echo -e "   ${RED}❌ Help command failed${NC}"
        return 1
    fi

    # Show binary info
    echo "  Binary information:"
    echo "   Size: $(du -h "${binary_name}" | cut -f1)"
    echo "   Type: $(file "${binary_name}")"

    echo -e "${GREEN} ✅ ${platform} binary test completed successfully${NC}"

    # Cleanup
    rm -f "${binary_name}" "${binary_name}.sha256"

    return 0
}

# Platforms to test
platforms=(
    "router-switch-${VERSION}-linux-x86_64:Linux x86_64"
    "router-switch-${VERSION}-linux-arm64:Linux ARM64"
    "router-switch-${VERSION}-darwin-x86_64:macOS Intel"
    "router-switch-${VERSION}-darwin-arm64:macOS Apple Silicon"
)

# Test each platform
success_count=0
total_count=${#platforms[@]}

for platform in "${platforms[@]}"; do
    IFS=':' read -r binary_name platform_name <<< "$platform"

    if test_binary "$binary_name" "$platform_name"; then
        ((success_count++))
    fi
done

# Summary
echo -e "\n${BLUE}📊 Test Summary${NC}"
echo "==============="
echo "Total platforms tested: ${total_count}"
echo -e "Successful tests: ${GREEN}${success_count}${NC}"
echo -e "Failed tests: ${RED}$((total_count - success_count))${NC}"

if [ $success_count -eq $total_count ]; then
    echo -e "\n${GREEN}🎉 All tests passed! Release ${VERSION} is ready for distribution.${NC}"
    exit 0
else
    echo -e "\n${RED}❌ Some tests failed. Please check the release artifacts.${NC}"
    exit 1
fi