import { ProviderConfig, loadConfig } from "./clear_env.js";

export function generateSetEnvironmentCommands(
  providerName: string,
  modelName?: string,
): string[] {
  const config = loadConfig();
  const provider = config.providers[providerName];
  const commands: string[] = [];

  if (!provider) {
    throw new Error(`Provider '${providerName}' not found in config.json`);
  }

  // Set provider base configuration
  commands.push(`export ANTHROPIC_BASE_URL="${provider.base_url}"`);
  commands.push(`export ANTHROPIC_AUTH_TOKEN="${provider.api_key}"`);

  // Set model if applicable
  if (provider.models && provider.models.length > 0) {
    let selectedModel: string;

    if (modelName) {
      // Validate user-specified model
      if (!provider.models.includes(modelName)) {
        throw new Error(
          `Model '${modelName}' not found for provider '${providerName}'. Available models: ${provider.models.join(", ")}`,
        );
      }
      selectedModel = modelName;
    } else {
      // Use default model (first in array)
      selectedModel = provider.models[0];
    }

    commands.push(`export ANTHROPIC_MODEL="${selectedModel}"`);
  }

  // Set custom environment variables
  if (provider.env) {
    Object.entries(provider.env).forEach(([key, value]) => {
      commands.push(`export ${key}="${value}"`);
    });
  }

  // Update current provider tracking
  commands.push(`export ROUTERSWITCH_CURRENT_PROVIDER="${providerName}"`);

  return commands;
}

export function validateProviderAndModel(
  providerName: string,
  modelName?: string,
): void {
  const config = loadConfig();

  if (!config.providers[providerName]) {
    throw new Error(
      `Provider '${providerName}' not found in config.json. Available providers: ${Object.keys(config.providers).join(", ")}`,
    );
  }

  const provider = config.providers[providerName];

  if (modelName && provider.models && provider.models.length > 0) {
    if (!provider.models.includes(modelName)) {
      throw new Error(
        `Model '${modelName}' not found for provider '${providerName}'. Available models: ${provider.models.join(", ")}`,
      );
    }
  }
}
