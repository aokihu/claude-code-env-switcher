import { parseArgs } from 'node:util';

export interface CliOptions {
  provider?: string;
  model?: string;
  help?: boolean;
  version?: boolean;
}

export function parseCommandLineArgs(): CliOptions {
  const options = {
    provider: {
      type: 'string' as const,
      short: 'p',
      description: 'Specify AI provider from config.json'
    },
    model: {
      type: 'string' as const,
      short: 'm',
      description: 'Specify AI model for the provider'
    },
    help: {
      type: 'boolean' as const,
      short: 'h',
      description: 'Display help information'
    },
    version: {
      type: 'boolean' as const,
      short: 'v',
      description: 'Display version information'
    }
  } as const;

  try {
    const { values } = parseArgs({ options, allowPositionals: true });
    return values as CliOptions;
  } catch (error) {
    throw new Error(`Failed to parse command line arguments: ${error instanceof Error ? error.message : error}`);
  }
}

export function displayHelp(): void {
  console.log('RouterSwitch CLI Tool');
  console.log('=====================');
  console.log('A CLI tool to switch between AI providers and models for Claude Code\n');
  console.log('Usage: router-switch [options]\n');
  console.log('Options:');
  console.log('  -p, --provider <provider>  Specify AI provider from config.json');
  console.log('  -m, --model <model>        Specify AI model for the provider');
  console.log('  -h, --help                 Display this help message');
  console.log('  -v, --version              Display version information');
  console.log('\nExamples:');
  console.log('  router-switch --provider deepseek --model deepseek-chat');
  console.log('  router-switch -p glm');
  console.log('\nConfiguration:');
  console.log('  Uses config.json file in the same directory for provider definitions');
}

export function displayVersion(): void {
  console.log('router-switch v1.0.0');
}
