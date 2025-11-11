# GitHub Actions Workflow Design

## Architecture Overview

### Build Matrix Strategy
使用GitHub Actions的矩阵策略实现跨平台编译：

```yaml
strategy:
  matrix:
    include:
      # Linux builds
      - os: ubuntu-latest
        arch: x86-64
        target: linux-x86_64
        compiler: gcc
      - os: ubuntu-latest
        arch: arm64
        target: linux-arm64
        compiler: aarch64-linux-gnu-gcc

      # macOS builds
      - os: macos-latest
        arch: x86-64
        target: darwin-x86_64
        compiler: clang
      - os: macos-latest
        arch: arm64
        target: darwin-arm64
        compiler: clang
```

### Cross-Compilation Approach

#### Linux ARM64
- 使用`aarch64-linux-gnu-gcc`交叉编译器
- 安装必要的交叉编译工具链
- 静态链接以避免依赖问题

#### macOS x86-64 (Intel)
- 在macOS ARM64 (Apple Silicon) runner上使用交叉编译
- 设置SDK目标为x86_64
- 使用`-target x86_64-apple-macos10.15`

#### Build Environment
- **Ubuntu**: 使用apt安装交叉编译工具链
- **macOS**: 使用Xcode命令行工具和SDK

### Release Management Strategy

#### Version Detection
```bash
# 从C代码中提取版本号
VERSION=$(grep "router-switch v" src/cli.c | sed 's/.*v\([0-9.]*\).*/\1/')

# 或者从Git标签获取
VERSION=$(git describe --tags --abbrev=0)
```

#### Asset Naming Convention
```
router-switch-v2.0.1-linux-x86_64
router-switch-v2.0.1-linux-arm64
router-switch-v2.0.1-darwin-x86_64
router-switch-v2.0.1-darwin-arm64
```

#### Release Trigger
- 推送到main分支时自动创建pre-release
- 手动创建Git标签时创建正式release

### Makefile Extensions

#### Cross-Compilation Targets
```makefile
# Linux cross-compilation
linux-x86_64:
	$(MAKE) release CC=gcc TARGET_OS=linux TARGET_ARCH=x86_64

linux-arm64:
	$(MAKE) release CC=aarch64-linux-gnu-gcc TARGET_OS=linux TARGET_ARCH=arm64

# macOS cross-compilation
darwin-x86_64:
	$(MAKE) release CC=clang TARGET_OS=darwin TARGET_ARCH=x86_64 \
		CFLAGS="$(CFLAGS) -target x86_64-apple-macos10.15"

darwin-arm64:
	$(MAKE) release CC=clang TARGET_OS=darwin TARGET_ARCH=arm64 \
		CFLAGS="$(CFLAGS) -target arm64-apple-macos11"
```

#### Packaging Target
```makefile
package: $(BINDIR_TARGET)/$(TARGET)
	@mkdir -p dist
	@cp $(BINDIR_TARGET)/$(TARGET) dist/$(PACKAGE_NAME)
	@chmod +x dist/$(PACKAGE_NAME)
	@echo "Packaged: dist/$(PACKAGE_NAME)"
```

### Workflow Implementation Details

#### Job Steps
1. **Checkout**: 获取源代码
2. **Setup Environment**: 安装编译器和工具链
3. **Build**: 使用Makefile编译目标平台
4. **Test**: 基本功能测试
5. **Package**: 重命名和准备二进制文件
6. **Upload**: 上传到GitHub Release

#### Caching Strategy
- 缓存编译器安装
- 缓存依赖包下载
- 缓存构建中间文件

### Security Considerations

#### Secrets Management
- GitHub Token自动提供，无需额外配置
- 不需要访问外部服务的敏感信息

#### Build Security
- 使用官方GitHub Actions镜像
- 验证编译产物完整性
- 二进制文件权限控制

### Error Handling

#### Build Failures
- 详细的错误日志输出
- 构建失败时通知机制
- 部分失败时的继续策略

#### Platform-Specific Issues
- 交叉编译器安装失败处理
- SDK不可用时的降级策略
- 权限问题的自动修复

### Performance Optimization

#### Parallel Builds
- 使用矩阵策略并行编译多个平台
- Make的并行编译选项

#### Build Time Optimization
- 缓存依赖安装
- 增量构建支持
- 最小化编译步骤

### Testing Strategy

#### Automated Tests
- 二进制文件基本功能测试
- 版本信息验证
- 帮助文档测试

#### Manual Testing
- 在实际目标平台上验证
- 用户体验测试

### Deployment Strategy

#### Rollout Plan
1. 先在测试分支验证工作流
2. 合并到main分支启用自动化
3. 监控首次完整构建流程
4. 根据反馈优化配置

#### Rollback Strategy
- 禁用GitHub Actions工作流
- 删除有问题的release
- 回退到手动构建流程