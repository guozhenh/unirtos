message(STATUS "cmake config ${CMAKE_CURRENT_SOURCE_DIR}")

set(target socket_adapter)
add_components_libraries($<TARGET_FILE:${target}>)
add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_unir_lib_dir})
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)


if(CONFIG_QOSA_LINUX_PLATFORM_FUNC)
target_sources(${target} PRIVATE
    linux_socket_adp.c
)
else()
target_sources(${target} PRIVATE
    lwip_socket_adp.c
)
endif()

target_link_libraries_if(CONFIG_QOSA_LINK_SYSTEM_LIBRARIES_FUNC THEN ${target} system_os)
target_link_libraries_if(CONFIG_QCM_VTLS_FUNC THEN ${target} vtls)
