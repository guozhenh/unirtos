set(target qurl)

add_library(${target} STATIC)

if(QURL_COMPILE_CFG_QURL_LIB_OUTPUT_DIRECTORY)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${QURL_COMPILE_CFG_QURL_LIB_OUTPUT_DIRECTORY})
endif() # QURL_COMPILE_CFG_QURL_LIB_OUTPUT_DIRECTORY
target_link_directories(${target} PUBLIC ${QURL_USER_PORT_LIBS_PATH})
target_link_libraries(${target} PUBLIC ${QURL_USER_PORT_LIBS})

target_include_directories(${target} PUBLIC ../include ./vauth ./include ./vtls/include ${QURL_USER_PORT_INC})

set(core_src
    ./qurl_slist.c
    ./qurl_list.c
    ./qurl_dbuf.c
    ./qurl_default_basic.c
    ./qurl_default_fs.c
    ./qurl_parsedate.c
    ./qurl_init.c
    ./qurl_select.c
    ./qurl_network.c
    ./qurl_ctype.c
    ./qurl_setopt.c
    ./qurl_getinfo.c
    ./qurl_transfer.c
    ./qurl_conn.c
    ./qurl_port.c
    ./qurl_url.c
    ./qurl_pingpong.c
    ./qurl_core.c
    ${QURL_USER_PORT_SRC}

    ./vtls/qurl_mbedtls.c
    ./vtls/qurl_tls.c

    ./qurl_http_chunked.c
    ./qurl_http_multiform.c
    ./qurl_http.c

    ./qurl_ftp.c

    ./vauth/qurl_cleartext.c
    ./vauth/qurl_oauth2.c
    ./qurl_sasl.c
    ./qurl_smtp.c

    ./qurl_test_unit.c
)

target_sources(${target} PRIVATE
    ${core_src}
)
