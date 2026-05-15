message("========add mbedtls ========")

if(CONFIG_QCM_MBEDTLS_4_LIBRARY_FUNC)
    add_subdirectory_if_exist(mbedtls4)
else()
    add_subdirectory_if_exist(mbedtls2)
endif()
