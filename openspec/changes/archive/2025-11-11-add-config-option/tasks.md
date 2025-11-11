## 1. Implementation
- [x] 1.1 Update parse_argv.ts to add --config (-c) option
- [x] 1.2 Modify CliOptions interface to include config property
- [x] 1.3 Update clear_env.ts loadConfig function to accept config path parameter
- [x] 1.4 Update set_env.ts to use config path from CLI options
- [x] 1.5 Update main index.ts to pass config path to environment functions
- [x] 1.6 Update help documentation to include --config option

## 2. Testing
- [x] 2.1 Test default behavior (no --config specified)
- [x] 2.2 Test custom config file with valid path
- [x] 2.3 Test error handling for non-existent config file
- [x] 2.4 Test error handling for malformed config file
- [x] 2.5 Test help command includes --config option

## 3. Validation
- [x] 3.1 Build the project successfully
- [x] 3.2 Test all configuration scenarios work correctly
- [x] 3.3 Verify backward compatibility with existing config.json