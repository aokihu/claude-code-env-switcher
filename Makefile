# RouterSwitch Makefile
# Compiles RouterSwitch CLI tool from C source

CC = gcc
TARGET = router-switch
SRCDIR = src
BINDIR = bin
SOURCES = $(wildcard $(SRCDIR)/*.c)

# Default configuration
BUILD_TYPE ?= release

# Cross-compilation support (override from environment)
TARGET_OS ?= $(shell uname -s | tr '[:upper:]' '[:lower:]')
TARGET_ARCH ?= $(shell uname -m | sed 's/x86_64/x86_64/')

# Build configurations
ifeq ($(BUILD_TYPE),debug)
    OBJDIR = obj/debug
    BINDIR_TARGET = $(BINDIR)/debug
    BASE_CFLAGS = -Wall -Wextra -std=c99 -g -DDEBUG -O0
    STRIP_CMD = @echo "Debug build - keeping symbols"
else ifeq ($(BUILD_TYPE),release)
    OBJDIR = obj/release
    BINDIR_TARGET = $(BINDIR)/release
    BASE_CFLAGS = -Wall -Wextra -std=c99 -O3 -DNDEBUG -flto
    STRIP_CMD = strip $(BINDIR_TARGET)/$(TARGET)
else
    $(error Invalid BUILD_TYPE. Use 'debug' or 'release')
endif

# Set architecture-specific flags
ifeq ($(TARGET_ARCH),x86_64)
    ifeq ($(TARGET_OS),linux)
        ARCH_FLAGS = -march=x86-64 -mtune=generic
    else ifeq ($(TARGET_OS),darwin)
        ARCH_FLAGS = -target x86_64-apple-macos10.15
    endif
else ifeq ($(TARGET_ARCH),arm64)
    ifeq ($(TARGET_OS),linux)
        ARCH_FLAGS =
    else ifeq ($(TARGET_OS),darwin)
        ARCH_FLAGS = -target arm64-apple-macos11
    endif
endif

# Final CFLAGS
CFLAGS ?= $(BASE_CFLAGS) $(ARCH_FLAGS)

OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
PREFIX = /usr/local

# Default target
all: $(BINDIR_TARGET)/$(TARGET)

# Create directories
$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR_TARGET):
	@mkdir -p $(BINDIR_TARGET)

# Create target binary
$(BINDIR_TARGET)/$(TARGET): $(OBJECTS) | $(BINDIR_TARGET)
	@echo "Linking $(TARGET) ($(BUILD_TYPE))..."
	$(CC) $(OBJECTS) -o $@
	@echo "Stripping debug symbols..."
	$(STRIP_CMD)
	@echo "Built $(TARGET) ($(BUILD_TYPE)) successfully!"
	@echo "Binary location: $@"

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@echo "Compiling $< ($(BUILD_TYPE))..."
	$(CC) $(CFLAGS) -c $< -o $@

# Build debug version
debug:
	$(MAKE) BUILD_TYPE=debug

# Build release version
release:
	$(MAKE) BUILD_TYPE=release

# Build both versions
build-all: debug release
	@echo "Built both debug and release versions!"

# Clean build artifacts
clean:
	rm -rf obj bin
	@echo "Cleaned all build artifacts"

# Clean specific build type
clean-debug:
	rm -rf obj/debug $(BINDIR)/debug
	@echo "Cleaned debug build artifacts"

clean-release:
	rm -rf obj/release $(BINDIR)/release
	@echo "Cleaned release build artifacts"

# Install binary to system
install: $(BINDIR_TARGET)/$(TARGET)
	install -d $(PREFIX)/bin
	install $(BINDIR_TARGET)/$(TARGET) $(PREFIX)/bin/
	@echo "Installed $(TARGET) ($(BUILD_TYPE)) to $(PREFIX)/bin/"

# Install release version (default)
install-release: release
	$(MAKE) install BUILD_TYPE=release

# Install debug version
install-debug: debug
	$(MAKE) install BUILD_TYPE=debug

# Uninstall from system
uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)
	@echo "Uninstalled $(TARGET) from $(PREFIX)/bin/"

# Run tests
test: $(BINDIR_TARGET)/$(TARGET)
	@echo "Running tests ($(BUILD_TYPE))..."
	bash test/test.sh

# Run tests on both versions
test-all: debug release
	@echo "Running tests on debug version..."
	$(BINDIR)/debug/$(TARGET) --help > /dev/null
	@echo "Debug version: PASSED"
	@echo "Running tests on release version..."
	$(BINDIR)/release/$(TARGET) --help > /dev/null
	@echo "Release version: PASSED"
	@echo "All tests passed!"

# Cross-compilation targets
linux-x86_64:
	$(MAKE) release CC=gcc TARGET_OS=linux TARGET_ARCH=x86_64
	@echo "Built Linux x86_64 release"

linux-arm64:
	$(MAKE) release CC=aarch64-linux-gnu-gcc TARGET_OS=linux TARGET_ARCH=arm64 \
		CFLAGS="$(BASE_CFLAGS) -static"
	@echo "Built Linux ARM64 release"

darwin-x86_64:
	$(MAKE) release CC=clang TARGET_OS=darwin TARGET_ARCH=x86_64
	@echo "Built macOS x86_64 release"

darwin-arm64:
	$(MAKE) release CC=clang TARGET_OS=darwin TARGET_ARCH=arm64
	@echo "Built macOS ARM64 release"

# Legacy platform targets (for backward compatibility)
linux-release:
	$(MAKE) linux-x86_64
	@echo "Built release for Linux"

macos-release:
	$(MAKE) darwin-x86_64
	@echo "Built release for macOS"

# Static build for distribution
static-release: CFLAGS += -static
static-release: release
	@echo "Built static release binary"

# Packaging and validation targets
PACKAGE_NAME ?= $(TARGET)

package: $(BINDIR_TARGET)/$(TARGET)
	@mkdir -p dist
	@cp $(BINDIR_TARGET)/$(TARGET) dist/$(PACKAGE_NAME)
	@chmod +x dist/$(PACKAGE_NAME)
	@echo "Packaged: dist/$(PACKAGE_NAME)"

package-with-checksum: package
	@cd dist && sha256sum $(PACKAGE_NAME) > $(PACKAGE_NAME).sha256
	@echo "Checksum created: dist/$(PACKAGE_NAME).sha256"

validate-binary: $(BINDIR_TARGET)/$(TARGET)
	@echo "=== Validating binary ==="
	@echo "File: $(BINDIR_TARGET)/$(TARGET)"
	@echo "Size: $$(du -h $(BINDIR_TARGET)/$(TARGET) | cut -f1)"
	@file $(BINDIR_TARGET)/$(TARGET)
	@echo "Testing functionality..."
	@$(BINDIR_TARGET)/$(TARGET) --version
	@$(BINDIR_TARGET)/$(TARGET) --help > /dev/null
	@echo "✅ Binary validation passed!"

# Build all platforms
build-all-platforms:
	@echo "Building all platform binaries..."
	$(MAKE) clean
	$(MAKE) linux-x86_64 PACKAGE_NAME=router-switch-linux-x86_64 package-with-checksum
	$(MAKE) clean
	$(MAKE) darwin-x86_64 PACKAGE_NAME=router-switch-darwin-x86_64 package-with-checksum
	@echo "✅ All platform builds completed!"
	@ls -la dist/

# Show binary information
info: $(BINDIR_TARGET)/$(TARGET)
	@echo "=== Binary Information ==="
	@echo "File: $(BINDIR_TARGET)/$(TARGET)"
	@echo "Build type: $(BUILD_TYPE)"
	@echo "Compiler: $(CC)"
	@echo "Flags: $(CFLAGS)"
	@echo "Size: $$(du -h $(BINDIR_TARGET)/$(TARGET) | cut -f1)"
	@file $(BINDIR_TARGET)/$(TARGET)

# Compare build sizes
compare: debug release
	@echo "=== Build Size Comparison ==="
	@echo "Debug:  $$(du -h $(BINDIR)/debug/$(TARGET) | cut -f1)"
	@echo "Release: $$(du -h $(BINDIR)/release/$(TARGET) | cut -f1)"
	@echo "Size reduction: $$(echo "scale=1; ($$(du -k $(BINDIR)/debug/$(TARGET) | cut -f1) - $$(du -k $(BINDIR)/release/$(TARGET) | cut -f1)) * 100 / $$(du -k $(BINDIR)/debug/$(TARGET) | cut -f1)" | bc)%"

# Development workflow
dev: clean debug test
	@echo "Development build complete!"

# Production workflow
prod: clean release test-all
	@echo "Production build complete!"

# Show help
help:
	@echo "RouterSwitch Build System"
	@echo "======================="
	@echo ""
	@echo "Build Targets:"
	@echo "  all           - Build $(TARGET) (default: release)"
	@echo "  debug         - Build debug version with symbols"
	@echo "  release       - Build optimized release version"
	@echo "  build-all     - Build both debug and release versions"
	@echo ""
	@echo "Clean Targets:"
	@echo "  clean         - Remove all build artifacts"
	@echo "  clean-debug   - Remove debug build artifacts"
	@echo "  clean-release - Remove release build artifacts"
	@echo ""
	@echo "Install Targets:"
	@echo "  install       - Install current build to system"
	@echo "  install-debug - Install debug version to system"
	@echo "  install-release- Install release version to system"
	@echo "  uninstall     - Remove from system"
	@echo ""
	@echo "Test Targets:"
	@echo "  test          - Run tests on current build"
	@echo "  test-all      - Run tests on both versions"
	@echo ""
	@echo "Cross-Platform Targets:"
	@echo "  linux-x86_64  - Build for Linux x86_64"
	@echo "  linux-arm64   - Build for Linux ARM64 (cross-compile)"
	@echo "  darwin-x86_64 - Build for macOS x86_64 (Intel)"
	@echo "  darwin-arm64  - Build for macOS ARM64 (Apple Silicon)"
	@echo ""
	@echo "Packaging Targets:"
	@echo "  package       - Package binary with custom name"
	@echo "  package-with-checksum - Package binary with SHA256 checksum"
	@echo "  validate-binary- Validate binary functionality"
	@echo "  build-all-platforms - Build for all supported platforms"
	@echo ""
	@echo "Advanced Targets:"
	@echo "  static-release- Build static binary for distribution"
	@echo "  info          - Show binary information"
	@echo "  compare       - Compare debug vs release sizes"
	@echo "  dev           - Clean, debug build, and test"
	@echo "  prod          - Clean, release build, and test all"
	@echo ""
	@echo "Examples:"
	@echo "  make debug              # Build debug version"
	@echo "  make release            # Build optimized release"
	@echo "  make install-release    # Install release to system"
	@echo "  make compare            # Compare binary sizes"
	@echo ""
	@echo "Binary Locations:"
	@echo "  Debug:  $(BINDIR)/debug/$(TARGET)"
	@echo "  Release: $(BINDIR)/release/$(TARGET)"

.PHONY: all debug release build-all clean clean-debug clean-release install install-release install-debug uninstall test test-all linux-x86_64 linux-arm64 darwin-x86_64 darwin-arm64 linux-release macos-release static-release package package-with-checksum validate-binary build-all-platforms info compare dev prod help