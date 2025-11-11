#include "router-switch.h"
#include <ctype.h>

// Simple JSON parser for config files
static char* extract_string_between_quotes(const char *str, int *end_pos) {
    int start = -1;
    int end = -1;
    int i = 0;

    // Find opening quote
    while (str[i]) {
        if (str[i] == '"') {
            start = i + 1;
            break;
        }
        i++;
    }

    if (start == -1) return NULL;

    // Find closing quote (accounting for escaped quotes)
    i = start;
    while (str[i]) {
        if (str[i] == '"') {
            // Check if this quote is escaped
            int backslash_count = 0;
            int j = i - 1;
            while (j >= 0 && str[j] == '\\') {
                backslash_count++;
                j--;
            }
            if (backslash_count % 2 == 0) {
                // Not escaped, this is the end
                end = i;
                break;
            }
        }
        i++;
    }

    if (end == -1) return NULL;

    int len = end - start;
    char *result = malloc(len + 1);
    if (!result) return NULL;

    strncpy(result, str + start, len);
    result[len] = '\0';
    *end_pos = end + 1;

    return result;
}

static const char* find_json_value(const char *json, const char *key) {
    char search_key[256];
    snprintf(search_key, sizeof(search_key), "\"%s\"", key);

    const char *key_pos = strstr(json, search_key);
    if (!key_pos) return NULL;

    // Find the colon after the key
    const char *colon = strstr(key_pos, ":");
    if (!colon) return NULL;

    // Return position after colon
    const char *value = colon + 1;
    while (*value && isspace(*value)) value++;

    return value;
}

int parse_config_file(const char *filename, Config *config) {
    FILE *file;
    char *content = NULL;
    long size;
    int result = 0;

    // Initialize config
    memset(config, 0, sizeof(Config));

    // Open and read file
    file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open config file '%s'\n", filename);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    content = malloc(size + 1);
    if (!content) {
        fprintf(stderr, "Failed to allocate memory\n");
        fclose(file);
        return 0;
    }

    if (fread(content, 1, (size_t)size, file) != (size_t)size) {
        fprintf(stderr, "Failed to read config file\n");
        free(content);
        fclose(file);
        return 0;
    }
    content[size] = '\0';
    fclose(file);

    // Find providers object
    const char *providers_value = find_json_value(content, "providers");
    if (!providers_value || *providers_value != '{') {
        fprintf(stderr, "Invalid config format: providers object not found\n");
        free(content);
        return 0;
    }

    // Parse each provider
    const char *current = providers_value + 1; // Skip opening brace
    int provider_count = 0;

    while (*current && *current != '}' && provider_count < MAX_PROVIDERS) {
        // Skip whitespace
        while (*current && isspace(*current)) current++;
        if (*current == '}' || *current == '\0') break;

        // Extract provider name (key)
        int name_end;
        char *provider_name = extract_string_between_quotes(current, &name_end);
        if (!provider_name) break;

        current += name_end;

        // Skip whitespace and colon
        while (*current && (isspace(*current) || *current == ':')) current++;
        if (*current != '{') {
            free(provider_name);
            break;
        }

        ProviderConfig *provider = &config->providers[provider_count];
        strncpy(provider->name, provider_name, MAX_PROVIDER_NAME - 1);
        provider->name[MAX_PROVIDER_NAME - 1] = '\0';
        free(provider_name);

        // Find the end of this provider object
        int brace_count = 1;
        const char *provider_start = current + 1; // Skip opening brace
        const char *provider_end = provider_start;

        while (*provider_end && brace_count > 0) {
            if (*provider_end == '{') brace_count++;
            else if (*provider_end == '}') brace_count--;
            provider_end++;
        }

        if (brace_count > 0) break;

        // Extract provider object content (without braces)
        int provider_obj_len = provider_end - current - 1;
        char *provider_obj = malloc(provider_obj_len + 1);
        if (!provider_obj) break;

        strncpy(provider_obj, current + 1, provider_obj_len);
        provider_obj[provider_obj_len] = '\0';

        // Parse fields within provider object

        // Description
        const char *desc_value = find_json_value(provider_obj, "description");
        if (desc_value && *desc_value == '"') {
            int desc_end;
            char *desc = extract_string_between_quotes(desc_value, &desc_end);
            if (desc) {
                strncpy(provider->description, desc, sizeof(provider->description) - 1);
                free(desc);
            }
        }

        // Base URL
        const char *url_value = find_json_value(provider_obj, "base_url");
        if (url_value && *url_value == '"') {
            int url_end;
            char *url = extract_string_between_quotes(url_value, &url_end);
            if (url) {
                strncpy(provider->base_url, url, sizeof(provider->base_url) - 1);
                free(url);
            }
        }

        // API Key
        const char *key_value = find_json_value(provider_obj, "api_key");
        if (key_value && *key_value == '"') {
            int key_end;
            char *key = extract_string_between_quotes(key_value, &key_end);
            if (key) {
                strncpy(provider->api_key, key, sizeof(provider->api_key) - 1);
                free(key);
            }
        }

        // Models array
        const char *models_value = find_json_value(provider_obj, "models");
        if (models_value && *models_value == '[') {
            const char *array_start = models_value + 1;
            const char *array_end = array_start;

            while (*array_end && *array_end != ']') array_end++;

            // Parse models in the array
            const char *model_current = array_start;
            provider->model_count = 0;

            while (*model_current && *model_current != ']' && provider->model_count < MAX_MODELS_PER_PROVIDER) {
                while (*model_current && (isspace(*model_current) || *model_current == ',')) model_current++;
                if (*model_current == ']' || *model_current == '\0') break;

                if (*model_current == '"') {
                    int model_end;
                    char *model = extract_string_between_quotes(model_current, &model_end);
                    if (model) {
                        strncpy(provider->models[provider->model_count], model, MAX_MODEL_NAME - 1);
                        free(model);
                        provider->model_count++;
                    }
                    model_current += model_end;
                } else {
                    model_current++;
                }
            }
        }

        // Environment variables object
        const char *env_value = find_json_value(provider_obj, "env");
        if (env_value && *env_value == '{') {
            const char *env_start = env_value + 1;
            const char *env_end = env_start;
            int brace_count = 1;

            while (*env_end && brace_count > 0) {
                if (*env_end == '{') brace_count++;
                else if (*env_end == '}') brace_count--;
                env_end++;
            }

            // Parse environment variables
            const char *env_current = env_start;
            provider->env_count = 0;

            while (*env_current && *env_current != '}' && provider->env_count < MAX_PROVIDERS) {
                while (*env_current && (isspace(*env_current) || *env_current == ',')) env_current++;
                if (*env_current == '}' || *env_current == '\0') break;

                if (*env_current == '"') {
                    // Extract key
                    int key_end;
                    char *env_key = extract_string_between_quotes(env_current, &key_end);
                    env_current += key_end;

                    // Skip to colon and value
                    while (*env_current && (isspace(*env_current) || *env_current == ':')) env_current++;

                    if (*env_current == '"') {
                        int value_end;
                        char *env_value = extract_string_between_quotes(env_current, &value_end);
                        if (env_key && env_value) {
                            strncpy(provider->env_names[provider->env_count], env_key, MAX_ENV_VAR_NAME - 1);
                            strncpy(provider->env_values[provider->env_count], env_value, MAX_ENV_VAR_VALUE - 1);
                            provider->env_count++;
                        }
                        if (env_key) free(env_key);
                        if (env_value) free(env_value);
                        env_current += value_end;
                    }
                } else {
                    env_current++;
                }
            }
        }

        free(provider_obj);
        current = provider_end;
        provider_count++;
    }

    config->provider_count = provider_count;
    free(content);
    result = 1;

    return result;
}