message(STATUS "cmake config ${CMAKE_CURRENT_SOURCE_DIR}")

set(target qcm_uart_log)
add_components_libraries($<TARGET_FILE:${target}>)
add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_unir_lib_dir})
target_include_directories(${target} PRIVATE ${SOURCE_TOP_DIR})
target_include_directories(${target} PRIVATE ${SOURCE_SYS_DEBUG_DIR})
target_include_directories(${target} PRIVATE ${SOURCE_SYS_OS_DIR})
target_include_directories(${target} PRIVATE ${SOURCE_TOP_DIR}/components/include)

target_add_sources(${target} PRIVATE
    qcm_uart_log.c
)

target_link_libraries_if(CONFIG_QOSA_LINK_SYSTEM_LIBRARIES_FUNC THEN ${target} system_os)
