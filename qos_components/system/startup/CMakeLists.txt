message(STATUS "cmake config ${CMAKE_CURRENT_SOURCE_DIR}")

set(target sys_startup)
add_apps_libraries(${target})
add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_unir_lib_dir})
target_include_directories(${target} PRIVATE ${SOURCE_TOP_DIR})
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


target_sources(${target} PRIVATE
    "unirtos_init.c"
 )
