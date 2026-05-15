/** @file         qurl_https_app.c
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-11-14 13:36:36
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#include "qurl_http_app.h"

#ifdef QURL_CFG_APPS_HTTP

#define QURL_APPS_HTTP_LOG (1)

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

    (void)arg; /*Skip compilation warnings*/

    qosa_memcpy(g_buff, buf, log_len);
    g_buff[log_len] = 0;

    printf("%s", g_buff);

    return len;
}

qurl_ecode_t qurl_http_app_init(void)
{
    qurl_ecode_t   ret = QURL_OK;
    qurl_core_t    core = QOSA_NULL;
    qurl_tls_cfg_t tls_cfg = {0};

    ret = qurl_core_create(&core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    ret = qurl_core_setopt(core, QURL_OPT_URL, "https://www.example.com");

    ret = qurl_core_setopt(core, QURL_OPT_WRITE_CB, qurl_http_app_w_cb);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    qurl_tls_cfg_init(&tls_cfg);
    tls_cfg.negotiate_timeout = 30;

    ret = qurl_core_setopt(core, QURL_OPT_TLS_CFG, &tls_cfg);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    ret = qurl_core_perform(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    // qurl_slist_del_all(headers);

    ret = qurl_core_delete(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    return QURL_OK;
}

#endif /* QURL_CFG_APPS_HTTP */
