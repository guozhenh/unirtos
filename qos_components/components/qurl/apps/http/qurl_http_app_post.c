/** @file         qurl_http_app_post.c
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-10-30 21:06:36
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
static char data[] = "this is test for qurl\n";
static char data1[] = "zzb lzh djt jamie qurl\n";

struct qurl_app_r_buf_s
{
    char *buf_ptr;
    long  valid_len;
};
typedef struct qurl_app_r_buf_s qurl_app_r_buf_t;

static long qurl_http_app_w_cb(char *buf, long size, void *arg)
{
    long len = size;
    long log_len = len > 2048 ? 2048 : len;

    qosa_memcpy(g_buff, buf, log_len);
    g_buff[log_len] = 0;

    printf("%s", g_buff);

    return len;
}

static long qurl_http_app_r_cb(char *buf, long size, void *arg)
{
    qurl_app_r_buf_t *r_buf_ptr = (qurl_app_r_buf_t *)arg;

    if ((size) < 1)
    {
        return 0;
    }

    if (r_buf_ptr->valid_len)
    {
        *buf = r_buf_ptr->buf_ptr[0];
        r_buf_ptr->buf_ptr++;
        r_buf_ptr->valid_len--;
        return 1; /*Send byte by byte*/
    }

    return 0;
}

qurl_ecode_t qurl_http_app_init(void)
{
    qurl_ecode_t         ret = QURL_OK;
    qurl_core_t          core = QOSA_NULL;
    qurl_slist_t         headers = QOSA_NULL;
    qurl_app_r_buf_t     r_buf = {0};
    qurl_http_form_cfg_t form_cfg = {0};

    r_buf.buf_ptr = data;
    r_buf.valid_len = qosa_strlen(data);

    ret = qurl_core_create(&core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    ret = qurl_core_setopt(core, QURL_OPT_URL, "http://httpbin.org/post");
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    // headers = qurl_slist_add_strdup(headers, "Accept: application/json");
    // ret     = qurl_core_setopt(core, QURL_OPT_HTTP_HEADER, headers);
    // if (ret != QURL_OK)
    // {
    // 	qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    // }

    ret = qurl_core_setopt(core, QURL_OPT_HTTP_POST_FORM, 1L);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    form_cfg.name_ptr = "lzm";
    form_cfg.filename_ptr = "jamie";
    form_cfg.content_type = QURL_HTTP_FORM_CONTENT_DATA;
    form_cfg.content_ptr = "this is qurl by lzm!!!";
    form_cfg.content_len = qosa_strlen(form_cfg.content_ptr);

    ret = qurl_core_setopt(core, QURL_OPT_FORM, 1L, &form_cfg);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    form_cfg.name_ptr = "lzm3";
    form_cfg.filename_ptr = "jamie3";
    form_cfg.content_type = QURL_HTTP_FORM_CONTENT_FILE;
    form_cfg.content_ptr = "/home/jamie/work/unirtos/qurl/doc/test2.txt";
    form_cfg.content_len = qosa_strlen(form_cfg.content_ptr);

    ret = qurl_core_setopt(core, QURL_OPT_FORM, 3L, &form_cfg);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    // ret = qurl_core_setopt(core, QURL_OPT_HTTP_POST, 1L);
    // if (ret != QURL_OK)
    // {
    // 	qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    // }

    // ret = qurl_core_setopt(core, QURL_OPT_UPLOAD_DATA, data1);
    // if (ret != QURL_OK)
    // {
    // 	qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    // }

    // ret = qurl_core_setopt(core, QURL_OPT_UPLOAD_FILE, "/home/jamie/work/unirtos/qurl/doc/test2.txt");
    // if (ret != QURL_OK)
    // {
    // 	qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    // }

    // ret = qurl_core_setopt(core, QURL_OPT_UPLOAD_SIZE, r_buf.valid_len);
    // if (ret != QURL_OK)
    // {
    // 	qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    // }

    ret = qurl_core_setopt(core, QURL_OPT_READ_CB, qurl_http_app_r_cb);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    ret = qurl_core_setopt(core, QURL_OPT_READ_CB_ARG, &r_buf);
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

    // qurl_slist_del_all(headers);

    ret = qurl_core_delete(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    return QURL_OK;
}

#endif /* QURL_CFG_APPS_HTTP */
