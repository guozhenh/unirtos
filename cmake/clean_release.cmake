# clean_release.cmake
# This script requires variables to be passed in via -DOUTPUT_RELEASE_DIR and -Dbuild_version.

if(NOT DEFINED OUTPUT_RELEASE_DIR)
    message(FATAL_ERROR "OUTPUT_RELEASE_DIR is not defined!")
endif()
if(NOT DEFINED build_version)
    message(FATAL_ERROR "build_version is not defined!")
endif()

message(STATUS "Cleaning subdirectories under: ${OUTPUT_RELEASE_DIR}")

# Traverse all subdirectories (relative paths) under OUTPUT_RELEASE_DIR
file(GLOB SUBDIRS RELATIVE "${OUTPUT_RELEASE_DIR}" "${OUTPUT_RELEASE_DIR}/*")
foreach(subdir ${SUBDIRS})
    set(full_dir "${OUTPUT_RELEASE_DIR}/${subdir}")
    if(EXISTS "${full_dir}" AND IS_DIRECTORY "${full_dir}")
        if("${subdir}" STREQUAL "${build_version}")
            message(STATUS "Found build_version folder (${full_dir}); cleaning its content.")
            # For the build_version folder, delete its contents instead of the entire folder.
            file(GLOB build_items "${full_dir}/*")
            foreach(item ${build_items})
                execute_process(COMMAND "${CMAKE_COMMAND}" -E remove_directory "${item}"
                                RESULT_VARIABLE res)
                if(NOT res EQUAL 0)
                    message(WARNING "Unable to delete ${item}, it may be occupied, skipping.")
                else()
                    message(STATUS "delete ${item} success")
                endif()
            endforeach()
        else()
            message(STATUS "Attempting to remove directory: ${full_dir}")
            execute_process(COMMAND "${CMAKE_COMMAND}" -E remove_directory "${full_dir}"
                            RESULT_VARIABLE res)
            if(NOT res EQUAL 0)
                message(WARNING " Ubable to delete directory ${full_dir}, skipping.")
            else()
                message(STATUS "delete directory ${full_dir} success.")
            endif()
        endif()
    endif()
endforeach()
