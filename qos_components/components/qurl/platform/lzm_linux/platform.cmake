# Paths
## Platform root path (the current file directory)
get_filename_component(_QURL_PLATFORM_PATH ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)
set(QURL_PLATFORM_PATH  "${_QURL_PLATFORM_PATH}" CACHE INTERNAL "qurl device path")

# Build-related settings
set(CMAKE_C_COMPILER "gcc" CACHE INTERNAL "c compiler")
set(CMAKE_CXX_COMPILER "g++" CACHE INTERNAL "cxx compiler")

### For 32-bit systems
# set(CMAKE_C_FLAGS "-m33 -Wl,-e,entry" CACHE INTERNAL "c flags")
# set(CMAKE_CXX_FLAGS "-m32" CACHE INTERNAL "cxx flags")
# set(CMAKE_EXE_LINKER_FLAGS "-m32" CACHE INTERNAL "exe linkerflags")

# Debug-only flags
set(CXX_FORMAT_SIGNEDNESS "-Wformat-signedness")
set(CXX_STACK_PROTECTOR "-fstack-protector-all")
set(CMAKE_C_FLAGS "-g -O0 -ggdb -Wall -Wno-format -Wextra -Werror ${CXX_FORMAT_SIGNEDNESS} ${CXX_STACK_PROTECTOR}" CACHE INTERNAL "c flags")

# Target
set(QURL_TARGET_NAME "lzm_qurl" CACHE INTERNAL "qurl target name")

# Build configuration
## Build the executable
set(QURL_COMPILE_CFG_EXE ON)
## Enable APPS
set(QURL_COMPILE_CFG_APPS ON)
## Build the SDK package
set(QURL_COMPILE_CFG_SDK ON)
set(QURL_COMPILE_CFG_SDK_CMD python3 ${CMAKE_CURRENT_SOURCE_DIR}/tools/qurl_sdk_build.py qurl_args --qurl_sdk_xml_file ${QURL_PLATFORM_PATH}/tools/qurl_sdk_filelist.xml)

# User files
## User source files
set(QURL_USER_SRC
    ${QURL_PLATFORM_PATH}/apps/qurl_main.c
CACHE INTERNAL "qurl user src")
## User include paths
set(QURL_USER_INC
    ${QURL_PLATFORM_PATH}/include
CACHE INTERNAL "qurl user include")
## User libraries
set(QURL_USER_LIBS
CACHE INTERNAL "qurl user libs")

## Port source files
set(QURL_USER_PORT_SRC
    ${QURL_PLATFORM_PATH}/port/qurl_platform_port.c
CACHE INTERNAL "qurl user src")
## Port include paths
set(QURL_USER_PORT_INC
    ${QURL_PLATFORM_PATH}/include
    ${QURL_PLATFORM_PATH}/mbedtls/include
CACHE INTERNAL "qurl user include")
## Port library search paths
set(QURL_USER_PORT_LIBS_PATH
    ${QURL_PLATFORM_PATH}/mbedtls/libs
CACHE INTERNAL "qurl user libs path")
## Port libraries
set(QURL_USER_PORT_LIBS
    pthread
    rt
    c
    m
    mbedtls
    mbedx509
    mbedcrypto
CACHE INTERNAL "qurl user libs")
