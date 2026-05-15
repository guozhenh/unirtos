message(STATUS "cmake config ${CMAKE_CURRENT_SOURCE_DIR}")

set(target vtls)
add_components_libraries($<TARGET_FILE:${target}>)
add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_unir_lib_dir})
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)
target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_add_sources(${target} PRIVATE
    mbedtls.c
    qcm_vtls.c
 )

target_link_libraries_if(CONFIG_QOSA_LINK_SYSTEM_LIBRARIES_FUNC THEN ${target} system_os)
target_link_libraries_if(CONFIG_QCM_FILE_API_FUNC THEN ${target} qcm_file)
target_link_libraries_if(CONFIG_QCM_MBEDTLS_LIBRARY_FUNC THEN ${target} mbedtls)
target_link_libraries_if(CONFIG_QCM_UTILS_UTILS_FUNC THEN ${target} utils)
