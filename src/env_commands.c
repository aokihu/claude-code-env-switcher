#include "router-switch.h"
#include <errno.h>

// Shell escaping utility function
static char* shell_escape(const char* value) {
    if (!value) return NULL;

    // Check if value needs escaping
    int needs_escape = 0;
    for (const char* p = value; *p; p++) {
        if (*p == '\'' || *p == '"' || *p == '$' || *p == '`' ||
            *p == '\\' || *p == '\n' || *p == '\t' || *p == ' ' ||
            *p == '(' || *p == ')' || *p == '[' || *p == ']' ||
            *p == '{' || *p == '}' || *p == '|' || *p == '&' ||
            *p == ';' || *p == '<' || *p == '>' || *p == '!' ||
            *p == '*' || *p == '?' || *p == '#') {
            needs_escape = 1;
            break;
        }
    }

    if (!needs_escape) {
        return strdup(value);
    }

    // Count single quotes to determine buffer size
    int quote_count = 0;
    for (const char* p = value; *p; p++) {
        if (*p == '\'') quote_count++;
    }

    // Allocate buffer for escaped string
    char* escaped = malloc(strlen(value) + quote_count * 4 + 3); // +3 for leading/trailing quotes
    if (!escaped) return NULL;

    char* dest = escaped;
    *dest++ = '\'';

    for (const char* src = value; *src; src++) {
        if (*src == '\'') {
            // Close quote, add escaped quote, reopen quote
            *dest++ = '\'';
            *dest++ = '"';
            *dest++ = '\'';
            *dest++ = '"';
            *dest++ = '\'';
        } else {
            *dest++ = *src;
        }
    }

    *dest++ = '\'';
    *dest = '\0';

    return escaped;
}

// Print export command for environment variable
static void print_export_command(const char* name, const char* value) {
    if (!name || !value) return;

    char* escaped_value = shell_escape(value);
    if (escaped_value) {
        printf("export %s=%s\n", name, escaped_value);
        free(escaped_value);
    }
}

// Print unset command for environment variable
static void print_unset_command(const char* name) {
    if (!name) return;
    printf("unset %s\n", name);
}

// Clear provider environment by outputting unset commands
int clear_provider_environment(const Config *config, const char *provider_name) {
    if (!config || !provider_name) {
        return 0;
    }

    ProviderConfig *provider = find_provider(config, provider_name);
    if (!provider) {
        return 0; // Provider not found
    }

    // Always unset standard Anthropic environment variables
    print_unset_command("ANTHROPIC_BASE_URL");
    print_unset_command("ANTHROPIC_AUTH_TOKEN");
    print_unset_command("ANTHROPIC_MODEL");

    // Unset custom environment variables from provider config
    for (int i = 0; i < provider->env_count; i++) {
        print_unset_command(provider->env_names[i]);
    }

    // Clear current provider tracking
    print_unset_command("ROUTERSWITCH_CURRENT_PROVIDER");

    return 1;
}

// Apply provider environment by outputting export commands
int apply_provider_environment(const Config *config, const char *provider_name, const char *model_name) {
    if (!config || !provider_name) {
        return 0;
    }

    ProviderConfig *provider = find_provider(config, provider_name);
    if (!provider) {
        fprintf(stderr, "Provider '%s' not found in config.json\n", provider_name);
        return 0;
    }

    // Set provider base configuration
    print_export_command("ANTHROPIC_BASE_URL", provider->base_url);
    print_export_command("ANTHROPIC_AUTH_TOKEN", provider->api_key);

    // Set model if applicable
    if (provider->model_count > 0) {
        const char *selected_model;

        if (model_name) {
            // Validate user-specified model
            int model_found = 0;
            for (int i = 0; i < provider->model_count; i++) {
                if (strcmp(provider->models[i], model_name) == 0) {
                    model_found = 1;
                    break;
                }
            }
            if (!model_found) {
                fprintf(stderr, "Model '%s' not found for provider '%s'\n", model_name, provider_name);
                return 0;
            }
            selected_model = model_name;
        } else {
            // Use default model (first in array)
            selected_model = provider->models[0];
        }

        print_export_command("ANTHROPIC_MODEL", selected_model);
    }

    // Set custom environment variables
    for (int i = 0; i < provider->env_count; i++) {
        print_export_command(provider->env_names[i], provider->env_values[i]);
    }

    // Update current provider tracking
    print_export_command("ROUTERSWITCH_CURRENT_PROVIDER", provider_name);

    return 1;
}

// Print shell wrapper function for installation
void print_shell_wrapper(const Config *config) {
    printf("# Shell wrapper function for router-switch\n");
    printf("# Add this to your ~/.zshrc, ~/.bashrc, or ~/.bash_profile\n\n");

    printf("router-switch() {\n");
    printf("    # Path to router-switch binary (adjust if needed)\n");
    printf("    local ROUTER_SWITCH_CMD=\"${ROUTER_SWITCH_BIN:-$(dirname \"${BASH_SOURCE[0]:-${(%):-%x}}\")/bin/release/router-switch}\"\n\n");

    printf("    # If no arguments, show help\n");
    printf("    if [ $# -eq 0 ]; then\n");
    printf("        echo \"Usage: router-switch <provider> [model] [options]\"\n");
    printf("        echo \"  provider: AI provider name (e.g., deepseek, glm");

    // Add provider names from config
    for (int i = 0; i < config->provider_count; i++) {
        printf(", %s", config->providers[i].name);
    }
    printf(")\"\n");

    printf("        echo \"  model: Optional model name\"\n");
    printf("        echo \"  options: Additional options passed to router-switch\"\n");
    printf("        echo \"\"\n");
    printf("        echo \"Examples:\"\n");
    printf("        echo \"  router-switch deepseek\"\n");
    printf("        echo \"  router-switch deepseek deepseek-chat\"\n");
    printf("        echo \"  router-switch -v glm\"\n");
    printf("        return 1\n");
    printf("    fi\n\n");

    printf("    # If --install flag, pass through to actual binary\n");
    printf("    if [ \"$1\" = \"--install\" ] || [ \"$1\" = \"-i\" ]; then\n");
    printf("        \"$ROUTER_SWITCH_CMD\" \"$@\"\n");
    printf("        return $?\n");
    printf("    fi\n\n");

    printf("    # Parse arguments\n");
    printf("    local args=()\n");
    printf("    local provider=\"\"\n");
    printf("    local model=\"\"\n");
    printf("    local has_provider_flag=false\n");
    printf("    local has_model_flag=false\n\n");

    printf("    # Check if provider flag is already used\n");
    printf("    for arg in \"$@\"; do\n");
    printf("        case $arg in\n");
    printf("            -p|--provider)\n");
    printf("                has_provider_flag=true\n");
    printf("                ;;\n");
    printf("            -m|--model)\n");
    printf("                has_model_flag=true\n");
    printf("                ;;\n");
    printf("        esac\n");
    printf("    done\n\n");

    printf("    # Build command arguments\n");
    printf("    if [ \"$has_provider_flag\" = false ]; then\n");
    printf("        # First argument is provider\n");
    printf("        provider=\"$1\"\n");
    printf("        args+=(-p \"$provider\")\n");
    printf("        shift\n");
    printf("    fi\n\n");

    printf("    if [ \"$has_model_flag\" = false ] && [ $# -gt 0 ] && [[ ! \"$1\" =~ ^- ]]; then\n");
    printf("        # Next argument is model (if it doesn't start with -)\n");
    printf("        model=\"$1\"\n");
    printf("        args+=(-m \"$model\")\n");
    printf("        shift\n");
    printf("    fi\n\n");

    printf("    # Add remaining arguments\n");
    printf("    args+=(\"$@\")\n\n");

    printf("    # Execute router-switch with eval\n");
    printf("    eval \"$(\"$ROUTER_SWITCH_CMD\" \"${args[@]}\")\"\n");
    printf("}\n\n");

    // Tab completion for zsh
    printf("# Tab completion for zsh\n");
    printf("if command -v compdef >/dev/null 2>&1; then\n");
    printf("    _router-switch() {\n");
    printf("        local -a providers\n");
    printf("        providers=(");

    for (int i = 0; i < config->provider_count; i++) {
        printf("%s ", config->providers[i].name);
    }
    printf(")\n\n");

    printf("        if [[ $CURRENT -eq 2 ]]; then\n");
    printf("            _describe 'providers' providers\n");
    printf("        fi\n");
    printf("    }\n");
    printf("    compdef _router-switch router-switch\n");
    printf("fi\n\n");

    // Tab completion for bash
    printf("# Tab completion for bash\n");
    printf("if command -v complete >/dev/null 2>&1; then\n");
    printf("    _router_switch_bash() {\n");
    printf("        local providers=\"");

    for (int i = 0; i < config->provider_count; i++) {
        if (i > 0) printf(" ");
        printf("%s", config->providers[i].name);
    }
    printf("\"\n");
    printf("        local cur=${COMP_WORDS[COMP_CWORD]}\n\n");
    printf("        if [ $COMP_CWORD -eq 1 ]; then\n");
    printf("            COMPREPLY=($(compgen -W \"$providers\" -- \"$cur\"))\n");
    printf("        fi\n");
    printf("    }\n");
    printf("    complete -F _router_switch_bash router-switch\n");
    printf("fi\n\n");

    printf("# To enable the wrapper, reload your shell or run:\n");
    printf("# source ~/.zshrc  # or ~/.bashrc\n");
}

char* get_current_provider(void) {
    char *provider = getenv("ROUTERSWITCH_CURRENT_PROVIDER");
    if (provider) {
        return strdup(provider);
    }
    return NULL;
}