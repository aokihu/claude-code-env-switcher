# Technical Design: Direct Environment Variable Setting

## Context
The current RouterSwitch implementation generates shell commands (export/unset) that modify the shell environment when executed via `eval`. This design creates user friction and requires understanding shell evaluation. We will replace this with direct environment variable setting for a more immediate and user-friendly experience.

## Goals / Non-Goals

### Goals
- Directly set environment variables in the current process by default
- Provide immediate feedback on successful environment changes
- Preserve all existing validation and error handling logic
- Simplify user workflow by removing eval requirement
- Ensure robust error handling for environment operations

### Non-Goals
- Modifying environment variables of parent processes (impossible)
- Cross-session environment persistence
- Maintaining backward compatibility with command output mode
- Shell-specific environment management beyond standard POSIX env vars

## Decisions

### Decision: Replace Command Output with Direct Setting
**What**: Completely replace shell command generation with direct environment variable setting
**Why**: Simplified user experience, immediate environment changes, eliminates eval complexity
**Alternatives considered**:
- Dual-mode approach with flags - rejected as overly complex for this use case
- Gradual migration - rejected to avoid maintaining two code paths

### Decision: Use Standard C setenv/unsetenv Functions
**What**: Use POSIX standard functions `setenv()` and `unsetenv()` for environment variable manipulation
**Why**: Cross-platform compatibility, standard behavior, proper memory management
**Alternatives considered**:
- Using `putenv()` - rejected due to memory management complexities
- Direct environment array manipulation - rejected for portability concerns

### Decision: Add Success Feedback
**What**: Output success messages indicating which environment variables were set/cleared
**Why**: Users need confirmation that environment changes took effect
**Alternatives considered**:
- Silent operation - rejected as users wouldn't know if changes succeeded
- Verbose logging - rejected as too much output for simple operations

## Risks / Trade-offs

### Risk: Breaking Change
- **Risk**: Existing scripts using `eval $(router-switch ...)` will break
- **Mitigation**: Clear migration guide, simple script update process

### Risk: Parent Process Environment Inheritance
- **Risk**: Child processes inherit environment but parent shell doesn't see changes
- **Mitigation**: Clear documentation about environment inheritance behavior

### Risk: Error Handling Complexity
- **Risk**: Environment variable setting can fail due to memory constraints
- **Mitigation**: Proper error checking and informative error messages

## Migration Plan

### Steps
1. Implement environment variable setting functions
2. Replace command generation logic with direct setting
3. Update output formatting for success/error messages
4. Add comprehensive testing for environment operations
5. Update documentation and usage examples
6. Release as breaking change with migration guide

### Rollback
- Not applicable - this is a complete replacement of functionality

## Open Questions
- How should we handle environment variable setting failures?
- Should we add a "status" command to show current environment state?
- What level of feedback is appropriate for successful operations?
- Should we add environment variable persistence features?