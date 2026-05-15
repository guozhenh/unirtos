/** @file         qurl_http_app_get_redirect.c
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-10-24 15:06:36
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#include "qurl_http_app.h"

#ifdef QURL_CFG_APPS_HTTP

#define QURL_APPS_HTTP_LOG (0)

#define qurl_app_log(debug, msg, ...)                                                                                                                          \
    do {                                                                                                                                                       \
        if ((debug) != 0)                                                                                                                                      \
        {                                                                                                                                                      \
            printf("%s(),%d:" msg, __FUNCTION__, __LINE__, ##__VA_ARGS__);                                                                                     \
        }                                                                                                                                                      \
    } while (0)

static char g_buff[2049] = {0};

static long qurl_http_app_w_cb(char *buf, long size, void *arg)
{
    long len = size;
    long log_len = len > 2048 ? 2048 : len;

    (void)arg;

    qosa_memcpy(g_buff, buf, log_len);
    g_buff[log_len] = 0;

    printf("%s", g_buff);

    return len;
}

qurl_ecode_t qurl_http_app_get_redirect_init(void)
{
    qurl_ecode_t ret = QURL_OK;
    qurl_core_t  core = QOSA_NULL;
    qurl_slist_t headers = QOSA_NULL;

    ret = qurl_core_create(&core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    ret = qurl_core_setopt(core, QURL_OPT_URL, "http://www.httpbin.org/absolute-redirect/1");
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    ret = qurl_core_setopt(core, QURL_OPT_WRITE_CB, qurl_http_app_w_cb);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    headers = qurl_slist_add_strdup(headers, "Host: httpbin.org");
    headers = qurl_slist_add_strdup(
        headers,
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/118.0.0.0 Safari/537.36"
    );
    headers = qurl_slist_add_strdup(headers, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8");
    headers = qurl_slist_add_strdup(headers, "Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2");
    headers = qurl_slist_add_strdup(headers, "Accept-Encoding: gzip, deflate");
    headers = qurl_slist_add_strdup(headers, "Connection: keep-alive");
    headers = qurl_slist_add_strdup(headers, "Upgrade-Insecure-Requests: 1");
    ret = qurl_core_setopt(core, QURL_OPT_HTTP_HEADER, headers);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    /*Supports redirection*/
    ret = qurl_core_setopt(core, QURL_OPT_FOLLOWLOCATION, 1);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    ret = qurl_core_perform(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    qurl_slist_del_all(headers);

    ret = qurl_core_delete(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    return QURL_OK;
}

#endif /* QURL_CFG_APPS_HTTP */
