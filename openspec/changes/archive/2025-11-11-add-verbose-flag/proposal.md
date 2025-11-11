# Change: Add Verbose Output Mode and Update Version to 2.0.1

## Why
The current RouterSwitch implementation always outputs detailed success messages for every environment variable operation, which can be verbose for automated scripts and CI/CD pipelines. Users need a way to have silent operation by default with optional verbose output for debugging purposes. Additionally, updating to version 2.0.1 reflects the completion of the environment variable setting changes and this usability improvement.

## What Changes
- **MODIFIED** Default behavior to silent operation (no output on success)
- **ADDED** New `--verbose` or `-V` flag for detailed output mode
- **UPDATED** Version number from 1.0.0 to 2.0.1
- **MODIFIED** Help and version information to reflect new behavior
- **PRESERVED** All error messages and failure output (always shown)

## Impact
- Affected specs: cli-interface
- Affected code: src/cli.c, src/main.c, src/env_commands.c
- User workflow changes: Default operation is now silent, use `-V` for verbose output
- Script compatibility: Existing scripts will have cleaner output by default
- CI/CD compatibility: Reduced noise in automated pipelines

## Migration Notes
- Default behavior becomes silent (breaking change for users expecting output)
- Add `-V` flag to scripts that need verbose output
- Error handling remains unchanged - errors always displayed