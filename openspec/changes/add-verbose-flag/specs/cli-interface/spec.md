## MODIFIED Requirements
### Requirement: Command-line Interface
The CLI SHALL provide command-line argument parsing for AI provider and model configuration using C language implementation.

#### Scenario: User runs tool with default silent behavior
- **WHEN** user runs `router-switch --provider openai --model gpt-4` without verbose flag
- **THEN** the tool directly sets environment variables in the current process
- **AND** produces no output on stdout on success
- **AND** exits with status code 0

#### Scenario: User runs tool with verbose flag
- **WHEN** user runs `router-switch --provider openai --model gpt-4 --verbose`
- **THEN** the tool directly sets environment variables in the current process
- **AND** outputs a success message to stdout indicating which variables were set
- **AND** exits with status code 0

#### Scenario: User uses short verbose flag
- **WHEN** user runs `router-switch -p anthropic -m claude-3 -V`
- **THEN** the tool directly sets environment variables in the current process using short flags
- **AND** outputs a success message to stdout indicating which variables were set
- **AND** exits with status code 0

#### Scenario: Error occurs regardless of verbose mode
- **WHEN** an error occurs during environment variable setting (invalid provider, file not found, etc.)
- **THEN** the tool displays an error message to stderr regardless of verbose flag
- **AND** exits with non-zero status code

#### Scenario: User requests help
- **WHEN** user runs `router-switch --help`
- **THEN** the tool displays usage information including the new `--verbose` option
- **AND** explains the default silent behavior and optional verbose output

#### Scenario: User requests version
- **WHEN** user runs `router-switch --version`
- **THEN** the tool displays version information showing v2.0.1

## ADDED Requirements
### Requirement: Verbose Output Control
The CLI SHALL support optional verbose output mode while maintaining silent default behavior.

#### Scenario: Silent default behavior
- **WHEN** user runs router-switch without `--verbose` flag
- **THEN** the tool performs all operations silently on success
- **AND** only outputs error messages when failures occur
- **AND** provides no feedback on successful environment variable changes

#### Scenario: Verbose mode enabled
- **WHEN** user specifies the `--verbose` or `-V` flag
- **THEN** the tool outputs detailed success messages showing which environment variables were set/cleared
- **AND** includes the variable names and values in the output
- **AND** maintains error output for failed operations

#### Scenario: Error output consistency
- **WHEN** any operation fails (missing provider, invalid model, file errors, etc.)
- **THEN** the tool always displays error messages regardless of verbose flag setting
- **AND** outputs to stderr for consistency
- **AND** includes helpful information about the failure

#### Scenario: Provider switching in verbose mode
- **WHEN** user switches providers with `--verbose` flag
- **THEN** the tool outputs messages for both clearing and setting operations
- **AND** shows which variables were cleared from the previous provider
- **AND** shows which variables were set for the new provider

## MODIFIED Requirements
### Requirement: Version Information
The CLI SHALL display current version information when requested.

#### Scenario: User requests version
- **WHEN** user runs `router-switch --version`
- **THEN** the tool displays version as "router-switch v2.0.1"