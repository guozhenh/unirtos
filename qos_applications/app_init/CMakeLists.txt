message(STATUS "cmake config ${CMAKE_CURRENT_SOURCE_DIR}")

set(target apps_init)
add_apps_libraries($<TARGET_FILE:${target}>)
add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_unir_lib_dir})
target_include_directories(${target} PRIVATE ${SOURCE_TOP_DIR})
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


target_sources(${target} PRIVATE
    "apps_init.c"
)

if(GENERATED_APP_INIT_ENTRIES_C AND EXISTS ${GENERATED_APP_INIT_ENTRIES_C})
    target_sources(${target} PRIVATE ${GENERATED_APP_INIT_ENTRIES_C})
endif()

target_link_libraries_if(CONFIG_QOSA_LINK_SYSTEM_LIBRARIES_FUNC THEN ${target} system_os)
target_link_libraries_if(CONFIG_QAPP_UNIRTOS_STD_FUNC THEN ${target} unirtos_std)
