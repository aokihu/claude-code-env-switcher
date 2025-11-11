# Shell Wrapper Function Design

## Wrapper Function Output Format

When user runs `router-switch --install`, the tool will output:

```bash
# Shell wrapper function for router-switch
# Add this to your ~/.zshrc, ~/.bashrc, or ~/.bash_profile

router-switch() {
    # Path to router-switch binary (adjust if needed)
    local ROUTER_SWITCH_CMD="${ROUTER_SWITCH_BIN:-$(dirname ${funcsourcetrace[1]%:*})/bin/release/router-switch}"

    # If no arguments, show help
    if [ $# -eq 0 ]; then
        echo "Usage: router-switch <provider> [model] [options]"
        echo "  provider: AI provider name (e.g., deepseek, glm)"
        echo "  model: Optional model name"
        echo "  options: Additional options passed to router-switch"
        echo ""
        echo "Examples:"
        echo "  router-switch deepseek"
        echo "  router-switch deepseek deepseek-chat"
        echo "  router-switch -v glm"
        return 1
    fi

    # Parse arguments
    local args=()
    local provider=""
    local model=""
    local has_provider_flag=false
    local has_model_flag=false

    # Check if provider flag is already used
    for arg in "$@"; do
        case $arg in
            -p|--provider)
                has_provider_flag=true
                ;;
            -m|--model)
                has_model_flag=true
                ;;
        esac
    done

    # Build command arguments
    if [ "$has_provider_flag" = false ]; then
        # First argument is provider
        provider="$1"
        args+=(-p "$provider")
        shift
    fi

    if [ "$has_model_flag" = false ] && [ $# -gt 0 ] && [[ ! "$1" =~ ^- ]]; then
        # Next argument is model (if it doesn't start with -)
        model="$1"
        args+=(-m "$model")
        shift
    fi

    # Add remaining arguments
    args+=("$@")

    # Execute router-switch with eval
    eval "$($ROUTER_SWITCH_CMD "${args[@]}")"
}

# Tab completion for zsh
if command -v compdef >/dev/null 2>&1; then
    _router-switch() {
        local -a providers
        providers=(deepseek glm minimax)

        if [[ $CURRENT -eq 2 ]]; then
            _describe 'providers' providers
        fi
    }
    compdef _router-switch router-switch
fi

# Tab completion for bash
if command -v complete >/dev/null 2>&1; then
    _router_switch_bash() {
        local providers="deepseek glm minimax"
        local cur=${COMP_WORDS[COMP_CWORD]}

        if [ $COMP_CWORD -eq 1 ]; then
            COMPREPLY=($(compgen -W "$providers" -- "$cur"))
        fi
    }
    complete -F _router_switch_bash router-switch
fi

# To enable the wrapper, reload your shell or run:
# source ~/.zshrc  # or ~/.bashrc
```

## Implementation Notes

1. **Binary Detection**: The wrapper tries to auto-detect the router-switch binary path relative to where it was sourced from

2. **Flexible Arguments**:
   - Supports both `router-switch provider model` and `router-switch -p provider -m model` syntax
   - Passes through any additional flags (like -v, -c, etc.)

3. **Cross-shell Compatibility**:
   - Works with both bash and zsh
   - Includes tab completion for both shells

4. **Customization**:
   - Users can override binary path with `ROUTER_SWITCH_BIN` environment variable
   - Simple syntax for common use cases

5. **Installation Instructions**:
   - Clear comments explaining where to add the function
   - Instructions for reloading shell configuration