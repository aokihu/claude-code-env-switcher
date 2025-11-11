# Add GitHub Actions Workflow

## Why
当前RouterSwitch项目缺少自动化构建和发布流程，用户需要手动编译源代码，这对非技术用户门槛较高。项目需要提供预编译的二进制文件供不同平台的用户直接下载使用。

## Summary
创建GitHub Actions工作流，实现自动编译Release版本二进制文件并发布，支持Linux和macOS两个平台以及ARM64和x86-64处理器架构。

## Problem Statement
当前RouterSwitch项目缺少自动化构建和发布流程。用户需要手动编译源代码，这对于非技术用户来说门槛较高。同时，项目没有提供预编译的二进制文件供不同平台的用户直接下载使用。

## Proposed Solution
实现GitHub Actions CI/CD工作流，在代码推送到main分支时自动触发：
1. 跨平台编译（Linux、macOS）
2. 多架构支持（x86-64、ARM64）
3. 创建GitHub Release
4. 上传编译好的二进制文件作为Release Assets

## Key Benefits
- 🚀 **自动化发布**: 代码推送后自动构建和发布
- 🌍 **跨平台支持**: Linux和macOS用户都能获得原生二进制文件
- 🔧 **多架构**: 支持x86-64和ARM64处理器
- 📦 **易于安装**: 用户可直接下载预编译版本
- 🔄 **版本管理**: 自动创建Git标签和Release

## Scope
- GitHub Actions工作流配置
- 跨平台编译脚本优化
- Release自动化流程
- 二进制文件打包和命名规范

## Out of Scope
- Windows平台支持（当前C代码未针对Windows优化）
- 包管理器集成（如Homebrew、APT）
- 自动化测试集成（可作为后续改进）

## Dependencies
- GitHub Actions服务
- 现有Makefile构建系统
- Git版本控制

## Success Criteria
- [ ] 推送到main分支时自动触发构建
- [ ] 生成4个二进制文件：Linux x86-64、Linux ARM64、macOS x86-64、macOS ARM64
- [ ] 自动创建GitHub Release
- [ ] 二进制文件正确打包并上传为Release Assets
- [ ] 版本号与Git标签同步