#include "router-switch.h"

int main(int argc, char *argv[]) {
    CliOptions options = {0};
    Config config = {0};
    char *current_provider = NULL;

    // Parse command line arguments
    parse_command_line_args(argc, argv, &options);

    // Handle help and version flags
    if (options.help) {
        display_help();
        return 0;
    }

    if (options.version) {
        display_version();
        return 0;
    }

    // Handle install flag
    if (options.install) {
        // Load configuration to get provider list for completion
        const char *config_path = strlen(options.config_path) > 0 ? options.config_path : "config.json";
        if (!load_config(config_path, &config)) {
            return 1;
        }

        // Print shell wrapper function
        print_shell_wrapper(&config);
        return 0;
    }

    // Validate that provider is specified
    if (strlen(options.provider) == 0) {
        fprintf(stderr, "Error: Provider must be specified with --provider or -p\n");
        return 1;
    }

    // Load configuration
    const char *config_path = strlen(options.config_path) > 0 ? options.config_path : "config.json";
    if (!load_config(config_path, &config)) {
        return 1;
    }

    // Validate provider and model before any output
    if (!validate_provider_and_model(&config, options.provider,
                                   strlen(options.model) > 0 ? options.model : NULL)) {
        return 1;
    }

    // Get current provider
    current_provider = get_current_provider();

    // Clear environment variables for current provider (if any)
    if (current_provider != NULL) {
        if (!clear_provider_environment(&config, current_provider)) {
            if (options.verbose) {
                fprintf(stderr, "Warning: Failed to clear environment for provider '%s'\n", current_provider);
            }
            // Don't return here, continue with setting new provider
        }
        free(current_provider);
    }

    // Set environment variables for new provider
    if (!apply_provider_environment(&config, options.provider,
                                   strlen(options.model) > 0 ? options.model : NULL)) {
        fprintf(stderr, "Error: Failed to set environment for provider '%s'\n", options.provider);
        return 1;
    }

    // Success
    return 0;
}