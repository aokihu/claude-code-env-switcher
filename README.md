# RouterSwitch

A CLI tool to switch between AI providers and models for Claude Code by outputting shell commands that can be executed to set environment variables.

## Overview

RouterSwitch allows you to easily switch between different AI providers (like DeepSeek, GLM, MiniMax) and their models by outputting shell export commands. Version 3.0 returns to the shell command output approach for better shell integration while providing an optional wrapper function for easier usage.

## Features

- 🐚 **Shell Command Output**: Outputs shell commands for environment variable setting
- 🚀 **Optional Shell Wrapper**: Generate wrapper function for easier usage
- 🔀 **Provider Switching**: Seamlessly switch between AI providers
- 📝 **Configuration File**: JSON-based provider configuration
- ✅ **Validation**: Validates providers and models before setting
- 🛡️ **Error Handling**: Comprehensive error checking and reporting
- 🔄 **Clean Switching**: Automatically unsets old provider variables

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

### Basic Usage

#### Method 1: Using eval (Direct)

```bash
# Switch to provider with eval
eval $(router-switch --provider deepseek --model deepseek-chat)

# Use short flags
eval $(router-switch -p glm)

# Use custom configuration file
eval $(router-switch --config /path/to/custom-config.json --provider deepseek)
```

#### Method 2: Using source (Recommended for complex values)

```bash
# Switch using source (safer for special characters)
source <(router-switch --provider deepseek --model deepseek-chat)

# Or with process substitution
source <(router-switch -p glm)
```

#### Method 3: Install Shell Wrapper (Easiest)

```bash
# Generate and install shell wrapper
router-switch --install >> ~/.zshrc && source ~/.zshrc

# For bash
router-switch --install >> ~/.bashrc && source ~/.bashrc

# Now you can use simple syntax:
router-switch deepseek
router-switch deepseek deepseek-chat
router-switch -v glm
```

### What Happens When You Run It

**Command Output:**
```bash
$ router-switch --provider deepseek --model deepseek-chat
export ANTHROPIC_BASE_URL='https://api.deepseek.com/anthropic'
export ANTHROPIC_AUTH_TOKEN='sk-your-api-key'
export ANTHROPIC_MODEL='deepseek-chat'
export ANTHROPIC_DEFAULT_SONNET_MODEL='deepseek-chat'
export ANTHROPIC_DEFAULT_OPUS_MODEL='deepseek-reasoner'
export ANTHROPIC_DEFAULT_HAIKU_MODEL='deepseek-chat'
export ROUTERSWITCH_CURRENT_PROVIDER='deepseek'
```

**With Shell Wrapper Installed:**
```bash
$ router-switch deepseek
# No output, but environment variables are set in current shell

$ router-switch -v deepseek deepseek-chat
# Verbose output to stderr
```

## Installation Guide

### Installing the Shell Wrapper

The shell wrapper provides the easiest usage experience. Here's how to install it:

1. **Generate the wrapper function:**
   ```bash
   router-switch --install > ~/.router-switch-wrapper.sh
   ```

2. **Add to your shell configuration:**
   ```bash
   # For zsh
   cat ~/.router-switch-wrapper.sh >> ~/.zshrc

   # For bash
   cat ~/.router-switch-wrapper.sh >> ~/.bashrc
   ```

3. **Reload your shell:**
   ```bash
   # For zsh
   source ~/.zshrc

   # For bash
   source ~/.bashrc
   ```

4. **Verify installation:**
   ```bash
   $ router-switch
   Usage: router-switch <provider> [model] [options]
     provider: AI provider name (e.g., deepseek, glm, minimax)
     model: Optional model name
     options: Additional options passed to router-switch

   Examples:
     router-switch deepseek
     router-switch deepseek deepseek-chat
     router-switch -v glm
   ```

### Wrapper Function Features

- **Simplified syntax**: `router-switch deepseek` instead of `eval $(router-switch -p deepseek)`
- **Tab completion**: Auto-completion for provider names
- **Cross-shell**: Works with both bash and zsh
- **Argument flexibility**: Supports both simple and flag-based syntax
- **Pass-through**: All original flags still work

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
- `[provider-name]`: Key used with `-p` flag
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

## Command Line Options

```
Usage: router-switch [options]

Options:
  -p, --provider <provider>  Specify AI provider from config.json
  -m, --model <model>        Specify AI model for the provider
  -c, --config <path>        Specify custom configuration file path
  -v, --verbose              Show detailed output to stderr
  -i, --install              Generate shell wrapper function for easy usage
  -V, --version              Display version information
  -h, --help                 Display this help message
```

## Environment Variables Set

RouterSwitch automatically sets these environment variables:

- `ANTHROPIC_BASE_URL`: API endpoint URL
- `ANTHROPIC_AUTH_TOKEN`: API authentication token
- `ANTHROPIC_MODEL`: Selected model (if models are configured)
- `ROUTERSWITCH_CURRENT_PROVIDER`: Current provider tracking
- Custom environment variables from provider configuration

## Usage Scenarios

```bash
# Quick switching with eval
eval $(router-switch --provider deepseek)
echo $ANTHROPIC_BASE_URL

# Using source (safer)
source <(router-switch --provider glm --model gpt-4)
echo $ANTHROPIC_BASE_URL

# With installed wrapper
router-switch deepseek deepseek-chat
echo $ANTHROPIC_BASE_URL

# In scripts
#!/bin/bash
eval $(router-switch -p deepseek)
./my-ai-app.py  # App inherits the environment

# For CI/CD pipelines
source <(router-switch --provider deepseek --model deepseek-chat)
make test
```

## Migration from v2.x

Version 3.0 changes back to shell command output approach. The migration is straightforward:

**If you were using v2.x:**
```bash
# Old (v2.x)
router-switch --provider deepseek

# New (v3.x) - need to eval the output
eval $(router-switch --provider deepseek)
```

**Or install the shell wrapper for the same experience:**
```bash
# One-time setup
router-switch --install >> ~/.zshrc && source ~/.zshrc

# Now use same syntax as v2.x
router-switch --provider deepseek
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

# Clean all build artifacts
make clean
```

### Build Types

- **Release**: Optimized binary with full compiler optimizations (`-O3`, `-flto`, `-march=native`)
- **Debug**: Development build with debug symbols and no optimization

### Binary Locations

- Debug version: `bin/debug/router-switch`
- Release version: `bin/release/router-switch`

## Troubleshooting

### "Provider not found" Error
- Check that the provider name matches exactly in `config.json`
- Verify the configuration file is valid JSON
- Use `--config` flag if your config file is elsewhere

### "eval $(command)" doesn't work
- Try using `source <(command)` instead
- Check for special characters in API keys or values
- Ensure the command output doesn't contain extra text

### Shell command contains unescaped characters
- The tool automatically escapes special characters
- If you encounter issues, use `source <(router-switch ...)` instead of `eval`

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
1. Check this README
2. Review the help text: `router-switch --help`
3. Open an issue on the project repository