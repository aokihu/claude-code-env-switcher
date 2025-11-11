# Change: Refactor CLI to output clean eval commands

## Why
The previous implementation generated shell scripts that required manual sourcing. Users requested a cleaner approach that outputs directly executable shell commands for immediate use with `eval`.

## What Changes
- Remove all descriptive output text from CLI
- Output only pure shell commands (`export` and `unset`)
- Optimize for direct `eval "$(command)"` usage
- Maintain all validation and error handling

## Impact
- Affected specs: `cli-interface` (modified)
- Affected code: `src/index.ts` (main output logic)
- User experience: Simplified, more direct usage