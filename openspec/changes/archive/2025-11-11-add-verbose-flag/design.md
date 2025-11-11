# Technical Design: Add Verbose Output Mode

## Context
The current RouterSwitch v2.0 implementation always outputs detailed success messages showing which environment variables were set. While this is helpful for interactive use and debugging, it creates unnecessary noise in automated scripts, CI/CD pipelines, and other automated scenarios where silent operation is preferred.

## Goals / Non-Goals

### Goals
- Make default operation silent for automated use cases
- Provide optional verbose mode for interactive use and debugging
- Maintain all error and failure output (always visible)
- Update version to 2.0.1 to reflect the completion of environment variable changes
- Preserve existing functionality while improving user experience

### Non-Goals
- Changing error handling behavior
- Modifying the actual environment variable setting functionality
- Changing help/version display behavior
- Implementing different logging levels (just verbose vs silent)

## Decisions

### Decision: Silent Default with Verbose Flag
**What**: Default operation produces no output on success, `-V` flag enables detailed output
**Why**: Silent by default is better for automation, verbose flag preserves debugging capability
**Alternatives considered**:
- Always verbose with quiet flag - rejected as automation is more common than debugging
- Configurable default via config file - rejected as over-engineering for this use case

### Decision: Update to Version 2.0.1
**What**: Increment version number to 2.0.1
**Why**: Reflects completion of major v2.0 changes and this usability improvement
**Alternatives considered**:
- Stay at 2.0.0 - rejected as this is a behavior change worth versioning
- Jump to 2.1.0 - rejected as this is a minor usability improvement

### Decision: Always Show Errors
**What**: Error messages and failures always display regardless of verbose mode
**Why**: Errors need to be visible in all scenarios for debugging and user feedback
**Alternatives considered**:
- Hide errors in silent mode - rejected as errors should always be visible

## Risks / Trade-offs

### Risk: Breaking Change for Existing Users
- **Risk**: Users expecting output may think the tool isn't working
- **Mitigation**: Clear documentation, updated help text, and prominent migration notes

### Risk: Reduced Discoverability
- **Risk**: Users may not know about the verbose flag
- **Mitigation**: Updated help text, examples, and documentation

## Migration Plan

### Steps
1. Add verbose flag to CLI options
2. Modify output functions to respect verbose setting
3. Update help text to explain new behavior
4. Update version number
5. Update documentation
6. Release as v2.0.1

### Rollback
- Simple change to restore verbose default if needed
- Version bump is not irreversible

## Open Questions
- Should we add a config file option to set default verbosity?
- Should we consider different levels of verbosity in the future?
- How should we handle scripts that depend on the current output format?