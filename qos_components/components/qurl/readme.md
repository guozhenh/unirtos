[toc]

# Overview

This file records development notes for qurl.

# Introduction

## Name

qurl means "UniRTOS URL". It provides a unified state-machine framework for protocol stacks. After a protocol is registered, the user side only needs to provide a URL to run the corresponding transfer.

# Build

## Build Command

Enter the `build` directory and run:

```shell
# Select the target platform and run cmake
cmake -DQURL_PLATFORM_CMAKE_FILE=platform/lzm_linux/platform.cmake ..
# Build
make
# Run
./qurl_iot.elf
```

# Debug

## Memory Leak Check

```txt
valgrind --leak-check=full --track-fds=yes --track-origins=yes ./lzm_qurl.elf
```

# License

The `qurl` component was developed with reference to the curl/libcurl project.
To keep third-party license and redistribution information with the component,
see:

- `THIRD_PARTY_NOTICES.md`
- `licenses/curl.txt`

If additional code is imported or ported from curl/libcurl in the future,
preserve the upstream provenance information in the affected source files.
