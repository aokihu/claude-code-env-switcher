# Shell Wrapper Installation Specification

## Purpose
Add an `--install`/`-i` flag that generates a shell wrapper function for router-switch, allowing users to use the tool without typing `eval` each time.

## ADDED Requirements

### Requirement: Install Flag Support
The CLI SHALL support an `--install` flag that outputs a shell wrapper function.

#### Scenario: User requests installation
- **WHEN** user runs `router-switch --install`
- **THEN** the tool outputs a shell wrapper function to stdout
- **AND** the function automatically handles eval calls
- **AND** instructions are provided for adding to shell config

#### Scenario: User uses short install flag
- **WHEN** user runs `router-switch -i`
- **THEN** the tool outputs the same shell wrapper function
- **AND** uses the short flag syntax

### Requirement: Shell Wrapper Function
The generated wrapper function SHALL provide a simplified interface.

#### Scenario: User uses wrapper function
- **WHEN** user runs `router-switch deepseek`
- **THEN** the wrapper calls `eval $(router-switch -p deepseek)`
- **AND** environment variables are set in the current shell

#### Scenario: User specifies model with wrapper
- **WHEN** user runs `router-switch deepseek deepseek-chat`
- **THEN** the wrapper calls `eval $(router-switch -p deepseek -m deepseek-chat)`
- **AND** both provider and model are set

#### Scenario: User passes additional flags
- **WHEN** user runs `router-switch -v deepseek`
- **THEN** the wrapper passes verbose flag through
- **AND** calls `eval $(router-switch -v -p deepseek)`

### Requirement: Shell Compatibility
The wrapper function SHALL work with common shells.

#### Scenario: Bash shell
- **WHEN** the wrapper function is loaded in bash
- **THEN** it works correctly with bash syntax
- **AND** handles bash-specific features properly

#### Scenario: Zsh shell
- **WHEN** the wrapper function is loaded in zsh
- **THEN** it works correctly with zsh syntax
- **AND** handles zsh-specific features properly

### Requirement: Installation Instructions
The install output SHALL include clear instructions for users.

#### Scenario: User requests installation
- **WHEN** user runs `router-switch --install`
- **THEN** the output includes the wrapper function
- **AND** includes instructions for adding to .zshrc, .bashrc, or .bash_profile
- **AND** shows how to reload the shell configuration

### Requirement: Wrapper Function Features
The wrapper function SHALL support all router-switch features.

#### Scenario: Autocompletion support
- **WHEN** the wrapper is installed
- **THEN** it preserves autocompletion for provider names
- **AND** supports model name completion

#### Scenario: Error handling
- **WHEN** router-switch fails in the wrapper
- **THEN** errors are properly displayed
- **AND** the wrapper exits with appropriate code