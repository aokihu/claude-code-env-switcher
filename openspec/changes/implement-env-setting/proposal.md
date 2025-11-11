# Change: Implement Direct Environment Variable Setting

## Why
The current RouterSwitch implementation outputs shell commands that users must execute with `eval` to set environment variables. This approach is indirect, requires users to understand shell evaluation, and doesn't actually modify the current process environment. Direct environment variable setting provides a more immediate and user-friendly experience, especially for scripting and automation scenarios.

## What Changes
- **BREAKING** Replace command output behavior with direct environment variable setting
- **REMOVED** Shell command generation functionality
- **MODIFIED** All operations to use `setenv()` and `unsetenv()` functions
- **MODIFIED** Change output format from shell commands to success/error messages
- **MODIFIED** Update error handling to work with direct environment manipulation
- **BREAKING** Change usage patterns from `eval $(router-switch ...)` to `router-switch ...`

## Impact
- Affected specs: cli-interface
- Affected code: src/main.c, src/env_commands.c, src/cli.c
- User workflow changes: Users run router-switch directly without eval wrapper
- Script compatibility: Existing scripts using `eval $(...)` must be updated to call router-switch directly