#!/usr/bin/env node

import { parseCommandLineArgs } from "./parse_argv.js";
import {
  getCurrentProvider,
  generateClearEnvironmentCommands,
} from "./clear_env.js";
import {
  generateSetEnvironmentCommands,
  validateProviderAndModel,
} from "./set_env.js";

async function main(): Promise<void> {
  try {
    const options = parseCommandLineArgs();

    // Handle help and version flags
    if (options.help || options.version) {
      // For help and version, still show the output
      if (options.help) {
        const { displayHelp } = await import("./parse_argv.js");
        displayHelp();
      } else if (options.version) {
        const { displayVersion } = await import("./parse_argv.js");
        displayVersion();
      }
      return;
    }

    // Validate that provider is specified
    if (!options.provider) {
      console.error("Error: Provider must be specified with --provider or -p");
      process.exit(1);
    }

    // Validate provider and model
    validateProviderAndModel(options.provider, options.model);

    // Get current provider and generate clear commands
    const currentProvider = getCurrentProvider();
    const allCommands: string[] = [];

    if (currentProvider) {
      const clearCommands = generateClearEnvironmentCommands(currentProvider);
      allCommands.push(...clearCommands);
    }

    // Generate set commands for new provider
    const setCommands = generateSetEnvironmentCommands(
      options.provider,
      options.model,
    );
    allCommands.push(...setCommands);

    // Output only the commands for eval execution
    allCommands.forEach((command) => {
      console.log(command);
    });
  } catch (error) {
    console.error("Error:", error instanceof Error ? error.message : error);
    process.exit(1);
  }
}

// Run the main function
main().catch((error) => {
  console.error("Unexpected error:", error);
  process.exit(1);
});
