# UniRTOS SDK

[English](README.md) | [简体中文](README_zh.md)

UniRTOS SDK is a unified embedded development kit for multiple Quectel cellular module platforms. It provides consistent APIs and component architecture, supports cross-platform development and efficient porting, and integrates rich middleware, drivers, protocol stacks, and application examples to accelerate the development and deployment of intelligent embedded devices.

## Key Features
- Multi-platform support (for example, eigen_718)
- Based on the FreeRTOS kernel and adaptable to multiple hardware architectures
- Modular design with flexible feature trimming
- Rich drivers and middleware (networking, storage, audio, camera, FOTA, NVM, and more)
- Kconfig-based configuration with menuconfig UI support
- CMake build system with Ninja/Make backends
- Standard application examples and secondary development interfaces

## Directory Structure
```
├── build.sh                # One-click build script (Linux/Windows)
├── CMakeLists.txt          # Top-level CMake configuration
├── Kconfig                 # Main configuration entry
├── qos_applications/       # Application examples and business code
│   ├── app_init/           # Application initialization and registration
│   └── unirtos_std/        # Standard application set
├── qos_components/         # System components and middleware
│   ├── components/         # Modular feature components
│   └── system/             # System services, drivers, protocol stacks
├── qos_kernel/             # Platform kernel adaptation and libraries
│   └── eigen_718/          # Typical platform adaptation
├── qos_tools/              # Build toolchain and scripts
│   └── python/             # Python tools for build/config/package
└── ...
```

## Quick Start

### Environment Setup

Refer to the [Quick Start Guide](https://docs.quectel.com/zh/UniRTOS/UniRTOS%E6%96%87%E6%A1%A3/%E5%BF%AB%E9%80%9F%E4%B8%8A%E6%89%8B/%E5%BF%AB%E9%80%9F%E4%B8%8A%E6%89%8B.html) to learn how to set up the development environment and obtain the cross-compilation toolchain, flashing tools, and related dependencies.

### Build Examples
> `unirtos` is the main entry command of UniRTOS SDK for one-click build, configuration, and compilation operations. It is provided by the cross-compilation toolchain package.
```sh
# 1. Build in one command
unirtos make <project> <version>
# Example:
unirtos make EG800ZCN_LA EG800ZCNLAR01A01_BETA_OCPU_20260513

# 2. Run menuconfig
unirtos menuconfig

# 3. Clean build outputs
unirtos make clean
```

### Build Outputs
- Build outputs are located at `qos_build/release/<version>/`.

## Community Forum
[Visit the Forum](https://forumschinese.quectel.com/c/66-category/66)

## Contribution Guide
Issues and pull requests are welcome.

---

For commercial support or custom development, please contact the maintainers.
