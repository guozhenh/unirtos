/** @file         qurl_http_app_auth.c
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-10-28 21:06:36
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

qurl_ecode_t qurl_http_app_auth_init(void)
{
    qurl_ecode_t ret = QURL_OK;
    qurl_core_t  core = QOSA_NULL;

    ret = qurl_core_create(&core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    /*Authentication*/
    ret = qurl_core_setopt(core, QURL_OPT_URL, "http://www.httpbin.org/basic-auth/jamie/lzm");
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }
    ret = qurl_core_setopt(core, QURL_OPT_HTTP_AUTH, QURL_HTTP_AUTH_ONLY | QURL_HTTP_AUTH_BASIC);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }
    ret = qurl_core_setopt(core, QURL_OPT_USERNAME, "jamie");
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }
    ret = qurl_core_setopt(core, QURL_OPT_PASSWORD, "lzm");
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    ret = qurl_core_setopt(core, QURL_OPT_WRITE_CB, qurl_http_app_w_cb);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    ret = qurl_core_perform(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    ret = qurl_core_delete(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    return QURL_OK;
}

#endif /* QURL_CFG_APPS_HTTP */
