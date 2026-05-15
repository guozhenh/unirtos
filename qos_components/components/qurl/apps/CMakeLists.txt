set(target qurl_apps)

add_library(${target} STATIC)

target_link_libraries(${target} PUBLIC qurl)

# demo
target_include_directories(${target} PUBLIC ./include)
target_include_directories(${target} PUBLIC ../include)
target_include_directories(${target} PUBLIC ${QURL_PLATFORM_PATH}/include)

set(apps_src
    ./http/qurl_http_app.c
    ./http/qurl_http_app_auth.c
    ./http/qurl_http_app_get_redirect.c
    ./ftp/qurl_ftp_app.c
)

target_sources(${target} PRIVATE
    ${apps_src}
)

