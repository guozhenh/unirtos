# UniRTOS SDK

[English](README.md) | 简体中文

UniRTOS SDK 是一个面向移远通信蜂窝模组各类平台的统一嵌入式开发套件，提供一致的接口与组件体系，支持跨平台开发与高效移植，集成丰富的中间件、驱动、协议栈和应用示例，助力快速开发和部署嵌入式智能终端。

## 主要特性
- 支持多平台（如 eigen_718）
- 基于 FreeRTOS 内核，适配多种硬件架构
- 组件化设计，支持灵活裁剪
- 丰富的驱动与中间件（网络、存储、音频、摄像头、FOTA、NVM 等）
- Kconfig 配置系统，支持 menuconfig 图形化配置
- CMake 构建系统，支持 Ninja/Make 多后端
- 提供标准应用示例和二次开发接口

## 目录结构
```
├── build.sh                # 一键构建脚本（Linux/Windows）
├── CMakeLists.txt          # 顶层 CMake 配置
├── Kconfig                 # 主配置入口
├── qos_applications/       # 应用示例与业务代码
│   ├── app_init/           # 应用初始化与注册
│   └── unirtos_std/        # 标准应用集
├── qos_components/         # 系统组件与中间件
│   ├── components/         # 可裁剪功能组件
│   └── system/             # 系统服务、驱动、协议栈
├── qos_kernel/             # 平台内核适配与库
│   └── eigen_718/          # 典型平台适配
├── qos_tools/              # 构建工具链与脚本
│   └── python/             # 构建/配置/打包 Python 工具
└── ...
```

## 快速上手

### 环境准备

参考[快速入门文档](https://docs.quectel.com/zh/UniRTOS/UniRTOS%E6%96%87%E6%A1%A3/%E5%BF%AB%E9%80%9F%E4%B8%8A%E6%89%8B/%E5%BF%AB%E9%80%9F%E4%B8%8A%E6%89%8B.html)，了解如何搭建开发环境，获取交叉编译工具链以及烧录工具等。

### 构建示例
> unirtos 是 UniRTOS SDK 的主入口，用于一键构建、配置、编译等操作，由交叉边编译工具链提供。
```sh
# 1. 一键构建
unirtos make <project> <version>
# 例如：
unirtos make EG800ZCN_LA EG800ZCNLAR01A01_BETA_OCPU_20260513

# 2. menuconfig 图形化配置
unirtos menuconfig

# 3. 清理构建产物
unirtos make clean
```

### 产物说明
- 构建输出位于 `qos_build/release/<version>/`

## 论坛社区
[点击访问](https://forumschinese.quectel.com/c/66-category/66)

## 贡献指南
欢迎提交 Issue、PR 参与共建！

---

如需商业支持或定制开发，请联系维护者。
