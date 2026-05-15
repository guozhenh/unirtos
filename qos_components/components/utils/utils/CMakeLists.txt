message(STATUS "cmake config ${CMAKE_CURRENT_SOURCE_DIR}")

set(target utils)
add_components_libraries($<TARGET_FILE:${target}>)
add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_unir_lib_dir})
target_include_directories(${target} PRIVATE ${SOURCE_TOP_DIR})
target_include_directories(${target} PRIVATE ${SOURCE_SYS_DEBUG_DIR})
target_include_directories(${target} PRIVATE ${SOURCE_SYS_OS_DIR})
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)

target_sources(${target} PRIVATE
    qcm_utils.c
    qcm_sha1.c
    qcm_sha256.c
    qcm_aes.c
    qcm_md5.c
 )

target_link_libraries_if(CONFIG_QOSA_LINK_SYSTEM_LIBRARIES_FUNC THEN ${target} system_os)
target_link_libraries_if(CONFIG_QCM_MBEDTLS_LIBRARY_FUNC THEN ${target} mbedtls)
