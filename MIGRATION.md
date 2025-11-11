# Migration Guide: RouterSwitch v2.0.1

## Breaking Change Alert

RouterSwitch v2.0 introduced a **breaking change** in how environment variables are set. The tool no longer outputs shell commands for evaluation with `eval`. Instead, it directly sets environment variables in the current process.

## v2.0.1 Update: Silent Default Behavior

RouterSwitch v2.0.1 introduces **silent default behavior** to improve compatibility with automation and scripts. The tool now operates silently by default, showing detailed output only when requested with the `--verbose` or `-v` flag.

## What Changed

### Before (v1.x)
```bash
# Had to wrap with eval
eval $(router-switch --provider deepseek --model deepseek-chat)

# Output was shell commands:
# export ANTHROPIC_BASE_URL="https://api.deepseek.com/anthropic"
# export ANTHROPIC_AUTH_TOKEN="sk-..."
# export ANTHROPIC_MODEL="deepseek-chat"
```

### After (v2.0)
```bash
# Direct execution
router-switch --provider deepseek --model deepseek-chat

# Output is success messages:
# Successfully set environment variables:
#   Set ANTHROPIC_BASE_URL to https://api.deepseek.com/anthropic
#   Set ANTHROPIC_AUTH_TOKEN to sk-...
#   Set ANTHROPIC_MODEL to deepseek-chat
```

## Migration Steps

### 1. Update Your Scripts
Replace all instances of `eval $(router-switch ...)` with direct calls:

```bash
# Old
eval $(router-switch --provider deepseek --model deepseek-chat)

# New
router-switch --provider deepseek --model deepseek-chat
```

### 2. Update Shell Functions
If you have shell functions that use RouterSwitch:

```bash
# Old
switch_provider() {
    eval $(router-switch --provider "$1")
}

# New
switch_provider() {
    router-switch --provider "$1"
}
```

### 3. Update Aliases
Update any aliases that use the old pattern:

```bash
# Old
alias rs-deepseek='eval $(router-switch --provider deepseek)'

# New
alias rs-deepseek='router-switch --provider deepseek'
```

## Important Notes

### Environment Inheritance
- Environment variables are set in the RouterSwitch process only
- **Child processes** started by RouterSwitch will inherit the environment
- **Parent shell** environment is NOT modified (this is impossible in Unix systems)
- Use `source` or `exec` if you need to modify the current shell

### For Shell Script Integration
If you need to modify the current shell environment:

```bash
# Option 1: Source the tool (if modified to support sourcing)
source <(router-switch --provider deepseek)

# Option 2: Use exec to replace current process
exec router-switch --provider deepseek

# Option 3: Manual export (not recommended)
eval $(router-switch --provider deepseek --output-commands)  # This option no longer exists
```

### Docker and Container Environments
The new approach works better in container environments since it doesn't require shell evaluation:

```dockerfile
# Dockerfile
FROM ubuntu:22.04
COPY router-switch /usr/local/bin/
COPY config.json /app/
WORKDIR /app
# Direct setting without eval
RUN router-switch --provider deepseek
```

## Benefits of the New Approach

1. **Simpler Usage**: No need to remember `eval` wrapper
2. **Better Error Handling**: Direct feedback on success/failure
3. **Improved Scripting**: Cleaner, more readable scripts
4. **Better Process Isolation**: Environment changes are contained
5. **Enhanced Debugging**: Clear success/failure messages

## Troubleshooting

### Q: Environment variables don't persist in my shell
A: This is expected behavior. RouterSwitch sets variables in its own process space. Use one of the approaches in the "For Shell Script Integration" section.

### Q: My existing scripts broke after upgrade
A: Remove `eval $(...)` wrapper from all RouterSwitch calls.

### Q: I need the old behavior
A: Unfortunately, the old command output mode is no longer supported. You'll need to adapt your workflow.

## Examples

### Development Workflow
```bash
# Set up development environment
router-switch --provider deepseek --model deepseek-chat

# Run your AI-powered tools
./my-ai-tool.py  # Inherits the environment variables
```

### CI/CD Pipeline
```yaml
# GitHub Actions example
- name: Setup AI Provider
  run: router-switch --provider deepseek

- name: Run AI Tests
  run: npm test  # Tests inherit the environment
```

### Shell Script Example
```bash
#!/bin/bash
# setup-ai-env.sh
provider=${1:-deepseek}
model=${2:-deepseek-chat}

router-switch --provider "$provider" --model "$model"
echo "AI environment configured with $provider"
```

## Need Help?

If you encounter issues during migration:
1. Check this guide first
2. Review the new help text: `router-switch --help`
3. Test with a simple provider switch first
4. Ensure your config.json is properly formatted