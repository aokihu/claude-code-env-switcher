# Shell Output Default Specification

## Purpose
Modify router-switch to output shell export commands by default instead of directly modifying environment variables, allowing users to set environment variables in their parent shell using eval or source.

## MODIFIED Requirements

### Requirement: Default Output Behavior
The CLI SHALL output shell export commands by default.

#### Scenario: User runs router-switch normally
- **WHEN** user runs `router-switch -p deepseek -m deepseek-chat`
- **THEN** the tool outputs shell export commands to stdout
- **AND** does not modify environment variables directly

#### Scenario: User switches providers
- **WHEN** user runs `router-switch -p glm`
- **THEN** the tool outputs unset commands for old provider followed by export commands for new provider to stdout
- **AND** the combined output can be directly used with eval

### Requirement: Export Command Format
The tool SHALL output properly formatted shell export commands.

#### Scenario: Exporting environment variables
- **WHEN** exporting provider configuration
- **THEN** the tool outputs `export KEY='VALUE'` format
- **AND** properly escapes single quotes in values

#### Scenario: Unsetting environment variables
- **WHEN** switching providers
- **THEN** the tool outputs `unset KEY` commands for old provider variables
- **AND** only outputs unset commands for variables that were previously set

### Requirement: Compatibility with eval
The exported commands SHALL be compatible with shell eval execution.

#### Scenario: User evals router-switch output
- **WHEN** user runs `eval $(router-switch --export -p provider)`
- **THEN** all commands execute successfully in the shell
- **AND** environment variables are set in the parent shell

#### Scenario: User sources router-switch output
- **WHEN** user runs `source <(router-switch --export -p provider)`
- **THEN** all commands execute successfully
- **AND** environment variables persist in the shell session

### Requirement: Output Cleanliness
The export mode SHALL output only valid shell commands to stdout.

#### Scenario: Successful export operation
- **WHEN** all operations succeed
- **THEN** stdout contains only export/unset commands
- **AND** contains no additional text or formatting
- **AND** stderr contains verbose messages if requested

#### Scenario: Shell escaping
- **WHEN** environment variable values contain special characters
- **THEN** values are properly escaped for shell execution
- **AND** quotes are handled correctly