# RouterSwitch

A CLI tool to switch between AI providers and models for Claude Code by directly setting environment variables.

## Overview

RouterSwitch allows you to easily switch between different AI providers (like DeepSeek, GLM, MiniMax) and their models by setting the appropriate environment variables. Unlike version 1.x, this version directly sets environment variables in the current process rather than outputting shell commands.

## Features

- 🚀 **Direct Environment Setting**: No more `eval` wrapper needed
- 🔀 **Provider Switching**: Seamlessly switch between AI providers
- 📝 **Configuration File**: JSON-based provider configuration
- ✅ **Validation**: Validates providers and models before setting
- 📢 **Clear Feedback**: Shows exactly which environment variables were set
- 🛡️ **Error Handling**: Comprehensive error checking and reporting

## Quick Start

### Installation

#### Option 1: Download Pre-built Binary (Recommended)

1. Go to the [Releases page](https://github.com/aokihu/claude-code-env-switcher/releases)
2. Download the appropriate binary for your platform:
   - **Linux x86-64**: `router-switch-v*-linux-x86_64`
   - **Linux ARM64**: `router-switch-v*-linux-arm64`
   - **macOS x86-64 (Intel)**: `router-switch-v*-darwin-x86_64`
   - **macOS ARM64 (Apple Silicon)**: `router-switch-v*-darwin-arm64`
3. Verify the download with the provided checksum
4. Make it executable and install:

```bash
# Example for Linux x86-64
chmod +x router-switch-v*-linux-x86_64
sudo mv router-switch-v*-linux-x86_64 /usr/local/bin/router-switch
```

#### Option 2: Build from Source

```bash
git clone https://github.com/aokihu/claude-code-env-switcher.git
cd RouterSwitch

# Build release version (recommended)
make release

# Or build both debug and release versions
make build-all

# Install release version system-wide (optional)
sudo make install-release
```

#### Cross-Platform Building

```bash
# Build for specific platforms
make linux-x86_64    # Linux x86_64
make darwin-x86_64   # macOS Intel
make darwin-arm64    # macOS Apple Silicon

# Package with checksums
make package-with-checksum PACKAGE_NAME=router-switch-custom
```

### Basic Usage

```bash
# Silent mode (default) - no output on success
router-switch --provider deepseek --model deepseek-chat

# Verbose mode - show detailed environment variable changes
router-switch --provider deepseek --model deepseek-chat --verbose

# Use short flags
router-switch -p glm -v

# Show version information
router-switch --version

# Use custom configuration file
router-switch --config /path/to/custom-config.json --provider deepseek

# Show help
router-switch --help
```

### What Happens When You Run It

**Verbose Mode Output:**
```bash
$ router-switch --provider deepseek --model deepseek-chat --verbose
Successfully set environment variables:
  Set ANTHROPIC_BASE_URL to https://api.deepseek.com/anthropic
  Set ANTHROPIC_AUTH_TOKEN to sk-xxx-xxxx-xxxx-xxxx-xxxx
  Set ANTHROPIC_MODEL to deepseek-chat
  Set ANTHROPIC_DEFAULT_SONNET_MODEL to deepseek-chat
  Set ANTHROPIC_DEFAULT_OPUS_MODEL to deepseek-reasoner
  Set ANTHROPIC_DEFAULT_HAIKU_MODEL to deepseek-chat
  Set ROUTERSWITCH_CURRENT_PROVIDER to deepseek
```

**Silent Mode (Default):**
```bash
$ router-switch --provider deepseek --model deepseek-chat
# No output on success, exit code 0
```

## Configuration

Create a `config.json` file in the same directory as the binary:

```json
{
  "providers": {
    "deepseek": {
      "description": "Deepseek V3.2",
      "base_url": "https://api.deepseek.com/anthropic",
      "api_key": "your-deepseek-api-key",
      "models": ["deepseek-chat", "deepseek-reasoner"],
      "env": {
        "ANTHROPIC_DEFAULT_SONNET_MODEL": "deepseek-chat",
        "ANTHROPIC_DEFAULT_OPUS_MODEL": "deepseek-reasoner",
        "ANTHROPIC_DEFAULT_HAIKU_MODEL": "deepseek-chat"
      }
    },
    "glm": {
      "description": "BigModel GLM 4.6",
      "base_url": "https://open.bigmodel.cn/api/anthropic",
      "api_key": "your-glm-api-key",
      "models": [],
      "env": {
        "API_TIMEOUT_MS": "3000000",
        "CLAUDE_CODE_DISABLE_NONESSENTIAL_TRAFFIC": "1"
      }
    }
  }
}
```

### Configuration Fields

- `providers`: Object containing all available providers
- `[provider-name]`: Key used with `🔒` flag
- `description`: Human-readable description
- `base_url`: API endpoint URL
- `api_key`: Authentication key (⚠️ Keep secure!)
- `models`: Array of available models (empty = use Claude Code defaults)
- `env`: Additional environment variables to set

### 🔒 Security Notes

- Never commit real API keys to version control
- Use environment variables or secure configuration management
- Replace `"your-api-key"` with your actual keys in the config file
- The example shows obscured keys for demonstration purposes only

## Environment Variables Set

RouterSwitch automatically sets these environment variables:

- `ANTHROPIC_BASE_URL`: API endpoint URL
- `ANTHROPIC_AUTH_TOKEN`: API authentication token
- `ANTHROPIC_MODEL`: Selected model (if models are configured)
- `ROUTERSWITCH_CURRENT_PROVIDER`: Current provider tracking
- Custom environment variables from provider configuration

## Important Notes

### Environment Inheritance
- Environment variables are set in the RouterSwitch process
- **Child processes** inherit these environment variables
- **Parent shell** environment is NOT modified (Unix limitation)

### Usage Scenarios

```bash
# For development - child processes inherit the environment
router-switch --provider deepseek
./my-ai-app.py  # This app inherits the environment

# For shell integration - use in scripts
#!/bin/bash
router-switch --provider glm
npm test  # Tests run with GLM environment

# For CI/CD pipelines
router-switch --provider deepseek --model deepseek-chat
make test  # Tests inherit the environment
```

## Migration from v1.x

If you're upgrading from version 1.x, see [MIGRATION.md](MIGRATION.md) for detailed migration instructions.

**Quick change:**
```bash
# Old (v1.x)
eval $(router-switch --provider deepseek)

# New (v2.x)
router-switch --provider deepseek
```

## Command Line Options

```
Usage: router-switch [options]

Options:
  -p, --provider <provider>  Specify AI provider from config.json
  -m, --model <model>        Specify AI model for the provider
  -c, --config <path>        Specify custom configuration file path
  -v, --verbose              Show detailed output (default: silent)
  -V, --version              Display version information
  -h, --help                 Display this help message
```

## Building

### Prerequisites

- GCC compiler (or Clang on macOS)
- Make
- Standard C library

### Build Commands

```bash
# Build release version (optimized)
make release

# Build debug version (with symbols)
make debug

# Build both versions
make build-all

# Development workflow (clean, debug, test)
make dev

# Production workflow (clean, release, test)
make prod

# Install release version system-wide
sudo make install-release

# Run tests
make test

# Compare debug vs release sizes
make compare

# Clean all build artifacts
make clean
```

### Build Types

- **Release**: Optimized binary with full compiler optimizations (`-O3`, `-flto`, `-march=native`)
- **Debug**: Development build with debug symbols and no optimization

### Binary Locations

- Debug version: `bin/debug/router-switch`
- Release version: `bin/release/router-switch`

## Automated Builds

This project uses GitHub Actions for automated building and releases:

### 🚀 Continuous Integration
- **Triggers**: Push to `main` branch or Git tags
- **Platforms**: Linux (x86-64, ARM64) and macOS (Intel, Apple Silicon)
- **Build Types**: Optimized release binaries with full validation
- **Artifacts**: Pre-compiled binaries with SHA256 checksums

### 📦 Release Process
- **Pre-releases**: Automatically created on every push to `main`
- **Official releases**: Created when Git tags are pushed
- **Assets**: All platform binaries and checksum files
- **Verification**: Each binary is tested before release

### 🔧 Build Matrix
| Runner | Architecture | Target | Compiler |
|--------|-------------|---------|----------|
| ubuntu-latest | x86-64 | linux-x86_64 | gcc |
| ubuntu-latest | ARM64 | linux-arm64 | aarch64-linux-gnu-gcc |
| macos-latest | x86-64 | darwin-x86_64 | clang |
| macos-latest | ARM64 | darwin-arm64 | clang |

## Troubleshooting

### "Provider not found" Error
- Check that the provider name matches exactly in `config.json`
- Verify the configuration file is valid JSON
- Use `--config` flag if your config file is elsewhere

### Environment Variables Not Visible
- This is normal - parent shell environment is not modified
- Child processes started after RouterSwitch will inherit the environment
- See MIGRATION.md for shell integration options

### Build Failures
- Ensure GCC is installed: `gcc --version`
- Check that all required source files are present
- Try `make clean && make all`

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## License

MIT License - see LICENSE file for details.

## Support

For issues and questions:
1. Check this README and MIGRATION.md
2. Review the help text: `router-switch --help`
3. Open an issue on the project repository