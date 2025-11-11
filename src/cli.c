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
        {0, 0, 0, 0}
    };

    // Reset options
    memset(options, 0, sizeof(CliOptions));

    while ((opt = getopt_long(argc, argv, "p:m:c:hvV", long_options, NULL)) != -1) {
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
    printf("This tool directly sets environment variables in the current process.\n");
    printf("By default, it operates silently on success. Use --verbose for detailed output.\n\n");
    printf("Usage: router-switch [options]\n\n");
    printf("Options:\n");
    printf("  -p, --provider <provider>  Specify AI provider from config.json\n");
    printf("  -m, --model <model>        Specify AI model for the provider\n");
    printf("  -c, --config <path>        Specify custom configuration file path\n");
    printf("  -v, --verbose              Show detailed output (default: silent)\n");
    printf("  -V, --version              Display version information\n");
    printf("  -h, --help                 Display this help message\n");
    printf("\nExamples:\n");
    printf("  router-switch --provider deepseek --model deepseek-chat     # Silent\n");
    printf("  router-switch -p glm --verbose                           # Verbose\n");
    printf("  router-switch -V -p openai                               # Version then switch\n");
    printf("  router-switch --config /path/to/custom-config.json\n");
    printf("\nBehavior:\n");
    printf("  - Silent by default: no output on successful operations\n");
    printf("  - Use --verbose or -v to see detailed environment variable changes\n");
    printf("  - Error messages always displayed regardless of verbose setting\n");
    printf("  - Child processes inherit the environment changes\n");
    printf("\nConfiguration:\n");
    printf("  Uses config.json file in the same directory for provider definitions by default\n");
    printf("  Use --config option to specify a custom configuration file path\n");
}

void display_version(void) {
    printf("router-switch v2.0.1\n");
}