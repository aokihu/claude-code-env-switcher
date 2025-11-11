#include "router-switch.h"

void parse_command_line_args(int argc, char *argv[], CliOptions *options) {
    int opt;

    static struct option long_options[] = {
        {"provider", required_argument, 0, 'p'},
        {"model",    required_argument, 0, 'm'},
        {"config",   required_argument, 0, 'c'},
        {"help",     no_argument,       0, 'h'},
        {"version",  no_argument,       0, 'V'},
        {"verbose",  no_argument,       0, 'v'},
        {"install",  no_argument,       0, 'i'},
        {0, 0, 0, 0}
    };

    // Reset options
    memset(options, 0, sizeof(CliOptions));

    while ((opt = getopt_long(argc, argv, "p:m:c:hvVi", long_options, NULL)) != -1) {
        switch (opt) {
            case 'p':
                strncpy(options->provider, optarg, MAX_PROVIDER_NAME - 1);
                options->provider[MAX_PROVIDER_NAME - 1] = '\0';
                break;
            case 'm':
                strncpy(options->model, optarg, MAX_MODEL_NAME - 1);
                options->model[MAX_MODEL_NAME - 1] = '\0';
                break;
            case 'c':
                strncpy(options->config_path, optarg, MAX_CONFIG_PATH - 1);
                options->config_path[MAX_CONFIG_PATH - 1] = '\0';
                break;
            case 'h':
                options->help = 1;
                break;
            case 'V':
                options->version = 1;
                break;
            case 'v':
                options->verbose = 1;
                break;
            case 'i':
                options->install = 1;
                break;
            case '?':
                fprintf(stderr, "Unknown option. Use --help for usage information.\n");
                exit(1);
            default:
                fprintf(stderr, "Unexpected error parsing options.\n");
                exit(1);
        }
    }
}

void display_help(void) {
    printf("RouterSwitch CLI Tool\n");
    printf("=====================\n");
    printf("A CLI tool to switch between AI providers and models for Claude Code\n\n");
    printf("This tool outputs shell commands to set environment variables.\n");
    printf("Use eval to execute the commands in your shell: eval $(router-switch -p provider)\n\n");
    printf("Usage: router-switch [options]\n\n");
    printf("Options:\n");
    printf("  -p, --provider <provider>  Specify AI provider from config.json\n");
    printf("  -m, --model <model>        Specify AI model for the provider\n");
    printf("  -c, --config <path>        Specify custom configuration file path\n");
    printf("  -v, --verbose              Show detailed output to stderr\n");
    printf("  -i, --install              Generate shell wrapper function for easy usage\n");
    printf("  -V, --version              Display version information\n");
    printf("  -h, --help                 Display this help message\n");
    printf("\nExamples:\n");
    printf("  eval $(router-switch --provider deepseek --model deepseek-chat)\n");
    printf("  eval $(router-switch -p glm)                                      # Simple\n");
    printf("  router-switch --install >> ~/.zshrc && source ~/.zshrc            # Install wrapper\n");
    printf("  router-switch --config /path/to/custom-config.json\n");
    printf("\nInstallation:\n");
    printf("  Run 'router-switch --install' to generate a shell wrapper function.\n");
    printf("  Add the output to your ~/.zshrc or ~/.bashrc, then reload your shell.\n");
    printf("  After installation, you can use: router-switch deepseek\n");
    printf("\nConfiguration:\n");
    printf("  Uses config.json file in the same directory for provider definitions by default\n");
    printf("  Use --config option to specify a custom configuration file path\n");
}

void display_version(void) {
    printf("router-switch v3.0.0\n");
}