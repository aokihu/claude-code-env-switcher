# Change: Add CLI argument parsing functionality

## Why
Currently the RouterSwitch CLI tool lacks command-line argument parsing, making it impossible for users to specify AI providers and models through the command line. This change will enable users to configure their AI provider and model preferences directly via CLI arguments.

## What Changes
- Add `--provider` (`-p`) option to specify AI provider from configuration
- Add `--model` (`-m`) option to specify AI model
- Implement Commander.js integration for CLI argument parsing
- Create basic CLI structure with help documentation

## Impact
- Affected specs: `cli-interface` (new capability)
- Affected code: `src/index.ts` (main CLI entry point)
- Dependencies: Commander.js (already in package.json)