#ifndef ROUTER_SWITCH_H
#define ROUTER_SWITCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

// Maximum string lengths
#define MAX_CONFIG_PATH 1024
#define MAX_PROVIDER_NAME 64
#define MAX_MODEL_NAME 64
#define MAX_ENV_VAR_NAME 128
#define MAX_ENV_VAR_VALUE 512
#define MAX_COMMAND_LENGTH 2048
#define MAX_PROVIDERS 10
#define MAX_MODELS_PER_PROVIDER 20

// Provider configuration structure
typedef struct {
    char name[MAX_PROVIDER_NAME];
    char description[256];
    char base_url[512];
    char api_key[512];
    char models[MAX_MODELS_PER_PROVIDER][MAX_MODEL_NAME];
    int model_count;
    char env_names[MAX_PROVIDERS][MAX_ENV_VAR_NAME];
    char env_values[MAX_PROVIDERS][MAX_ENV_VAR_VALUE];
    int env_count;
} ProviderConfig;

// Configuration structure
typedef struct {
    ProviderConfig providers[MAX_PROVIDERS];
    int provider_count;
} Config;

// Command line options structure
typedef struct {
    char provider[MAX_PROVIDER_NAME];
    char model[MAX_MODEL_NAME];
    char config_path[MAX_CONFIG_PATH];
    int help;
    int version;
    int verbose;
    int install;
} CliOptions;

// Function declarations

// main.c
int main(int argc, char *argv[]);

// config.c
int load_config(const char *config_path, Config *config);
ProviderConfig* find_provider(const Config *config, const char *provider_name);
int validate_provider_and_model(const Config *config, const char *provider_name, const char *model_name);

// cli.c
void parse_command_line_args(int argc, char *argv[], CliOptions *options);
void display_help(void);
void display_version(void);

// env_commands.c
// Environment management functions
int clear_provider_environment(const Config *config, const char *provider_name);
int apply_provider_environment(const Config *config, const char *provider_name, const char *model_name);
void print_shell_wrapper(const Config *config);
char* get_current_provider(void);

// json_parser.c
int parse_config_file(const char *filename, Config *config);
char* extract_json_string(const char *json, const char *key);
int extract_json_string_array(const char *json, const char *key, char values[][MAX_MODEL_NAME], int max_values);
int extract_json_object(const char *json, const char *key, char keys[][MAX_ENV_VAR_NAME], char values[][MAX_ENV_VAR_VALUE], int max_pairs);

#endif // ROUTER_SWITCH_H