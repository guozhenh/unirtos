message(STATUS "cmake config ${CMAKE_CURRENT_SOURCE_DIR}")

set(target qcm_file)
add_components_libraries($<TARGET_FILE:${target}>)
add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_unir_lib_dir})
target_include_directories(${target} PRIVATE ${SOURCE_TOP_DIR})
target_include_directories(${target} PRIVATE ${SOURCE_SYS_DEBUG_DIR})
target_include_directories(${target} PRIVATE ${SOURCE_SYS_OS_DIR})
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)
target_include_directories(${target} PRIVATE ${SOURCE_TOP_DIR}/components/include)

target_add_sources(${target} PRIVATE
    qcm_file_api.c
 )


target_sources_if(CONFIG_QCM_FILE_SFS_FUNC THEN ${target} PRIVATE qcm_qsfs_api.c)
target_link_libraries_if(CONFIG_QCM_FILE_SFS_FUNC THEN ${target} utils)

target_link_libraries_if(CONFIG_QOSA_LINK_SYSTEM_LIBRARIES_FUNC THEN ${target} system_os)
target_link_libraries(${target} utf8)
