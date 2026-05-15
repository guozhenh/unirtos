# Paths
## Platform root path (the current file directory)
get_filename_component(_QURL_PLATFORM_PATH ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)
set(QURL_PLATFORM_PATH  "${_QURL_PLATFORM_PATH}" CACHE INTERNAL "qurl device path")

# Build configuration
## Do not build an executable; only build libraries
set(QURL_COMPILE_CFG_EXE OFF)
## Append the qurl library output directory
set(QURL_COMPILE_CFG_QURL_LIB_OUTPUT_DIRECTORY ${out_unir_lib_dir})

# Platform adapter layer
## Port source files
set(QURL_USER_PORT_SRC
    ${QURL_PLATFORM_PATH}/port/qurl_platform_port.c
    ${QURL_PLATFORM_PATH}/port/qurl_platform_tls_port.c
CACHE INTERNAL "qurl port src")

## Port include paths
set(QURL_USER_PORT_INC
    ${QURL_PLATFORM_PATH}/include
CACHE INTERNAL "qurl port include")

## Port library search paths
set(QURL_USER_PORT_LIBS_PATH
CACHE INTERNAL "qurl port libs path")

## Port libraries
set(QURL_USER_PORT_LIBS
CACHE INTERNAL "qurl port libs")

if(CONFIG_QOSA_LINK_SYSTEM_LIBRARIES_FUNC)
list(APPEND QURL_USER_PORT_LIBS
    system_os
)
endif()

if(CONFIG_QCM_FILE_API_FUNC)
list(APPEND QURL_USER_PORT_LIBS
    qcm_file
)
endif()

if(CONFIG_QCM_VTLS_FUNC)
list(APPEND QURL_USER_PORT_LIBS
    vtls
)
endif()
