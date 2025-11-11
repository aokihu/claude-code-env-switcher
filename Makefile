# RouterSwitch Makefile
# Compiles RouterSwitch CLI tool from C source

CC = gcc
TARGET = router-switch
SRCDIR = src
BINDIR = bin
SOURCES = $(wildcard $(SRCDIR)/*.c)

# Default configuration
BUILD_TYPE ?= release

# Build configurations
ifeq ($(BUILD_TYPE),debug)
    OBJDIR = obj/debug
    BINDIR_TARGET = $(BINDIR)/debug
    CFLAGS = -Wall -Wextra -std=c99 -g -DDEBUG -O0
    STRIP_CMD = @echo "Debug build - keeping symbols"
else ifeq ($(BUILD_TYPE),release)
    OBJDIR = obj/release
    BINDIR_TARGET = $(BINDIR)/release
    CFLAGS = -Wall -Wextra -std=c99 -O3 -DNDEBUG -flto -march=native -mtune=native
    STRIP_CMD = strip $(BINDIR_TARGET)/$(TARGET)
else
    $(error Invalid BUILD_TYPE. Use 'debug' or 'release')
endif

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

# Build for different platforms
linux-release:
	$(MAKE) release CC=gcc CFLAGS="$(CFLAGS) -DPLATFORM_LINUX"
	@echo "Built release for Linux"

macos-release:
	$(MAKE) release CC=clang CFLAGS="$(CFLAGS) -DPLATFORM_MACOS"
	@echo "Built release for macOS"

# Static build for distribution
static-release: CFLAGS += -static
static-release: release
	@echo "Built static release binary"

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

.PHONY: all debug release build-all clean clean-debug clean-release install install-release install-debug uninstall test test-all linux-release macos-release static-release info compare dev prod help