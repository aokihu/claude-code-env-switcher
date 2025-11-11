import { readFileSync } from "node:fs";
import { join } from "node:path";

export interface ProviderConfig {
  description: string;
  base_url: string;
  api_key: string;
  models: string[];
  env: Record<string, string | number>;
}

export interface Config {
  providers: Record<string, ProviderConfig>;
}

export function loadConfig(): Config {
  try {
    const configPath = join(process.cwd(), "config.json");
    const configContent = readFileSync(configPath, "utf-8");
    return JSON.parse(configContent) as Config;
  } catch (error) {
    throw new Error(
      `Failed to load config.json: ${error instanceof Error ? error.message : error}`,
    );
  }
}

export function generateClearEnvironmentCommands(
  providerName: string,
): string[] {
  const config = loadConfig();
  const provider = config.providers[providerName];
  const commands: string[] = [];

  if (!provider) {
    return commands; // Provider not found in config, nothing to clear
  }

  // Generate unset commands for provider-specific environment variables
  commands.push("unset ANTHROPIC_BASE_URL");
  commands.push("unset ANTHROPIC_AUTH_TOKEN");
  commands.push("unset ANTHROPIC_MODEL");

  // Generate unset commands for custom environment variables from provider config
  if (provider.env) {
    Object.keys(provider.env).forEach((envVar) => {
      commands.push(`unset ${envVar}`);
    });
  }

  // Also unset the current provider tracking variable
  commands.push("unset ROUTERSWITCH_CURRENT_PROVIDER");

  return commands;
}

export function getCurrentProvider(): string | null {
  return process.env.ROUTERSWITCH_CURRENT_PROVIDER || null;
}
