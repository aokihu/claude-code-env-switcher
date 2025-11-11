# ci-cd Specification

## Purpose
自动化构建、测试和发布RouterSwitch二进制文件的持续集成和持续部署流程。

## Requirements

## ADDED Requirements

### Requirement: Automated Build Pipeline
系统**应该**在代码推送时自动触发跨平台编译流程。The system **SHALL** automatically trigger cross-platform compilation when code is pushed.

#### Scenario: Code pushed to main branch
- **WHEN** code is pushed to the main branch
- **THEN** the system **SHALL** automatically trigger GitHub Actions workflow
- **AND** **SHALL** initiate builds for all target platforms (Linux x86-64, Linux ARM64, macOS x86-64, macOS ARM64)
- **AND** **SHALL** use appropriate cross-compilation tools for each build
- **AND** **SHALL** generate build artifacts with platform-specific optimizations

#### Scenario: Git tag is created
- **WHEN** a Git tag is pushed to the repository
- **THEN** the system **SHALL** trigger GitHub Actions workflow for release creation
- **AND** **SHALL** extract version number from the tag
- **AND** **SHALL** create builds for all target platforms
- **AND** **SHALL** create a formal GitHub Release with the compiled binaries

### Requirement: Cross-Platform Compilation
系统**应该**能够为不同的操作系统和处理器架构编译二进制文件。The system **SHALL** compile binaries for different operating systems and processor architectures.

#### Scenario: Building for Linux x86-64
- **WHEN** building for Linux x86-64 platform
- **THEN** the system **SHALL** use GCC compiler on Ubuntu runner
- **AND** **SHALL** apply Linux-specific compilation flags
- **AND** **SHALL** create optimized binary with `-O3 -flto -march=x86-64`
- **AND** **SHALL** output binary named `router-switch-v{version}-linux-x86_64`

#### Scenario: Building for Linux ARM64
- **WHEN** building for Linux ARM64 platform
- **THEN** the system **SHALL** install and use aarch64-linux-gnu-gcc cross-compiler
- **AND** **SHALL** use static linking to avoid runtime dependencies
- **AND** **SHALL** apply ARM64-specific optimizations
- **AND** **SHALL** output binary named `router-switch-v{version}-linux-arm64`

#### Scenario: Building for macOS x86-64
- **WHEN** building for macOS x86-64 platform
- **THEN** the system **SHALL** use Clang compiler on macOS runner
- **AND** **SHALL** set target to x86_64-apple-macos with appropriate minimum version
- **AND** **SHALL** apply macOS-specific compilation flags
- **AND** **SHALL** output binary named `router-switch-v{version}-darwin-x86_64`

#### Scenario: Building for macOS ARM64
- **WHEN** building for macOS ARM64 platform
- **THEN** the system **SHALL** use Clang compiler on Apple Silicon runner
- **AND** **SHALL** set target to arm64-apple-macos with appropriate minimum version
- **AND** **SHALL** apply ARM64-specific optimizations
- **AND** **SHALL** output binary named `router-switch-v{version}-darwin-arm64`

### Requirement: Release Management
系统**应该**自动管理GitHub Release的创建和二进制文件的发布。The system **SHALL** automatically manage GitHub Release creation and binary distribution.

#### Scenario: Creating release from main branch push
- **WHEN** code is pushed to main branch without a tag
- **THEN** the system **SHALL** create a pre-release with timestamp-based version
- **AND** **SHALL** upload all compiled binaries as release assets
- **AND** **SHALL** include build information and checksums in release notes
- **AND** **SHALL** mark the release as pre-release for testing purposes

#### Scenario: Creating formal release from Git tag
- **WHEN** a Git tag matching version pattern (vX.Y.Z) is pushed
- **THEN** the system **SHALL** create a formal GitHub Release
- **AND** **SHALL** use the tag version for release naming
- **AND** **SHALL** upload all compiled binaries as release assets
- **AND** **SHALL** generate comprehensive release notes with changelog
- **AND** **SHALL** create checksums for all binary files

### Requirement: Build Validation
系统**应该**验证编译产物的质量和功能完整性。The system **SHALL** validate the quality and functionality of compiled artifacts.

#### Scenario: Validating compiled binaries
- **WHEN** compilation is completed for any platform
- **THEN** the system **SHALL** verify the binary file exists and is executable
- **AND** **SHALL** run `--version` command to verify version information
- **AND** **SHALL** run `--help` command to verify basic functionality
- **AND** **SHALL** check binary size is within expected range
- **AND** **SHALL** generate SHA256 checksum for integrity verification

#### Scenario: Handling build failures
- **WHEN** compilation fails for any platform
- **THEN** the system **SHALL** log detailed error information
- **AND** **SHALL** mark the workflow run as failed
- **AND** **SHALL** provide actionable error messages in GitHub Actions logs
- **AND** **SHALL** continue building other platforms if possible (matrix continue-on-error)

### Requirement: Asset Management
系统**应该**正确管理和命名Release资产文件。The system **SHALL** properly manage and name Release assets.

#### Scenario: Packaging release assets
- **WHEN** binaries are successfully compiled
- **THEN** the system **SHALL** rename binaries with platform-specific naming convention
- **AND** **SHALL** set appropriate file permissions (executable)
- **AND** **SHALL** create checksum files for each binary
- **AND** **SHALL** package assets in organized structure

#### Scenario: Uploading to GitHub Release
- **WHEN** all binaries are packaged and validated
- **THEN** the system **SHALL** create or update GitHub Release
- **AND** **SHALL** upload each platform binary as a separate release asset
- **AND** **SHALL** upload checksum files as release assets
- **AND** **SHALL** include platform information in asset descriptions
- **AND** **SHALL** verify successful upload of all assets

### Requirement: Workflow Security
系统**应该**确保构建过程的安全性和可靠性。The system **SHALL** ensure the security and reliability of the build process.

#### Scenario: Using GitHub Actions securely
- **WHEN** workflow is executed
- **THEN** the system **SHALL** use official GitHub Actions images
- **AND** **SHALL** limit permissions to minimum required (contents: write)
- **AND** **SHALL** validate inputs and environment variables
- **AND** **SHALL** not expose sensitive information in logs

#### Scenario: Managing build dependencies
- **WHEN** installing cross-compilation tools
- **THEN** the system **SHALL** use package manager with pinned versions
- **AND** **SHALL** validate tool installation before compilation
- **AND** **SHALL** cache dependencies for faster subsequent builds
- **AND** **SHALL** clean up temporary files after completion