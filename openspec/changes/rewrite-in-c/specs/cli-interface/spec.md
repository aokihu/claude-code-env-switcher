## MODIFIED Requirements
### Requirement: Command-line Interface
The CLI SHALL provide command-line argument parsing for AI provider and model configuration using C language implementation.

#### Scenario: User specifies provider and model
- **WHEN** user runs `router-switch --provider openai --model gpt-4`
- **THEN** the tool outputs only shell export commands to stdout
- **AND** the output can be directly used with eval

#### Scenario: User uses short flags
- **WHEN** user runs `router-switch -p anthropic -m claude-3`
- **THEN** the tool outputs only shell export commands to stdout using short flags
- **AND** the output can be directly used with eval

#### Scenario: User specifies custom configuration file
- **WHEN** user runs `router-switch --config /path/to/custom-config.json --provider openai`
- **THEN** the tool loads configuration from the specified file
- **AND** outputs appropriate shell commands to stdout

#### Scenario: User uses short flag for configuration
- **WHEN** user runs `router-switch -c ./my-config.json -p glm`
- **THEN** the tool loads configuration from the specified file
- **AND** outputs appropriate shell commands to stdout

#### Scenario: User switches providers
- **WHEN** user switches from one provider to another
- **THEN** the tool outputs unset commands for old provider followed by export commands for new provider to stdout
- **AND** the combined output can be directly used with eval

#### Scenario: User requests help
- **WHEN** user runs `router-switch --help`
- **THEN** the tool displays usage information including provider, model, and config options

### Requirement: Provider Configuration
The CLI SHALL support provider selection from available configuration options using C language implementation.

#### Scenario: Valid provider specified
- **WHEN** user specifies a valid provider (e.g., `--provider openai`)
- **THEN** the tool accepts the provider configuration
- **AND** outputs appropriate shell commands to stdout

#### Scenario: Invalid provider specified
- **WHEN** user specifies an invalid provider
- **THEN** the tool displays an error message to stderr
- **AND** exits with non-zero status code

### Requirement: Model Configuration
The CLI SHALL support model selection for the specified provider using C language implementation.

#### Scenario: Valid model specified
- **WHEN** user specifies a valid model for the selected provider
- **THEN** the tool accepts the model configuration
- **AND** outputs appropriate shell commands to stdout

#### Scenario: Invalid model specified
- **WHEN** user specifies an invalid model for the selected provider
- **THEN** the tool displays an error message to stderr
- **AND** exits with non-zero status code

### Requirement: Configuration File Support
The CLI SHALL support loading configuration from custom file paths using C language implementation.

#### Scenario: Default configuration file
- **WHEN** user does not specify a configuration file
- **THEN** the tool loads configuration from `config.json` in the current directory
- **AND** proceeds with normal operation

#### Scenario: Custom configuration file
- **WHEN** user specifies a custom configuration file path
- **THEN** the tool loads configuration from the specified file
- **AND** validates the file exists and has valid format

#### Scenario: Invalid configuration file
- **WHEN** user specifies a non-existent configuration file
- **THEN** the tool displays an error message to stderr
- **AND** exits with non-zero status code

#### Scenario: Malformed configuration file
- **WHEN** user specifies a configuration file with invalid JSON format
- **THEN** the tool displays an error message to stderr
- **AND** exits with non-zero status code