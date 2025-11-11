## REMOVED Requirements
### Requirement: Shell Command Output Generation
**Reason**: The tool will no longer generate shell commands for environment variable setting, as this creates user friction and requires eval wrapper usage.
**Migration**: Users should call router-switch directly instead of using `eval $(router-switch ...)`

## MODIFIED Requirements
### Requirement: Command-line Interface
The CLI SHALL provide command-line argument parsing for AI provider and model configuration using C language implementation.

#### Scenario: User specifies provider and model
- **WHEN** user runs `router-switch --provider openai --model gpt-4`
- **THEN** the tool directly sets environment variables in the current process
- **AND** outputs a success message to stdout indicating which variables were set
- **AND** the tool exits with status code 0

#### Scenario: User uses short flags
- **WHEN** user runs `router-switch -p anthropic -m claude-3`
- **THEN** the tool directly sets environment variables in the current process using short flags
- **AND** outputs a success message to stdout indicating which variables were set
- **AND** the tool exits with status code 0

#### Scenario: User switches providers
- **WHEN** user switches from one provider to another
- **THEN** the tool unsets old provider environment variables directly
- **AND** sets new provider environment variables directly
- **AND** outputs a success message showing both cleared and set variables
- **AND** the tool exits with status code 0

#### Scenario: User specifies custom configuration file
- **WHEN** user runs `router-switch --config /path/to/custom-config.json --provider openai`
- **THEN** the tool loads configuration from the specified file
- **AND** directly sets appropriate environment variables
- **AND** outputs a success message to stdout

#### Scenario: User uses short flag for configuration
- **WHEN** user runs `router-switch -c ./my-config.json -p glm`
- **THEN** the tool loads configuration from the specified file
- **AND** directly sets appropriate environment variables
- **AND** outputs a success message to stdout

#### Scenario: User requests help
- **WHEN** user runs `router-switch --help`
- **THEN** the tool displays usage information including provider, model, and config options
- **AND** explains that environment variables are set directly

## ADDED Requirements
### Requirement: Direct Environment Variable Setting
The CLI SHALL directly set environment variables in the current process for all operations.

#### Scenario: Environment variable setting succeeds
- **WHEN** environment variables are successfully set
- **THEN** the tool outputs a success message like "Set ANTHROPIC_BASE_URL to https://api.openai.com"
- **AND** lists all environment variables that were modified
- **AND** exits with status code 0

#### Scenario: Environment variable setting fails
- **WHEN** setting an environment variable fails due to system limitations
- **THEN** the tool outputs an error message to stderr
- **AND** exits with a non-zero status code
- **AND** provides information about the failure reason

#### Scenario: Provider switching with tracking
- **WHEN** user switches providers
- **THEN** the tool updates the `ROUTERSWITCH_CURRENT_PROVIDER` environment variable
- **AND** clears all environment variables from the previous provider
- **AND** sets all environment variables for the new provider including custom env vars

#### Scenario: Clearing previous provider environment
- **WHEN** switching from a provider with existing environment variables
- **THEN** the tool unsets ANTHROPIC_BASE_URL, ANTHROPIC_AUTH_TOKEN, ANTHROPIC_MODEL
- **AND** unsets all custom environment variables from the previous provider
- **AND** unsets ROUTERSWITCH_CURRENT_PROVIDER
- **AND** reports which variables were cleared

### Requirement: Environment Variable Validation
The CLI SHALL validate environment variable operations before attempting them.

#### Scenario: Validate provider before setting variables
- **WHEN** user specifies an invalid provider
- **THEN** the tool validates the provider exists in configuration
- **AND** displays an error message without modifying any environment variables
- **AND** exits with non-zero status code

#### Scenario: Validate model before setting variables
- **WHEN** user specifies an invalid model for a provider
- **THEN** the tool validates the model exists for the specified provider
- **AND** displays an error message without modifying any environment variables
- **AND** exits with non-zero status code

#### Scenario: Validate configuration file before setting variables
- **WHEN** user specifies a non-existent or invalid configuration file
- **THEN** the tool validates the file exists and has valid JSON format
- **AND** displays an error message without modifying any environment variables
- **AND** exits with non-zero status code