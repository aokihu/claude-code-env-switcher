# Change router-switch to Output Shell Commands by Default

## Summary
Modify router-switch to output shell export commands (`export ENV=VAL`) by default instead of directly modifying environment variables. This allows users to set environment variables in their current shell using `eval $(router-switch -p provider)`. Additionally, add an `--install`/`-i` flag that generates a shell wrapper function for easier usage.

## Problem Statement
Currently, router-switch directly modifies environment variables using `setenv()`, which only affects the child process and not the parent shell. Users expect the tool to output shell commands that can be executed to persistently set environment variables. Additionally, users would benefit from a convenient wrapper function that eliminates the need to type `eval` every time.

## Proposed Solution
1. Change the default behavior to output shell commands instead of modifying environment variables
2. Add an `--install`/`-i` flag that outputs a shell wrapper function that can be added to .zshrc, .bashrc, etc.
3. The wrapper function will automatically handle the eval call, making usage simpler

## Impact Analysis
- **Breaking Change**: Yes - this changes the default behavior from v2.0
- **Scope**: Core functionality change + new install option
- **Dependencies**: None
- **Risks**: Moderate - requires users to change how they invoke the tool

## Example Usage
After installation:
```bash
# Instead of: eval $(router-switch -p deepseek)
router-switch deepseek
```