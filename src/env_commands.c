#include "router-switch.h"
#include <errno.h>

// Task 1.1: Create set_environment_variable() function with error handling
static int set_environment_variable(const char *name, const char *value, EnvOperationResult *result) {
    if (!name || !value || !result) {
        return 0; // Invalid parameters
    }

    // Validate environment variable name
    if (strlen(name) == 0 || strlen(name) >= MAX_ENV_VAR_NAME) {
        snprintf(result->operations[result->operation_count].error_message,
                sizeof(result->operations[result->operation_count].error_message),
                "Invalid environment variable name: %s", name);
        result->operations[result->operation_count].success = 0;
        strncpy(result->operations[result->operation_count].var_name, name, MAX_ENV_VAR_NAME - 1);
        result->operation_count++;
        result->failure_count++;
        return 0;
    }

    // Validate environment variable value
    if (strlen(value) >= MAX_ENV_VAR_VALUE) {
        snprintf(result->operations[result->operation_count].error_message,
                sizeof(result->operations[result->operation_count].error_message),
                "Environment variable value too long for: %s", name);
        result->operations[result->operation_count].success = 0;
        strncpy(result->operations[result->operation_count].var_name, name, MAX_ENV_VAR_NAME - 1);
        result->operation_count++;
        result->failure_count++;
        return 0;
    }

    // Set the environment variable
    int set_result = setenv(name, value, 1); // 1 = overwrite existing
    if (set_result == 0) {
        // Success
        result->operations[result->operation_count].success = 1;
        strncpy(result->operations[result->operation_count].var_name, name, MAX_ENV_VAR_NAME - 1);
        strncpy(result->operations[result->operation_count].var_value, value, MAX_ENV_VAR_VALUE - 1);
        result->operations[result->operation_count].error_message[0] = '\0';
        result->operation_count++;
        result->success_count++;
        return 1;
    } else {
        // Failure
        snprintf(result->operations[result->operation_count].error_message,
                sizeof(result->operations[result->operation_count].error_message),
                "Failed to set environment variable %s (errno: %d)", name, errno);
        result->operations[result->operation_count].success = 0;
        strncpy(result->operations[result->operation_count].var_name, name, MAX_ENV_VAR_NAME - 1);
        result->operation_count++;
        result->failure_count++;
        return 0;
    }
}

// Task 1.2: Implement unset_environment_variable() function with error handling
static int unset_environment_variable(const char *name, EnvOperationResult *result) {
    if (!name || !result) {
        return 0; // Invalid parameters
    }

    // Validate environment variable name
    if (strlen(name) == 0 || strlen(name) >= MAX_ENV_VAR_NAME) {
        snprintf(result->operations[result->operation_count].error_message,
                sizeof(result->operations[result->operation_count].error_message),
                "Invalid environment variable name: %s", name);
        result->operations[result->operation_count].success = 0;
        strncpy(result->operations[result->operation_count].var_name, name, MAX_ENV_VAR_NAME - 1);
        result->operation_count++;
        result->failure_count++;
        return 0;
    }

    // Store the current value before unsetting (for reporting)
    char *current_value = getenv(name);
    if (current_value) {
        strncpy(result->operations[result->operation_count].var_value, current_value, MAX_ENV_VAR_VALUE - 1);
    } else {
        result->operations[result->operation_count].var_value[0] = '\0';
    }

    // Unset the environment variable
    int unset_result = unsetenv(name);
    if (unset_result == 0) {
        // Success
        result->operations[result->operation_count].success = 1;
        strncpy(result->operations[result->operation_count].var_name, name, MAX_ENV_VAR_NAME - 1);
        result->operations[result->operation_count].error_message[0] = '\0';
        result->operation_count++;
        result->success_count++;
        return 1;
    } else {
        // Failure
        snprintf(result->operations[result->operation_count].error_message,
                sizeof(result->operations[result->operation_count].error_message),
                "Failed to unset environment variable %s (errno: %d)", name, errno);
        result->operations[result->operation_count].success = 0;
        strncpy(result->operations[result->operation_count].var_name, name, MAX_ENV_VAR_NAME - 1);
        result->operation_count++;
        result->failure_count++;
        return 0;
    }
}

// Task 1.3: Create clear_provider_environment() function for bulk clearing
int clear_provider_environment(const Config *config, const char *provider_name, EnvOperationResult *result) {
    if (!config || !provider_name || !result) {
        return 0;
    }

    ProviderConfig *provider = find_provider(config, provider_name);
    if (!provider) {
        return 0; // Provider not found
    }

    // Initialize result tracking
    memset(result, 0, sizeof(EnvOperationResult));

    // Always clear standard Anthropic environment variables
    unset_environment_variable("ANTHROPIC_BASE_URL", result);
    unset_environment_variable("ANTHROPIC_AUTH_TOKEN", result);
    unset_environment_variable("ANTHROPIC_MODEL", result);

    // Clear custom environment variables from provider config
    for (int i = 0; i < provider->env_count; i++) {
        unset_environment_variable(provider->env_names[i], result);
    }

    // Clear current provider tracking
    unset_environment_variable("ROUTERSWITCH_CURRENT_PROVIDER", result);

    return (result->success_count > 0);
}

// Task 1.4: Create apply_provider_environment() function for bulk setting
int apply_provider_environment(const Config *config, const char *provider_name, const char *model_name, EnvOperationResult *result) {
    if (!config || !provider_name || !result) {
        return 0;
    }

    ProviderConfig *provider = find_provider(config, provider_name);
    if (!provider) {
        fprintf(stderr, "Provider '%s' not found in config.json\n", provider_name);
        return 0;
    }

    // Initialize result tracking
    memset(result, 0, sizeof(EnvOperationResult));

    // Set provider base configuration
    set_environment_variable("ANTHROPIC_BASE_URL", provider->base_url, result);
    set_environment_variable("ANTHROPIC_AUTH_TOKEN", provider->api_key, result);

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

        set_environment_variable("ANTHROPIC_MODEL", selected_model, result);
    }

    // Set custom environment variables
    for (int i = 0; i < provider->env_count; i++) {
        set_environment_variable(provider->env_names[i], provider->env_values[i], result);
    }

    // Update current provider tracking
    set_environment_variable("ROUTERSWITCH_CURRENT_PROVIDER", provider_name, result);

    return (result->success_count > 0);
}

// Task 1.5: Add environment operation success/failure tracking (implemented in above functions)

// Task 3.2: Implement success message formatting for environment changes
void print_environment_operations_result(const EnvOperationResult *result, const char *operation_type, int verbose) {
    if (!result || !operation_type) {
        return;
    }

    // Always show error messages regardless of verbose setting
    if (result->failure_count > 0) {
        fprintf(stderr, "Failed to %s environment variables:\n", operation_type);
        for (int i = 0; i < result->operation_count; i++) {
            if (!result->operations[i].success) {
                fprintf(stderr, "  %s: %s\n", result->operations[i].var_name, result->operations[i].error_message);
            }
        }
    }

    // Only show success messages in verbose mode
    if (verbose && result->success_count > 0) {
        printf("Successfully %s environment variables:\n", operation_type);
        for (int i = 0; i < result->operation_count; i++) {
            if (result->operations[i].success) {
                if (strlen(result->operations[i].var_value) > 0) {
                    printf("  Set %s to %s\n", result->operations[i].var_name, result->operations[i].var_value);
                } else {
                    printf("  Unset %s", result->operations[i].var_name);
                    if (strlen(result->operations[i].var_value) > 0) {
                        printf(" (was: %s)", result->operations[i].var_value);
                    }
                    printf("\n");
                }
            }
        }
    }
}

char* get_current_provider(void) {
    char *provider = getenv("ROUTERSWITCH_CURRENT_PROVIDER");
    if (provider) {
        return strdup(provider);
    }
    return NULL;
}