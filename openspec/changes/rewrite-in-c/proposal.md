# Change: Rewrite RouterSwitch in C Language

## Why
Rewriting the RouterSwitch CLI tool from TypeScript/Bun to C will provide better performance, smaller binary size, and eliminate the Node.js/Bun runtime dependency. This makes the tool more portable and suitable for environments where JavaScript runtimes are not available or desirable.

## What Changes
- **BREAKING** Technology stack change from TypeScript/Bun to C
- **BREAKING** Build process changes from Bun compilation to C compilation
- **BREAKING** Package management changes from npm to system package managers
- **BREAKING** Runtime dependency changes from Bun to standard C libraries
- **MODIFIED** Implementation language while maintaining identical CLI behavior

## Impact
- Affected specs: cli-interface
- Affected code: All source files (src/*.ts → src/*.c)
- Build system: package.json → Makefile
- Distribution: npm packages → compiled binaries