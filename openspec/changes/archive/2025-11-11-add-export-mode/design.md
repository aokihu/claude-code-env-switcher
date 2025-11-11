# Design: Export Mode Implementation

## Architecture Overview

The export mode will be implemented as a new command-line flag `--export` (short: `-e`) that changes the program's behavior from direct environment variable modification to outputting shell commands.

## Implementation Details

### 1. CLI Changes
- Add `--export` / `-e` flag to the command-line options
- Add `export_mode` boolean field to the `Options` struct
- Update help text to include the new option

### 2. Output Functions
Create new functions in `env_commands.c`:
- `export_environment_variable()` - outputs `export KEY=VALUE` commands
- `export_unset_environment_variable()` - outputs `unset KEY` commands
- `export_provider_environment()` - orchestrates exporting all provider variables
- `export_clear_provider_environment()` - orchestrates clearing operations

### 3. Flow Changes
When export mode is enabled:
- Skip all `setenv()`/`unsetenv()` calls
- Print shell commands directly to stdout
- Maintain the same validation and error handling
- Keep verbose output separate from export commands

### 4. Output Format
Export commands will follow this format:
```bash
# Clear old provider variables (if switching)
unset ANTHROPIC_BASE_URL
unset ANTHROPIC_AUTH_TOKEN

# Set new provider variables
export ANTHROPIC_BASE_URL='https://api.deepseek.com/anthropic'
export ANTHROPIC_AUTH_TOKEN='sk-api-key'
export ANTHROPIC_MODEL='deepseek-chat'
export ANTHROPIC_DEFAULT_SONNET_MODEL='deepseek-chat'
```

## Backward Compatibility
- Default behavior (direct env var setting) remains unchanged
- All existing flags and options continue to work as before
- Only when `--export` is specified does the behavior change

## Error Handling
- Validation errors still go to stderr
- Export commands only go to stdout
- Exit codes remain the same (0 for success, non-zero for errors)