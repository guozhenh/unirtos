/** @file         qurl_tls.c
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-11-09 20:15:15
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#include "qurl_config.h"
#include "qurl_code.h"
#include "qurl_def_in.h"
#include "qurl_log.h"

#include "qurl_tls_def.h"
#include "qurl_mbedtls.h"

#if QURL_CFG_ENABLE_TLS

/* API */
__attribute__((weak)) void qurl_tls_cfg_init(qurl_tls_cfg_t *tls_cfg_ptr)
{
    if (tls_cfg_ptr == QOSA_NULL)
    {
        return;
    }

    tls_cfg_ptr->version = QURL_TLS_VERSION_ALL;
    tls_cfg_ptr->ciphersuites = QOSA_NULL;
    tls_cfg_ptr->verify = QURL_TLS_VERIFY_NONE;
    tls_cfg_ptr->ca_cert_path_slist = QOSA_NULL;
    tls_cfg_ptr->own_cert_path_ptr = QOSA_NULL;
    tls_cfg_ptr->own_key_path_ptr = QOSA_NULL;
    tls_cfg_ptr->own_key_pwd_ptr = QOSA_NULL;
    tls_cfg_ptr->negotiate_timeout = QURL_CFG_TLS_HANDSHAKE_TIMEOUT_DEFAULT;

    tls_cfg_ptr->bits.sni_enable = QOSA_FALSE;
    tls_cfg_ptr->bits.session_match = QOSA_TRUE;
    tls_cfg_ptr->bits.session_share = QOSA_TRUE;
}

//TLS is enabled and not adapted by the platform side
#if (QURL_CFG_ENABLE_TLS_PLATFORM == 0)

/*Design a firmware that only supports one set of TLS.*/
const qurl_tls_adapter_t *g_tls_port_ptr =
#if (QURL_CFG_ENABLE_TLS_MBEDTLS)
    &g_tls_port_mbedtls;
#elif (QURL_CFG_ENABLE_TLS_OPENSSL)

#else
#error "Missing qurl_tls_adapter_t for selected TLS backend"
#endif

/*Internal Auxiliary*/

/**
 * @brief Copy: When encountering pointer parameters, new memory resources will be allocated for storage based on their meaning, making dst and src completely independent.
 *
 * @param src_ptr
 * @param dst_ptr
 * @return qurl_ecode_t
 */
static inline qurl_ecode_t tls_cfgdup(qurl_tls_cfg_t *src_ptr, qurl_tls_cfg_t *dst_ptr)
{
    if (src_ptr == QOSA_NULL || dst_ptr == QOSA_NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    dst_ptr->version = src_ptr->version;
    dst_ptr->verify = src_ptr->verify;
    dst_ptr->negotiate_timeout = src_ptr->negotiate_timeout;

    dst_ptr->bits.sni_enable = src_ptr->bits.sni_enable;
    dst_ptr->bits.session_match = src_ptr->bits.session_match;
    dst_ptr->bits.session_share = src_ptr->bits.session_share;

    return QURL_OK;
}

/* API */
/**
 * Belongs to the TLS interface layer, parameter validation is required.
 */

/** VTLS Interface*/
qurl_tls_ctrl_t *qurl_tls_new(qurl_tls_cfg_t *tls_cfg_ptr)
{
    qurl_tls_ctrl_t *tls_ctrl_ptr = QOSA_NULL;

    tls_ctrl_ptr = (qurl_tls_ctrl_t *)qurl_malloc(sizeof(qurl_tls_ctrl_t));
    if (tls_ctrl_ptr == QOSA_NULL)
    {
        return tls_ctrl_ptr;
    }
    qurl_memset(tls_ctrl_ptr, 0x00, sizeof(qurl_tls_ctrl_t));
    tls_ctrl_ptr->sock_fd = -1;

    tls_ctrl_ptr->tls_cfg_ptr = qurl_malloc(sizeof(qurl_tls_cfg_t));
    if (tls_ctrl_ptr->tls_cfg_ptr == QOSA_NULL)
    {
        qurl_free(tls_ctrl_ptr);
        tls_ctrl_ptr = QOSA_NULL;
        return tls_ctrl_ptr;
    }
    qurl_memset(tls_ctrl_ptr->tls_cfg_ptr, 0x00, sizeof(qurl_tls_cfg_t));

    tls_ctrl_ptr->tls_ptr = qurl_malloc(g_tls_port_ptr->tls_backend_ctrl_size);
    if (tls_ctrl_ptr->tls_ptr == QOSA_NULL)
    {
        qurl_free(tls_ctrl_ptr->tls_cfg_ptr);
        qurl_free(tls_ctrl_ptr);
        tls_ctrl_ptr = QOSA_NULL;
        return tls_ctrl_ptr;
    }
    qurl_memset(tls_ctrl_ptr->tls_ptr, 0x00, g_tls_port_ptr->tls_backend_ctrl_size);

    if (tls_cfg_ptr != QOSA_NULL)
    {
        tls_cfgdup(tls_cfg_ptr, tls_ctrl_ptr->tls_cfg_ptr);
    }
    else
    {
        qurl_tls_cfg_init(tls_ctrl_ptr->tls_cfg_ptr);
    }

    return tls_ctrl_ptr;
}

void qurl_tls_free(qurl_tls_ctrl_t *tls_ctrl_ptr)
{
    if (tls_ctrl_ptr)
    {
        qurl_safe_free(tls_ctrl_ptr->remote_host_name);
        qurl_safe_free(tls_ctrl_ptr->tls_ptr);
        qurl_safe_free(tls_ctrl_ptr->tls_cfg_ptr);
        qurl_safe_free(tls_ctrl_ptr);
    }
}

qurl_ecode_t qurl_tls_set_option(qurl_tls_ctrl_t *tls_ctrl_ptr, int opt_tag, void *opt_val)
{
    qurl_ecode_t ret = QURL_OK;

    if (tls_ctrl_ptr == QOSA_NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    switch (opt_tag)
    {
        case QURL_TLS_OPT_SOCK_FD: {
            int arg = 0;

            if (opt_val != QOSA_NULL)
            {
                arg = *(int *)opt_val;
            }

            tls_ctrl_ptr->sock_fd = arg;
        }
        break;
        case QURL_TLS_OPT_REMOTE_HOST_NAME: {
            char *name_ptr = QOSA_NULL;

            if (tls_ctrl_ptr->remote_host_name != QOSA_NULL)
            {
                qurl_free(tls_ctrl_ptr->remote_host_name);
                tls_ctrl_ptr->remote_host_name = QOSA_NULL;
            }

            if (opt_val != QOSA_NULL)
            {
                name_ptr = qurl_strdup((char *)opt_val);
                if (name_ptr != QOSA_NULL)
                {
                    tls_ctrl_ptr->remote_host_name = name_ptr;
                }
                else
                {
                    ret = QURL_ECODE_NO_MEMORY;
                }
            }
        }
        break;
        case QURL_TLS_OPT_REMOTE_PORT: {
            int arg = 0;

            if (opt_val != QOSA_NULL)
            {
                arg = *(int *)opt_val;
            }

            tls_ctrl_ptr->remote_port = arg;
        }
        break;
        case QURL_TLS_OPT_MATCH_REMOTE_PORT: {
            int arg = 0;

            if (opt_val != QOSA_NULL)
            {
                arg = *(int *)opt_val;
            }

            tls_ctrl_ptr->match_remote_port = arg;
        }
        break;
        default: {
            ret = QURL_ECODE_PARAM_INVALID;
        }
    }

    return ret;
}

/** Third-party TLS decoupling interface*/

qurl_ecode_t qurl_tls_init(void)
{
    qurl_ecode_t ret = 0;

    ret = g_tls_port_ptr->init_func();

    return ret;
}

qurl_ecode_t qurl_tls_deinit(void)
{
    qurl_ecode_t ret = 0;

    ret = g_tls_port_ptr->deinit_func();

    return ret;
}

long qurl_tls_version(char *buf, long size)
{
    qurl_ecode_t ret = 0;

    ret = g_tls_port_ptr->version_func(buf, size);

    return ret;
}

qurl_bit_t qurl_tls_data_pending(qurl_tls_ctrl_t *tls_ctrl_ptr)
{
    if (tls_ctrl_ptr == QOSA_NULL)
    {
        return QOSA_FALSE;
    }

    return g_tls_port_ptr->data_pending_func(tls_ctrl_ptr);
}

qurl_ecode_t qurl_tls_connect(qurl_tls_ctrl_t *tls_ctrl_ptr)
{
    qurl_ecode_t ret = 0;

    if (tls_ctrl_ptr == QOSA_NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    tls_ctrl_ptr->state = QURL_TLS_STATE_NEGOTIATING; /*Marking is under negotiation*/

    ret = g_tls_port_ptr->connect_func(tls_ctrl_ptr);

    return ret;
}

qurl_ecode_t qurl_tls_connect_nonblocking(qurl_tls_ctrl_t *tls_ctrl_ptr, qurl_bit_t *done_ptr)
{
    qurl_ecode_t ret = 0;

    if (tls_ctrl_ptr == QOSA_NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    tls_ctrl_ptr->state = QURL_TLS_STATE_NEGOTIATING; /*Marking is under negotiation*/

    ret = g_tls_port_ptr->connect_nonblocking_func(tls_ctrl_ptr, done_ptr);

    return ret;
}

qurl_ecode_t qurl_tls_close(qurl_tls_ctrl_t *tls_ctrl_ptr)
{
    qurl_ecode_t ret = 0;

    if (tls_ctrl_ptr == QOSA_NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    ret = g_tls_port_ptr->close_func(tls_ctrl_ptr);

    tls_ctrl_ptr->state = QURL_TLS_STATE_NONE; /*Done*/

    return ret;
}

long qurl_tls_read(qurl_tls_ctrl_t *tls_ctrl_ptr, unsigned char *buf, long buf_size, qurl_ecode_t *ecode_ptr)
{
    qurl_ecode_t ret = 0;

    if (tls_ctrl_ptr == QOSA_NULL)
    {
        *ecode_ptr = QURL_ECODE_PARAM_INVALID;
        return -1;
    }

    ret = g_tls_port_ptr->read_func(tls_ctrl_ptr, buf, buf_size, ecode_ptr);

    return ret;
}

long qurl_tls_write(qurl_tls_ctrl_t *tls_ctrl_ptr, unsigned char *buf, long buf_size, qurl_ecode_t *ecode_ptr)
{
    long len = 0;

    if (tls_ctrl_ptr == QOSA_NULL)
    {
        *ecode_ptr = QURL_ECODE_PARAM_INVALID;
        return -1;
    }

    /*[202402291600]Fixed the requirement for the tls_write interface to send data in rounds. The next round can only be sent after the current round is completed.*/
    if (tls_ctrl_ptr->cur_w_limit <= 0)
    {
        tls_ctrl_ptr->cur_w_limit = buf_size;
    }
    else
    {
        if (tls_ctrl_ptr->cur_w_limit > buf_size)
        {
            *ecode_ptr = QURL_ECODE_TLS_WRITE_LEN_ERR;
            return -1;
        }
        else if (tls_ctrl_ptr->cur_w_limit < buf_size)
        {
            buf_size = tls_ctrl_ptr->cur_w_limit;
        }
    }

    len = g_tls_port_ptr->write_func(tls_ctrl_ptr, buf, buf_size, ecode_ptr);

    /* [202402291600] */
    if (len > 0)
    {
        tls_ctrl_ptr->cur_w_limit -= len;
    }

    return len;
}

#endif /* QURL_CFG_ENABLE_TLS_PLATFORM == 0 */

#endif
