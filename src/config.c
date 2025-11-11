#include "router-switch.h"

int load_config(const char *config_path, Config *config) {
    if (!parse_config_file(config_path, config)) {
        fprintf(stderr, "Failed to load config file '%s'\n", config_path);
        return 0;
    }
    return 1;
}

ProviderConfig* find_provider(const Config *config, const char *provider_name) {
    for (int i = 0; i < config->provider_count; i++) {
        if (strcmp(config->providers[i].name, provider_name) == 0) {
            return (ProviderConfig*)&config->providers[i];
        }
    }
    return NULL;
}

int validate_provider_and_model(const Config *config, const char *provider_name, const char *model_name) {
    ProviderConfig *provider = find_provider(config, provider_name);

    if (!provider) {
        fprintf(stderr, "Provider '%s' not found in config.json. Available providers: ", provider_name);
        for (int i = 0; i < config->provider_count; i++) {
            if (i > 0) fprintf(stderr, ", ");
            fprintf(stderr, "%s", config->providers[i].name);
        }
        fprintf(stderr, "\n");
        return 0;
    }

    if (model_name && provider->model_count > 0) {
        int model_found = 0;
        for (int i = 0; i < provider->model_count; i++) {
            if (strcmp(provider->models[i], model_name) == 0) {
                model_found = 1;
                break;
            }
        }
        if (!model_found) {
            fprintf(stderr, "Model '%s' not found for provider '%s'. Available models: ",
                   model_name, provider_name);
            for (int i = 0; i < provider->model_count; i++) {
                if (i > 0) fprintf(stderr, ", ");
                fprintf(stderr, "%s", provider->models[i]);
            }
            fprintf(stderr, "\n");
            return 0;
        }
    }

    return 1;
}