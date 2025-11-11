# CLI Interface Modifications for Shell Output Default

## Purpose
Modify existing CLI interface to output shell commands by default instead of modifying environment variables.

## MODIFIED Requirements

### Requirement: Command-line Interface
The CLI SHALL provide command-line argument parsing for AI provider and model configuration with shell command output as default.

#### Scenario: User specifies provider and model
- **WHEN** user runs `router-switch --provider openai --model gpt-4`
- **THEN** the tool outputs only shell export commands to stdout
- **AND** the output can be directly used with eval
- **AND** does not modify environment variables directly

#### Scenario: User uses short flags
- **WHEN** user runs `router-switch -p anthropic -m claude-3`
- **THEN** the tool outputs only shell export commands to stdout using short flags
- **AND** the output can be directly used with eval

#### Scenario: User specifies custom configuration file
- **WHEN** user runs `router-switch --config /path/to/custom-config.json --provider openai`
- **THEN** the tool loads configuration from the specified file
- **AND** outputs appropriate shell commands to stdout
- **AND** does not modify environment variables directly

#### Scenario: User switches providers
- **WHEN** user switches from one provider to another
- **THEN** the tool outputs unset commands for old provider followed by export commands for new provider to stdout
- **AND** the combined output can be directly used with eval
- **AND** does not modify environment variables directly

#### Scenario: User requests help
- **WHEN** user runs `router-switch --help`
- **THEN** the tool displays usage information including provider, model, and config options
- **AND** shows examples of using eval to set environment variables

### Requirement: Error Handling
The CLI SHALL handle errors consistently and output errors to stderr.

#### Scenario: Invalid provider specified
- **WHEN** user specifies an invalid provider
- **THEN** the tool displays an error message to stderr
- **AND** exits with non-zero status code
- **AND** outputs no export commands to stdout

#### Scenario: Invalid model specified
- **WHEN** user specifies an invalid model for the selected provider
- **THEN** the tool displays an error message to stderr
- **AND** exits with non-zero status code
- **AND** outputs no export commands to stdout

#### Scenario: Invalid configuration file
- **WHEN** user specifies a non-existent configuration file
- **THEN** the tool displays an error message to stderr
- **AND** exits with non-zero status code
- **AND** outputs no export commands to stdout

### Requirement: Output Stream Separation
The CLI SHALL maintain proper separation of output streams.

#### Scenario: Verbose output
- **WHEN** user runs `router-switch --verbose -p provider`
- **THEN** the tool outputs export commands to stdout
- **AND** outputs verbose messages to stderr
- **AND** the streams remain completely separate