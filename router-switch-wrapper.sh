#!/bin/bash

# Wrapper script for router-switch to set environment variables in current shell

# Path to the actual router-switch binary
ROUTER_SWITCH_BIN="./bin/release/router-switch"

# Create a temporary file to capture the environment changes
TEMP_ENV_FILE=$(mktemp)

# Function to cleanup
cleanup() {
    rm -f "$TEMP_ENV_FILE"
}
trap cleanup EXIT

# Modify router-switch to output export commands instead of setting environment
# This requires modifying the source code or creating a special mode
# For now, we'll manually parse the verbose output

# Get the provider and model from arguments
PROVIDER=""
MODEL=""
VERBOSE=0

while [[ $# -gt 0 ]]; do
    case $1 in
        -p|--provider)
            PROVIDER="$2"
            shift 2
            ;;
        -m|--model)
            MODEL="$2"
            shift 2
            ;;
        -v|--verbose)
            VERBOSE=1
            shift
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

if [[ -z "$PROVIDER" ]]; then
    echo "Error: Provider is required"
    echo "Usage: $0 -p <provider> [-m <model>] [-v]"
    exit 1
fi

# Parse config.json and set environment variables
CONFIG_FILE="config.json"
if [[ ! -f "$CONFIG_FILE" ]]; then
    echo "Error: config.json not found"
    exit 1
fi

# Use jq to parse JSON (requires jq to be installed)
if command -v jq &> /dev/null; then
    # Extract provider configuration
    BASE_URL=$(jq -r ".providers.${PROVIDER}.base_url // empty" "$CONFIG_FILE")
    API_KEY=$(jq -r ".providers.${PROVIDER}.api_key // empty" "$CONFIG_FILE")

    # Set model
    if [[ -n "$MODEL" ]]; then
        SELECTED_MODEL="$MODEL"
    else
        # Get first model from array or use default
        SELECTED_MODEL=$(jq -r ".providers.${PROVIDER}.models[0] // empty" "$CONFIG_FILE")
    fi

    # Set base environment variables
    if [[ -n "$BASE_URL" ]]; then
        export ANTHROPIC_BASE_URL="$BASE_URL"
        echo "Set ANTHROPIC_BASE_URL to $BASE_URL"
    fi

    if [[ -n "$API_KEY" ]]; then
        export ANTHROPIC_AUTH_TOKEN="$API_KEY"
        echo "Set ANTHROPIC_AUTH_TOKEN"
    fi

    if [[ -n "$SELECTED_MODEL" ]]; then
        export ANTHROPIC_MODEL="$SELECTED_MODEL"
        echo "Set ANTHROPIC_MODEL to $SELECTED_MODEL"
    fi

    # Set custom environment variables
    ENV_VARS=$(jq -r ".providers.${PROVIDER}.env // {} | to_entries[] | \"\\(.key)=\\(.value)\"" "$CONFIG_FILE" 2>/dev/null)
    while IFS='=' read -r key value; do
        if [[ -n "$key" && -n "$value" ]]; then
            export "$key"="$value"
            echo "Set $key to $value"
        fi
    done <<< "$ENV_VARS"

    # Set current provider tracking
    export ROUTERSWITCH_CURRENT_PROVIDER="$PROVIDER"
    echo "Set ROUTERSWITCH_CURRENT_PROVIDER to $PROVIDER"

else
    echo "Error: jq is required for JSON parsing. Please install jq."
    echo "Alternatively, modify router-switch to support an 'export mode'."
    exit 1
fi

if [[ $VERBOSE -eq 1 ]]; then
    echo "Environment variables have been set in the current shell."
fi