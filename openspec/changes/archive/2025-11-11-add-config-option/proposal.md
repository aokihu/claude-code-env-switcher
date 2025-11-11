# Change: Add --config option to specify configuration file

## Why
Currently the RouterSwitch CLI tool only supports loading configuration from a hardcoded `config.json` file in the current directory. This change will allow users to specify custom configuration file paths, enabling better organization and multiple configuration scenarios.

## What Changes
- Add `--config` (`-c`) option to specify custom configuration file path
- Maintain backward compatibility with default `config.json` behavior
- Update configuration loading logic to use specified file path
- Update help documentation to include new option

## Impact
- Affected specs: `cli-interface` (modified)
- Affected code: `src/parse_argv.ts`, `src/clear_env.ts`, `src/set_env.ts`
- Dependencies: None (uses existing Node.js file system APIs)