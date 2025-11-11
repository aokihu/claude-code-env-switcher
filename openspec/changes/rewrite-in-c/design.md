# Technical Design: C Language Rewrite

## Context
The current RouterSwitch implementation uses TypeScript with Bun runtime, providing a CLI tool for switching AI providers and models. The rewrite to C aims to eliminate runtime dependencies while maintaining identical functionality.

## Goals / Non-Goals

### Goals
- Maintain 100% functional compatibility with existing TypeScript implementation
- Eliminate Bun/Node.js runtime dependency
- Reduce binary size and improve startup performance
- Support cross-platform compilation (Linux, macOS, Windows)
- Preserve all existing CLI behavior and configuration formats

### Non-Goals
- Adding new features beyond current functionality
- Changing the configuration file format
- Modifying the CLI interface or behavior
- Supporting additional programming languages

## Decisions

### Decision: Use Standard C Libraries
**What**: Use standard C libraries (libc) for core functionality instead of external dependencies
**Why**: Maximizes portability and minimizes dependencies
**Alternatives considered**:
- Using external JSON parsing libraries (e.g., jansson, cJSON) - rejected to minimize dependencies
- Using argument parsing libraries (e.g., argp, getopt_long) - using standard getopt for basic needs

### Decision: Manual JSON Parsing
**What**: Implement basic JSON parsing for configuration files
**Why**: Configuration files are simple and well-structured, manual parsing reduces dependencies
**Alternatives considered**:
- Using external JSON libraries - rejected for dependency minimization
- Using different configuration format (INI, YAML) - rejected to maintain compatibility

### Decision: Single Binary Distribution
**What**: Compile to a single statically-linked binary
**Why**: Simplifies deployment and eliminates runtime dependencies
**Alternatives considered**:
- Dynamic linking with system libraries - rejected for better portability
- Multiple binaries for different platforms - using single binary with conditional compilation

## Risks / Trade-offs

### Risk: JSON Parsing Complexity
- **Risk**: Manual JSON parsing may be error-prone for complex configurations
- **Mitigation**: Implement comprehensive validation and testing

### Risk: Cross-Platform Compatibility
- **Risk**: C code may behave differently across platforms
- **Mitigation**: Use standard C functions and extensive cross-platform testing

### Risk: Memory Management
- **Risk**: Manual memory management in C increases risk of leaks and errors
- **Mitigation**: Implement rigorous memory management patterns and testing

## Migration Plan

### Steps
1. Implement C version alongside TypeScript version
2. Test C implementation thoroughly against existing test cases
3. Update build system to compile C version as primary
4. Remove TypeScript source code and dependencies
5. Update documentation and deployment processes

### Rollback
- Keep TypeScript implementation until C version is fully validated
- Maintain ability to revert to TypeScript if critical issues found

## Open Questions
- Should we support both static and dynamic linking options?
- Are there specific platform limitations we need to address?
- What level of C standard compliance should we target?