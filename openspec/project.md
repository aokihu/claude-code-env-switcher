# Project Context

## Purpose
使用`routerswitch`为`Claude Code`实现切换AI供应商和模型的功能,用户可以通过设置配置文件自定义AI供应商和模型

## Tech Stack
- Typescript 5.0
- Bun 1.3

## Project Conventions

### Code Style
- 只使用`bun`内置的功能实现,或者`nodejs`兼容的内置功能,不使用任何第三方的库

### Architecture Patterns

- `dist/` 目录中存放的是编译后的可执行文件
- `src/` 目录中是程序源代码
- `src/parse_argv.ts` 是解析命令行参数的源文件
- `src/clear_env.ts` 是清理环境变量的源文件
- `src/set_env.ts` 是设置环境变量的源文件

### Testing Strategy
[Explain your testing approach and requirements]

### Git Workflow
- 开始新的`Spec`开发的时候,创建并切换到一个新的分支进行开发
- 当`Spec`归档的时候,将分支合并到`master`主分支

## Domain Context

### 配置文件
配置文件名称是`config.json`,默认保存在同一目录下

配置文件的格式如下
```json
{
  "providers": {
    "deepseek": {
      "description":"Deepseek V3.2",
      "base_url":"https://api.deepseek.com/anthropic",
      "api_key":"sk-...",
      "models": ["deepseek-chat", "deepseek-reasoner"],
      "env": {
        "ANTHROPIC_DEFAULT_SONNET_MODEL":"deepseek-chat",   // 最高级模型
        "ANTHROPIC_DEFAULT_OPUS_MODEL":"deepseek-reasoner", // 高级模型
        "ANTHROPIC_DEFAULT_HAIKU_MODEL":"deepseek-chat",    // 快速模型
      }
    },
    "glm": {
      "description":"BigModel GLM 4.6",
      "base_url":"https://open.bigmodel.cn/api/anthropic",
      "api_key":"sk-...",
      "models":[],                                        // 表示按照Claude Code的内置模型执行
      "env": {
        "API_TIMEOUT_MS": "3000000",
        "CLAUDE_CODE_DISABLE_NONESSENTIAL_TRAFFIC": 1
      }
    }
  }
}
```
- *providers*是主键,表示有哪些AI供应商可以使用
- *deepseek*以及*glm*是供应商的名称,使用`routerswitch -p deepseek`或者`routerswitch -p glm`的时候作为参数使用
- *description* 是对供应商和模型的描述,没有实际作用,但是可以作为回答时候参考
- *base_url* 和 *api_key* 是AI供应商的基础信息,在设置环境变量的时候要转换成 *ANTHROPIC_BASE_URL* 以及 *ANTHROPIC_AUTH_TOKEN* 环境变量名称
- *models* 是一个数组,数组中第一个是默认的模型名称,命令行参数如果没有指定模型名称,那么就使用默认的模型名称,比如`routerswitch -p deepseek`那么就使用默认的模型名称`deepseek-chat`
- *models* 中设置环境变量的名称是*ANTHROPIC_MODEL*,这个环境变量类型是一个字符串,只能设置一个模型的名称
- *models* 的数组长度如果为*0*,那么就不用设置模型的环境变量
- *env* 是用户设定的其他的环境变量,**key**就是环境变量名,**value**就是环境变量的值

### 设置环境变量的流程

1. 从环境变量中读取*ROUTERSWITCH_CURRENT_PROVIDER*
2. 根据当前的AI供应商,清除对应的环境变量,包括*env*中的环境变量
3. 根据命令行参数设置AI供应商和模型
  3.1 *base_url* 转化成环境变量 *ANTHROPIC_BASE_URL*
  3.2 *api_key* 转换成环境量 *ANTHROPIC_AUTH_TOKEN*
  3.3 如果*models*数组是空, 则不设置*ANTHROPIC_MODEL*
  3.4 如果*models*数组不为空, 并且用户没有设置模型, 则*models*数组中第一个模型设置为*ANTHROPIC_MODEL*, 如果用户设置了模型, 并且与*models*中的模型匹配则设置为用户指定的模型
  3.5 *env* 存在并且不为空的情况下, 直接按照**key**:**value**的形式设置环境变量
4. 更新环境变量*ROUTERSWITCH_CURRENT_PROVIDER*,设置为当前用户设定的AI供应商

## Important Constraints
[List any technical, business, or regulatory constraints]

## External Dependencies
[Document key external services, APIs, or systems]
