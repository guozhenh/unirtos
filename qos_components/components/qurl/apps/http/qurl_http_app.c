/** @file         qurl_http_app.c
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-05 21:06:36
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#include "qurl_http_app.h"

#include "unistd.h"

#if QURL_CFG_APPS_HTTP

#define QURL_APPS_HTTP_LOG           (1)
#define QURL_APPS_HTTP_TEST          (0)
#define QURL_APPS_HTTP_GET           (1)
#define QURL_APPS_HTTPS_GET          (0)
#define QURL_APPS_HTTP_HEAD          (0)
#define QURL_APPS_HTTP_GET_RAW_HEAD  (0)
#define QURL_APPS_HTTP_POST_RAW_HEAD (0)
#define QURL_APPS_HTTP_POST_FORM     (0)

#define qurl_app_log(debug, msg, ...)                                                                                                                          \
    do {                                                                                                                                                       \
        if ((debug) != 0)                                                                                                                                      \
        {                                                                                                                                                      \
            printf("%s(),%d:" msg, __FUNCTION__, __LINE__, ##__VA_ARGS__);                                                                                     \
        }                                                                                                                                                      \
    } while (0)

#define QURL_HTTP_APP_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define QURL_HTTP_APP_MIN(a, b) (((a) < (b)) ? (a) : (b))

struct qurl_app_r_buf_s
{
    char *buf_ptr;
    long  buf_len;
    long  r_index;
};
typedef struct qurl_app_r_buf_s qurl_app_r_buf_t;

static char g_buff[2049] = {0};

static long qurl_http_app_w_h_cb(char *buf, long size, void *arg)
{
    long len = size;
    long log_len = len > 2048 ? 2048 : len;

    (void)arg;

    qosa_memcpy(g_buff, buf, log_len);
    g_buff[log_len] = 0;

    printf("%s", g_buff);

    return len;
}

static long qurl_http_app_w_cb(char *buf, long size, void *arg)
{
    long len = size;
    long log_len = len > 2048 ? 2048 : len;

    (void)arg;

    qosa_memcpy(g_buff, buf, log_len);
    g_buff[log_len] = 0;

    // printf("%s", g_buff);

    printf("%d\r\n", size);

    // sleep(1);

    return len;
}

#if QURL_APPS_HTTP_TEST
static char data1[] = "zzb lzh djt jamie qurl\n";

qurl_ecode_t qurl_http_app_test(void)
{
    qurl_ecode_t ret = QURL_OK;
    qurl_core_t  core = QOSA_NULL;
    qurl_slist_t headers = QOSA_NULL;

    ret = qurl_core_create(&core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    ret = qurl_core_setopt(core, QURL_OPT_URL, "http://httpbin.org/put");
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    ret = qurl_core_setopt(core, QURL_OPT_WRITE_CB, qurl_http_app_w_cb);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    headers = qurl_slist_add_strdup(headers, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:102.0) Gecko/20100101 Firefox/102.0");
    headers = qurl_slist_add_strdup(headers, "Accept: application/json");
    headers = qurl_slist_add_strdup(headers, "Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2");
    headers = qurl_slist_add_strdup(headers, "Accept-Encoding: gzip, deflate");
    headers = qurl_slist_add_strdup(headers, "Connection: keep-alive");
    ret = qurl_core_setopt(core, QURL_OPT_HTTP_HEADER, headers);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    qurl_core_setopt(core, QURL_OPT_HTTP_PUT, 1L);
    qurl_core_setopt(core, QURL_OPT_UPLOAD_DATA, data1);
    qurl_core_setopt(core, QURL_OPT_UPLOAD_SIZE, qosa_strlen(data1));

    // ret = qurl_core_setopt(core, QURL_OPT_RESUME_FROM, 10L);
    // if (ret != QURL_OK)
    // {
    // 	qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    // }
    // ret = qurl_core_setopt(core, QURL_OPT_RANGE, "5-10, 20-30");
    // if (ret != QURL_OK)
    // {
    // 	qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    // }

    /*Supports redirection*/
    // ret = qurl_core_setopt(core, QURL_OPT_FOLLOWLOCATION, 1L);
    // if (ret != QURL_OK)
    // {
    // 	qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    // }

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
#endif /* QURL_APPS_HTTP_TEST */

#if QURL_APPS_HTTP_GET
qurl_ecode_t qurl_http_app_get(void)
{
    qurl_ecode_t ret = QURL_OK;
    qurl_core_t  core = QOSA_NULL;
    long         resp_code = 0;
    long         content_length = 0;
    qurl_slist_t headers = QOSA_NULL;

    ret = qurl_core_create(&core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    ret = qurl_core_setopt(core, QURL_OPT_URL, "183.240.98.161/");

    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }
    // qurl_core_setopt(core, QURL_OPT_HTTP_AUTH, QURL_HTTP_AUTH_BASIC);

    headers = qurl_slist_add_strdup(headers, "User-Agent: Quectel-Module");
    qurl_core_setopt(core, QURL_OPT_HTTP_HEADER, headers);

    qurl_core_setopt(core, QURL_OPT_FOLLOWLOCATION, 1L);

    qurl_core_setopt(core, QURL_OPT_WRITE_HEAD_CB, qurl_http_app_w_h_cb);
    qurl_core_setopt(core, QURL_OPT_WRITE_CB, qurl_http_app_w_cb);

    qurl_core_setopt(core, QURL_OPT_TIMEOUT_MS, (5 * 1000));

    ret = qurl_core_perform(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    qurl_core_getinfo(core, QURL_INFO_RESP_CODE, &resp_code);
    qurl_app_log(QURL_APPS_HTTP_LOG, "resp_code:[%d]\r\n", resp_code);

    qurl_core_getinfo(core, QURL_INFO_RESP_CONTENT_LENGTH, &content_length);
    qurl_app_log(QURL_APPS_HTTP_LOG, "content_length:[%d]\r\n", content_length);

    ret = qurl_core_delete(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    qurl_slist_del_all(headers);

    return QURL_OK;
}
#endif /* QURL_APPS_HTTP_GET */

#if QURL_APPS_HTTPS_GET
qurl_ecode_t qurl_https_app_get(void)
{
    qurl_ecode_t   ret = QURL_OK;
    qurl_core_t    core = QOSA_NULL;
    long           resp_code = 0;
    long           content_length = 0;
    qurl_tls_cfg_t tls_cfg = {0};

    ret = qurl_core_create(&core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    ret = qurl_core_setopt(core, QURL_OPT_URL, "https://112.31.84.164:8301/9X07-QAT/1K.txt");
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    qurl_core_setopt(core, QURL_OPT_WRITE_HEAD_CB, qurl_http_app_w_h_cb);
    qurl_core_setopt(core, QURL_OPT_WRITE_CB, qurl_http_app_w_cb);

    qurl_tls_cfg_init(&tls_cfg);
    tls_cfg.negotiate_timeout = 30;

    qurl_core_setopt(core, QURL_OPT_TLS_CFG, &tls_cfg);

    ret = qurl_core_perform(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    qurl_core_getinfo(core, QURL_INFO_RESP_CODE, &resp_code);
    qurl_app_log(QURL_APPS_HTTP_LOG, "resp_code:[%d]\r\n", resp_code);

    qurl_core_getinfo(core, QURL_INFO_RESP_CONTENT_LENGTH, &content_length);
    qurl_app_log(QURL_APPS_HTTP_LOG, "content_length:[%d]\r\n", content_length);

    ret = qurl_core_delete(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    return QURL_OK;
}
#endif /* QURL_APPS_HTTPS_GET */

#if QURL_APPS_HTTP_HEAD
qurl_ecode_t qurl_http_app_head(void)
{
    qurl_ecode_t ret = QURL_OK;
    qurl_core_t  core = QOSA_NULL;
    long         resp_code = 0;
    long         content_length = 0;
    qurl_slist_t headers = QOSA_NULL;

    ret = qurl_core_create(&core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    ret = qurl_core_setopt(core, QURL_OPT_URL, "http://112.31.84.164:8300");
    // ret = qurl_core_setopt(core, QURL_OPT_URL, "http://www.baidu.com/");
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    qurl_core_setopt(core, QURL_OPT_TIMEOUT_MS, 300 * 1000);
    qurl_core_setopt(core, QURL_OPT_NOBODY, 1L);
    qurl_core_setopt(core, QURL_OPT_FOLLOWLOCATION, 1L);

    headers = qurl_slist_add_strdup(headers, "Connection: close");
    qurl_core_setopt(core, QURL_OPT_HTTP_HEADER, headers);

    qurl_core_setopt(core, QURL_OPT_WRITE_HEAD_CB, qurl_http_app_w_h_cb);
    qurl_core_setopt(core, QURL_OPT_WRITE_CB, qurl_http_app_w_cb);

    ret = qurl_core_perform(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    qurl_core_getinfo(core, QURL_INFO_RESP_CODE, &resp_code);
    qurl_app_log(QURL_APPS_HTTP_LOG, "resp_code:[%d]\r\n", resp_code);

    qurl_core_getinfo(core, QURL_INFO_RESP_CONTENT_LENGTH, &content_length);
    qurl_app_log(QURL_APPS_HTTP_LOG, "content_length:[%d]\r\n", content_length);

    ret = qurl_core_delete(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    qurl_slist_del_all(headers);

    return QURL_OK;
}
#endif /* QURL_APPS_HTTP_HEAD */

#if QURL_APPS_HTTP_GET_RAW_HEAD
static long qurl_http_app_r_raw_head_cb(char *buf, long size, void *arg)
{
    qurl_app_r_buf_t *r_buf_ptr = (qurl_app_r_buf_t *)arg;

    if (size < 1)
    {
        return 0;
    }

    size = QURL_HTTP_APP_MIN(size, r_buf_ptr->buf_len - r_buf_ptr->r_index);
    qosa_memcpy(buf, r_buf_ptr->buf_ptr, size);
    r_buf_ptr->r_index += size;

    return size;
}

static char  head_data[] = "GET / HTTP/1.1\r\nHost: www.baidu.com\r\nAccept: */*\r\n\r\n";
qurl_ecode_t qurl_http_app_get_raw_head(void)
{
    qurl_ecode_t     ret = QURL_OK;
    qurl_core_t      core = QOSA_NULL;
    long             resp_code = 0;
    long             content_length = 0;
    qurl_app_r_buf_t r_head_buf = {0};

    r_head_buf.buf_ptr = head_data;
    r_head_buf.buf_len = qosa_strlen(head_data);
    r_head_buf.r_index = 0;

    ret = qurl_core_create(&core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    ret = qurl_core_setopt(core, QURL_OPT_URL, "http://www.baidu.com/");
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    qurl_core_setopt(core, QURL_OPT_READ_HEAD_CB, qurl_http_app_r_raw_head_cb);
    qurl_core_setopt(core, QURL_OPT_READ_HEAD_CB_ARG, &r_head_buf);

    qurl_core_setopt(core, QURL_OPT_WRITE_HEAD_CB, qurl_http_app_w_h_cb);
    qurl_core_setopt(core, QURL_OPT_WRITE_CB, qurl_http_app_w_cb);

    ret = qurl_core_perform(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    qurl_core_getinfo(core, QURL_INFO_RESP_CODE, &resp_code);
    qurl_app_log(QURL_APPS_HTTP_LOG, "resp_code:[%d]\r\n", resp_code);

    qurl_core_getinfo(core, QURL_INFO_RESP_CONTENT_LENGTH, &content_length);
    qurl_app_log(QURL_APPS_HTTP_LOG, "content_length:[%d]\r\n", content_length);

    ret = qurl_core_delete(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    return QURL_OK;
}
#endif /* QURL_APPS_HTTP_GET_RAW_HEAD */

#if QURL_APPS_HTTP_POST_RAW_HEAD
static long qurl_http_app_post_r_raw_head_cb(char *buf, long size, void *arg)
{
    qurl_app_r_buf_t *r_buf_ptr = (qurl_app_r_buf_t *)arg;

    if (size < 1)
    {
        return 0;
    }

    size = QURL_HTTP_APP_MIN(size, r_buf_ptr->buf_len - r_buf_ptr->r_index);
    qosa_memcpy(buf, r_buf_ptr->buf_ptr, size);
    r_buf_ptr->r_index += size;

    return size;
}

static char  head_data[] = "POST / HTTP/1.1\r\nHost: www.baidu.com\r\nContent-Length: 10\r\nAccept: */*\r\n\r\n";
static char  head_data_body[] = "1234567890";
static char  head_data_head_body[] = "POST / HTTP/1.1\r\nHost: www.baidu.com\r\nContent-Length: 10\r\nAccept: */*\r\n\r\n1234567890";
qurl_ecode_t qurl_http_app_post_raw_head(void)
{
    qurl_ecode_t     ret = QURL_OK;
    qurl_core_t      core = QOSA_NULL;
    long             resp_code = 0;
    long             content_length = 0;
    qurl_app_r_buf_t r_head_buf = {0};

    (void)head_data_body;
    (void)r_head_buf;
    (void)qurl_http_app_post_r_raw_head_cb;

    r_head_buf.buf_ptr = head_data;
    r_head_buf.buf_len = qosa_strlen(head_data);
    r_head_buf.r_index = 0;

    ret = qurl_core_create(&core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    ret = qurl_core_setopt(core, QURL_OPT_URL, "http://www.baidu.com/");
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    qurl_core_setopt(core, QURL_OPT_UPLOAD_HEAD_RAW, 1L);
    // qurl_core_setopt(core, QURL_OPT_READ_HEAD_CB, qurl_http_app_post_r_raw_head_cb);
    // qurl_core_setopt(core, QURL_OPT_READ_HEAD_CB_ARG, &r_head_buf);

    qurl_core_setopt(core, QURL_OPT_HTTP_POST, 1L);

    qurl_core_setopt(core, QURL_OPT_UPLOAD_DATA, head_data_head_body);
    qurl_core_setopt(core, QURL_OPT_UPLOAD_SIZE, qosa_strlen(head_data_head_body));
    //qurl_core_setopt(core, QURL_OPT_UPLOAD_DATA, head_data_body);
    //qurl_core_setopt(core, QURL_OPT_UPLOAD_SIZE, qosa_strlen(head_data_body));

    qurl_core_setopt(core, QURL_OPT_WRITE_HEAD_CB, qurl_http_app_w_h_cb);
    qurl_core_setopt(core, QURL_OPT_WRITE_CB, qurl_http_app_w_cb);

    ret = qurl_core_perform(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    qurl_core_getinfo(core, QURL_INFO_RESP_CODE, &resp_code);
    qurl_app_log(QURL_APPS_HTTP_LOG, "resp_code:[%d]\r\n", resp_code);

    qurl_core_getinfo(core, QURL_INFO_RESP_CONTENT_LENGTH, &content_length);
    qurl_app_log(QURL_APPS_HTTP_LOG, "content_length:[%d]\r\n", content_length);

    ret = qurl_core_delete(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    return QURL_OK;
}
#endif /* QURL_APPS_HTTP_POST_RAW_HEAD */

#if QURL_APPS_HTTP_POST_FORM
long qurl_http_app_r_form_cb(unsigned char *buf, long size, void *arg)
{
    qurl_app_r_buf_t *r_buf_ptr = (qurl_app_r_buf_t *)arg;

    //qurl_app_log(QURL_APPS_HTTP_LOG, "\r\n[%d]\r\n\r\n", size);

    if (size < 1)
    {
        return 0;
    }

    size = QURL_HTTP_APP_MIN(size, r_buf_ptr->buf_len - r_buf_ptr->r_index);
    qosa_memcpy(buf, r_buf_ptr->buf_ptr, size);
    r_buf_ptr->r_index += size;
    if (r_buf_ptr->r_index >= r_buf_ptr->buf_len)
    {
        r_buf_ptr->r_index = 0;
    }

    //qurl_app_log(QURL_APPS_HTTP_LOG, "\r\n[%d]\r\n\r\n", size);

    return size;
}

static char  form_data[] = "abcdefg";
qurl_ecode_t qurl_http_app_post_form(void)
{
    qurl_ecode_t         ret = QURL_OK;
    qurl_core_t          core = QOSA_NULL;
    long                 resp_code = 0;
    long                 content_length = 0;
    qurl_http_form_cfg_t form_cfg = {0};
    qurl_slist_t         form_headers = QOSA_NULL;
    qurl_app_r_buf_t     r_head_buf = {0};

    r_head_buf.buf_ptr = form_data;
    r_head_buf.buf_len = qosa_strlen(form_data);
    r_head_buf.r_index = 0;

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

    qurl_core_setopt(core, QURL_OPT_HTTP_POST_FORM, 1L);

    form_cfg.name_ptr = "lzm";
    form_cfg.filename_ptr = "jamie";
    form_cfg.content_type = QURL_HTTP_FORM_CONTENT_DATA;
    form_cfg.content_ptr = "this is qurl by lzm!!!";
    form_cfg.read_content_func = QOSA_NULL;
    form_cfg.content_len = qosa_strlen(form_cfg.content_ptr);
    qurl_core_setopt(core, QURL_OPT_FORM, 1L, &form_cfg);
    form_cfg.name_ptr = "lzm2";
    form_cfg.filename_ptr = "jamie2";
    form_cfg.content_type = QURL_HTTP_FORM_CONTENT_DATA;
    form_cfg.content_ptr = "this 123";
    form_cfg.read_content_func = QOSA_NULL;
    form_cfg.content_len = qosa_strlen(form_cfg.content_ptr);
    qurl_core_setopt(core, QURL_OPT_FORM, 2L, &form_cfg);
    form_cfg.name_ptr = "lzm1";
    form_cfg.filename_ptr = "jamie1";
    form_cfg.content_type = QURL_HTTP_FORM_CONTENT_CB;
    form_cfg.content_ptr = &r_head_buf;
    form_cfg.read_content_func = qurl_http_app_r_form_cb;
    form_cfg.content_len = r_head_buf.buf_len;
    form_headers = qurl_slist_add_strdup(form_headers, "Content-Type: 123");
    form_cfg.headers_slist = form_headers;
    qurl_core_setopt(core, QURL_OPT_FORM, 3L, &form_cfg);

    qurl_core_setopt(core, QURL_OPT_WRITE_HEAD_CB, qurl_http_app_w_h_cb);
    qurl_core_setopt(core, QURL_OPT_WRITE_CB, qurl_http_app_w_cb);

    ret = qurl_core_perform(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    qurl_core_perform(core);
    qurl_core_getinfo(core, QURL_INFO_RESP_CODE, &resp_code);
    qurl_app_log(QURL_APPS_HTTP_LOG, "resp_code:[%d]\r\n", resp_code);

    qurl_core_getinfo(core, QURL_INFO_RESP_CONTENT_LENGTH, &content_length);
    qurl_app_log(QURL_APPS_HTTP_LOG, "content_length:[%d]\r\n", content_length);

    qurl_slist_del_all(form_headers);
    form_headers = QOSA_NULL;

    ret = qurl_core_delete(core);
    if (ret != QURL_OK)
    {
        qurl_app_log(QURL_APPS_HTTP_LOG, "%x\r\n", ret);
    }

    return QURL_OK;
}
#endif /* QURL_APPS_HTTP_POST_RAW_HEAD */

qurl_ecode_t qurl_http_app_init(void)
{
    qurl_ecode_t ret = QURL_OK;

#if QURL_APPS_HTTP_TEST
    ret = qurl_http_app_test();
#endif
#if QURL_APPS_HTTP_GET
    ret = qurl_http_app_get();
#endif
#if QURL_APPS_HTTPS_GET
    ret = qurl_https_app_get();
#endif
#if QURL_APPS_HTTP_HEAD
    ret = qurl_http_app_head();
#endif
#if QURL_APPS_HTTP_GET_RAW_HEAD
    ret = qurl_http_app_get_raw_head();
#endif
#if QURL_APPS_HTTP_POST_RAW_HEAD
    ret = qurl_http_app_post_raw_head();
#endif
#if QURL_APPS_HTTP_POST_FORM
    ret = qurl_http_app_post_form();
#endif

    return ret;
}

#endif /* QURL_CFG_APPS_HTTP */
