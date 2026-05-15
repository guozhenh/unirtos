/** @file         qurl_mbedtls.c
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
#include "qurl_def_basic.h"
#include "qurl_def_in.h"
#include "qurl_log.h"
#include "qurl_port.h"
#include "qurl_select.h"

#include "qurl_tls_def.h"
#include "qurl_tls.h"

#if (QURL_CFG_ENABLE_TLS && QURL_CFG_ENABLE_TLS_MBEDTLS)

/* mbedtls */
#include <mbedtls/version.h>
#if MBEDTLS_VERSION_NUMBER >= 0x02040000
#include <mbedtls/net_sockets.h>
#else
#include <mbedtls/net.h>
#endif
#include <mbedtls/ssl.h>
#include <mbedtls/x509.h>
#include <mbedtls/error.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/sha256.h>
#include <mbedtls/debug.h>

/*Internal Data*/
/**
 * @struct qurl_tls_backend_ctrl_t
 * @brief tls backend control block
 */
struct qurl_tls_backend_ctrl_s
{
    mbedtls_ctr_drbg_context ctr_drbg; /*!< Pseudo-random number generator*/
    mbedtls_entropy_context  entropy;  /*!< Entropy source context*/
    mbedtls_ssl_context      ssl;      /*!< Context for SSL/TLS connection*/
    mbedtls_x509_crt         cacert;   /*!< X.509 certificate object, trusted CA*/
    mbedtls_x509_crt         clicert;  /*!< X.509 certificate object, client certificate*/
    mbedtls_pk_context       pk;       /*!< Public key context*/
    mbedtls_ssl_config       config;   /*!< Configuration for SSL/TLS connection*/
};
typedef struct qurl_tls_backend_ctrl_s qurl_tls_backend_ctrl_t;

/**
 * @struct qurl_tls_session_ctrl_bits_t
 * @brief 。
 */
struct qurl_tls_session_ctrl_bits_s
{
    qurl_bit_t invalid; /*!< Invalid state*/
};
typedef struct qurl_tls_session_ctrl_bits_s qurl_tls_session_ctrl_bits_t;

/**
 * @struct qurl_tls_session_ctrl_t
 * @brief tls session control block
 */
struct qurl_tls_session_ctrl_s
{
    qurl_list_node_t node; /*!< Linked list node*/

    qurl_mutex_t lock;     /*!< Global lock*/

    /*Matching Information*/
    char *host_name; /*!< Host*/
    int   port;      /*!< Port. -1 is invalid.*/

    /* session id */
    mbedtls_ssl_session session; /*!< SSL/TLS Session*/

    /*Status Information*/
    int used_cnt; /*!< Citation count*/

    qurl_tls_session_ctrl_bits_t bits;
};
typedef struct qurl_tls_session_ctrl_s qurl_tls_session_ctrl_t;

/**
 * @struct qurl_tls_sessions_g_ctrl_bits_t
 * @brief 。
 */
struct qurl_tls_sessions_g_ctrl_bits_s
{
    qurl_bit_t inited; /*!< Whether it has been initialized*/
};
typedef struct qurl_tls_sessions_g_ctrl_bits_s qurl_tls_sessions_g_ctrl_bits_t;

/**
 * @struct qurl_tls_sessions_g_ctrl_s
 * @brief All TLS session management
 */
struct qurl_tls_sessions_g_ctrl_s
{
    qurl_mutex_t lock;         /*!< Global lock*/

    int         max_cnt;       /*!< Maximum allowed valid cnt*/
    qurl_list_t sessions_list; /*!< All sessions*/

    qurl_tls_sessions_g_ctrl_bits_t bits;
};
typedef struct qurl_tls_sessions_g_ctrl_s qurl_tls_sessions_g_ctrl_t;

static qurl_tls_sessions_g_ctrl_t g_sessions_g_ctrl = {0};

/*Internal Auxiliary*/
/** Session maintenance*/
static qurl_tls_session_ctrl_t *mbed_session_new(qurl_tls_ctrl_t *tls_ctrl_ptr)
{
    int                      ret = 0;
    qurl_tls_session_ctrl_t *tls_session_ctrl_ptr = QOSA_NULL;
    qurl_tls_backend_ctrl_t *mbed_ctrl_ptr = (qurl_tls_backend_ctrl_t *)tls_ctrl_ptr->tls_ptr;

    tls_session_ctrl_ptr = qurl_malloc(sizeof(qurl_tls_session_ctrl_t));
    if (tls_session_ctrl_ptr == QOSA_NULL)
    {
        return QOSA_NULL;
    }
    qurl_memset(tls_session_ctrl_ptr, 0x00, sizeof(qurl_tls_session_ctrl_t));

    tls_session_ctrl_ptr->host_name = qurl_strdup(tls_ctrl_ptr->remote_host_name);
    if (tls_session_ctrl_ptr->host_name == QOSA_NULL)
    {
        qurl_safe_free(tls_session_ctrl_ptr);
        return QOSA_NULL;
    }
    tls_session_ctrl_ptr->port = tls_ctrl_ptr->match_remote_port;

    ret = qurl_mutex_create(&tls_session_ctrl_ptr->lock);
    if (ret != QURL_OK)
    {
        qurl_safe_free(tls_session_ctrl_ptr->host_name);
        qurl_safe_free(tls_session_ctrl_ptr);
        return QOSA_NULL;
    }
    qurl_list_node_init(&(tls_session_ctrl_ptr->node));
    mbedtls_ssl_session_init(&(tls_session_ctrl_ptr->session));

    ret = mbedtls_ssl_get_session(&(mbed_ctrl_ptr->ssl), &(tls_session_ctrl_ptr->session));
    if (ret != 0)
    {
        qurl_log(QURL_CFG_LOG_TLS, "failed\n  ! mbedtls_ssl_get_session() returned -%#02x\n", (unsigned)-ret);
        mbedtls_ssl_session_free(&(tls_session_ctrl_ptr->session));
        qurl_mutex_delete(tls_session_ctrl_ptr->lock);
        qurl_safe_free(tls_session_ctrl_ptr->host_name);
        qurl_safe_free(tls_session_ctrl_ptr);
        return QOSA_NULL;
    }

    return tls_session_ctrl_ptr;
}

static int mbed_session_del(qurl_tls_session_ctrl_t *session_ctrl_ptr)
{
    qurl_mutex_lock(g_sessions_g_ctrl.lock, QURL_WAIT_FOREVER);
    qurl_mutex_lock(session_ctrl_ptr->lock, QURL_WAIT_FOREVER);

    if (session_ctrl_ptr->used_cnt > 0)
    {
        session_ctrl_ptr->bits.invalid = QOSA_TRUE;
        qurl_mutex_unlock(session_ctrl_ptr->lock);
    }
    else
    {
        session_ctrl_ptr->bits.invalid = QOSA_FALSE;

        if (session_ctrl_ptr->host_name)
        {
            qurl_free(session_ctrl_ptr->host_name);
        }

        qurl_list_node_delete(&(g_sessions_g_ctrl.sessions_list), &(session_ctrl_ptr->node));
        mbedtls_ssl_session_free(&(session_ctrl_ptr->session));
        qurl_mutex_unlock(session_ctrl_ptr->lock);
        qurl_mutex_delete(session_ctrl_ptr->lock);

        qurl_free(session_ctrl_ptr);
    }

    qurl_mutex_unlock(g_sessions_g_ctrl.lock);

    return QURL_OK;
}

static qurl_tls_session_ctrl_t *mbed_session_alloc(const char *host_name, int port)
{
    qurl_tls_session_ctrl_t *session_ctrl_ptr = QOSA_NULL;
    qurl_list_node_t        *node_ptr = QOSA_NULL;

    qurl_mutex_lock(g_sessions_g_ctrl.lock, QURL_WAIT_FOREVER);
    node_ptr = (qurl_list_node_t *)qurl_list_check_head(&g_sessions_g_ctrl.sessions_list);
    while (node_ptr)
    {
        session_ctrl_ptr = qurl_list_node_entry(node_ptr, qurl_tls_session_ctrl_t, node);

        qurl_mutex_lock(session_ctrl_ptr->lock, QURL_WAIT_FOREVER);
        if (qurl_strncasecmp(session_ctrl_ptr->host_name, host_name, qurl_strlen(host_name)) == 0 && session_ctrl_ptr->port == port
            && session_ctrl_ptr->bits.invalid == QOSA_FALSE)
        {
            session_ctrl_ptr->used_cnt++;
            break;
        }
        qurl_mutex_unlock(session_ctrl_ptr->lock);
        session_ctrl_ptr = QOSA_NULL;

        //Continue
        node_ptr = (qurl_list_node_t *)qurl_list_check_next(&g_sessions_g_ctrl.sessions_list, node_ptr);
    }

    qurl_mutex_unlock(g_sessions_g_ctrl.lock);
    return session_ctrl_ptr;
}

static int mbed_session_free(qurl_tls_session_ctrl_t *session_ctrl_ptr)
{
    qurl_mutex_lock(session_ctrl_ptr->lock, QURL_WAIT_FOREVER);

    if (session_ctrl_ptr->used_cnt > 0)
    {
        session_ctrl_ptr->used_cnt--;
    }

    if (session_ctrl_ptr->used_cnt == 0 && session_ctrl_ptr->bits.invalid == QOSA_TRUE)
    {
        qurl_mutex_unlock(session_ctrl_ptr->lock);
        mbed_session_del(session_ctrl_ptr);
    }
    else
    {
        qurl_mutex_unlock(session_ctrl_ptr->lock);
    }

    return QURL_OK;
}

static void mbed_sessions_add(qurl_tls_session_ctrl_t *new_session_ctrl_ptr)
{
    qurl_bit_t               add_ok = QOSA_FALSE;
    int                      cur_cnt = 0;
    qurl_tls_session_ctrl_t *session_ctrl_ptr = QOSA_NULL;
    qurl_list_node_t        *node_ptr = QOSA_NULL;

    /*1. First, check if it exists*/
    qurl_mutex_lock(g_sessions_g_ctrl.lock, QURL_WAIT_FOREVER);
    node_ptr = (qurl_list_node_t *)qurl_list_check_head(&g_sessions_g_ctrl.sessions_list);
    while (node_ptr)
    {
        session_ctrl_ptr = qurl_list_node_entry(node_ptr, qurl_tls_session_ctrl_t, node);

        qurl_mutex_lock(session_ctrl_ptr->lock, QURL_WAIT_FOREVER);
        if (qurl_strncasecmp(session_ctrl_ptr->host_name, new_session_ctrl_ptr->host_name, qurl_strlen(new_session_ctrl_ptr->host_name)) == 0
            && session_ctrl_ptr->port == new_session_ctrl_ptr->port)
        {
            qurl_mutex_unlock(session_ctrl_ptr->lock);
            mbed_session_del(session_ctrl_ptr);
            qurl_list_put_head(&(g_sessions_g_ctrl.sessions_list), &(new_session_ctrl_ptr->node)); /*Push to the head for quick access*/
            add_ok = QOSA_TRUE;
            break;
        }
        qurl_mutex_unlock(session_ctrl_ptr->lock);

        //Continue
        node_ptr = (qurl_list_node_t *)qurl_list_check_next(&g_sessions_g_ctrl.sessions_list, node_ptr);
    }

    if (!add_ok)
    {
        cur_cnt = qurl_list_cnt(&g_sessions_g_ctrl.sessions_list);
        if (cur_cnt >= g_sessions_g_ctrl.max_cnt)
        {
            node_ptr = (qurl_list_node_t *)qurl_list_check_tail(&g_sessions_g_ctrl.sessions_list);
            qurl_list_node_delete(&(g_sessions_g_ctrl.sessions_list), node_ptr);
            mbed_session_del(session_ctrl_ptr);
        }
        qurl_list_put_head(&(g_sessions_g_ctrl.sessions_list), &(new_session_ctrl_ptr->node)); /*Push to the head for quick access*/
        add_ok = QOSA_TRUE;
    }
    qurl_mutex_unlock(g_sessions_g_ctrl.lock);
}

/** Network IO Assistant*/
static int mbed_bio_write(void *arg, const unsigned char *buf, qosa_size_t len)
{
    int              ret = 0;
    int              err = 0;
    qurl_tls_ctrl_t *tls_ctrl_ptr = (qurl_tls_ctrl_t *)arg;

    ret = qurl_socket_write(tls_ctrl_ptr->sock_fd, buf, len);

    if (ret < 0)
    {
        err = qurl_socket_get_errno(tls_ctrl_ptr->sock_fd);
        if ((err == EINPROGRESS) || (err == EWOULDBLOCK))
        {
            ret = MBEDTLS_ERR_SSL_WANT_WRITE;
        }
        else
        {
            ret = MBEDTLS_ERR_NET_CONN_RESET;
        }
    }

    return ret;
}

static int mbed_bio_read(void *arg, unsigned char *buf, qosa_size_t len)
{
    int              ret = 0;
    int              err = 0;
    qurl_tls_ctrl_t *tls_ctrl_ptr = (qurl_tls_ctrl_t *)arg;

    ret = qurl_socket_read(tls_ctrl_ptr->sock_fd, buf, len);

    if (ret < 0)
    {
        err = qurl_socket_get_errno(tls_ctrl_ptr->sock_fd);
        if ((err == EINPROGRESS) || (err == EWOULDBLOCK))
        {
            ret = MBEDTLS_ERR_SSL_WANT_READ;
        }
        else
        {
            ret = MBEDTLS_ERR_NET_CONN_RESET;
        }
    }

    return ret;
}

/** Adapter Layer Interface Assistant*/
#if (QURL_CFG_LOG_TLS == 2)
static void mbed_log(void *ctx, int level, const char *file, int line, const char *str)
{
    (void)ctx;   /*Skip compilation warnings*/
    (void)level; /*Skip compilation warnings*/

    qurl_log(QURL_CFG_LOG_TLS, "\r\nfile,line:[%s][%d]\r\n%s\r\n", file, line, str);
}
#endif

/**
 * @brief Initialize and configure TLS connection
 *
 * @param tls_ctrl_ptr
 * @return qurl_ecode_t
 */
static qurl_ecode_t mbed_connect_step1(qurl_tls_ctrl_t *tls_ctrl_ptr)
{
    qurl_ecode_t ret = 0;

    qurl_tls_cfg_t          *mbed_cfg_ptr = (qurl_tls_cfg_t *)tls_ctrl_ptr->tls_cfg_ptr;
    qurl_tls_backend_ctrl_t *mbed_ctrl_ptr = (qurl_tls_backend_ctrl_t *)tls_ctrl_ptr->tls_ptr;

    /*Random number*/
    /** Pseudo-random number*/
    mbedtls_ctr_drbg_init(&mbed_ctrl_ptr->ctr_drbg);
    /** Entropy Source*/
    mbedtls_entropy_init(&mbed_ctrl_ptr->entropy);

    /** Set random number seed*/
    ret = mbedtls_ctr_drbg_seed(&mbed_ctrl_ptr->ctr_drbg, mbedtls_entropy_func, &mbed_ctrl_ptr->entropy, QOSA_NULL, 0);
    if (ret != 0)
    {
        qurl_log(QURL_CFG_LOG_TLS, "random failed:[-0x%x]", -ret);
        return QURL_ECODE_TLS_RANDOM_ERR;
    }

    /*Certificate Related*/
    /** CA Root Certificate*/
    mbedtls_x509_crt_init(&mbed_ctrl_ptr->cacert);
    /** Client digital certificate*/
    mbedtls_x509_crt_init(&mbed_ctrl_ptr->clicert);
    /** Client private key*/
    mbedtls_pk_init(&mbed_ctrl_ptr->pk);

    /*TLS Configuration*/
    /** Initialize TLS configuration*/
    mbedtls_ssl_config_init(&mbed_ctrl_ptr->config);
    /** Set some default values for this configuration: [lzm][todo]Currently set as TCP client; if DTLS or server is needed, modify here during development.*/
    ret = mbedtls_ssl_config_defaults(&mbed_ctrl_ptr->config, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
    if (ret != 0)
    {
        qurl_log(QURL_CFG_LOG_TLS, "config failed:[-0x%x]", -ret);
        return QURL_ECODE_TLS_CONFIG_ERR;
    }

    /*Process according to verification mode*/
    if (mbed_cfg_ptr->verify == QURL_TLS_VERIFY_NONE)
    {
        /*No authentication*/
        mbedtls_ssl_conf_authmode(&mbed_ctrl_ptr->config, MBEDTLS_SSL_VERIFY_NONE);
    }
    else
    {
        /*Authentication required*/
        mbedtls_ssl_conf_authmode(&mbed_ctrl_ptr->config, MBEDTLS_SSL_VERIFY_REQUIRED);

        /*Load certificate chain*/
        /** [lzm][todo]Traverse and load certificate buffer*/
        /** [lzm][todo]Traverse and load certificate files*/
    }

    /*Initialize ssl/tls context*/
    mbedtls_ssl_init(&mbed_ctrl_ptr->ssl);
    /*Binding Configuration*/
    if (mbedtls_ssl_setup(&mbed_ctrl_ptr->ssl, &mbed_ctrl_ptr->config) != 0)
    {
        qurl_log(QURL_CFG_LOG_TLS, "setup failed:[%x]", ret);
        return QURL_ECODE_TLS_CONNECT_ERR;
    }

    /*Certificate Security Configuration*/
    /** new profile with RSA min key len = 1024 ... */
    // mbedtls_ssl_conf_cert_profile(&mbed_ctrl_ptr->config, &mbedtls_x509_crt_profile_fr);

    /*Protocol Version*/
    switch (mbed_cfg_ptr->version)
    {
        case QURL_TLS_VERSION_DEFAULT:
        case QURL_TLS_VERSION_SSL3_0:
        case QURL_TLS_VERSION_TLS1_0:
        case QURL_TLS_VERSION_TLS1_1:
        case QURL_TLS_VERSION_TLS1_2: {
            mbedtls_ssl_conf_min_tls_version(&mbed_ctrl_ptr->config, MBEDTLS_SSL_VERSION_TLS1_2);
            mbedtls_ssl_conf_max_tls_version(&mbed_ctrl_ptr->config, MBEDTLS_SSL_VERSION_TLS1_2);
            break;
        }
        case QURL_TLS_VERSION_TLS1_3: {
            mbedtls_ssl_conf_min_tls_version(&mbed_ctrl_ptr->config, MBEDTLS_SSL_VERSION_TLS1_3);
            mbedtls_ssl_conf_max_tls_version(&mbed_ctrl_ptr->config, MBEDTLS_SSL_VERSION_TLS1_3);
            break;
        }
        case QURL_TLS_VERSION_ALL: {
            mbedtls_ssl_conf_min_tls_version(&mbed_ctrl_ptr->config, MBEDTLS_SSL_VERSION_TLS1_2);
            mbedtls_ssl_conf_max_tls_version(&mbed_ctrl_ptr->config, MBEDTLS_SSL_VERSION_TLS1_3);
            break;
        }
        default:
            break;
    }

    /*Bind random number generator*/
    mbedtls_ssl_conf_rng(&mbed_ctrl_ptr->config, mbedtls_ctr_drbg_random, &mbed_ctrl_ptr->ctr_drbg);

    /*Ignore user-specified certificate errors*/
    // mbedtls_ssl_conf_verify(&mbed_ctrl_ptr->config, mbed_ssl_verify, &mbed_ctrl_ptr->ignore_certiem);

    /*Bind network IO*/
    /** [lzm][todo]Need to support distinguishing between blocking and non-blocking*/
    mbedtls_ssl_set_bio(&mbed_ctrl_ptr->ssl, tls_ctrl_ptr, mbed_bio_write, mbed_bio_read, QOSA_NULL);

    /*Bind supported encryption suites*/
    if (mbed_cfg_ptr->ciphersuites == QOSA_NULL)
    {
        mbedtls_ssl_conf_ciphersuites(&mbed_ctrl_ptr->config, mbedtls_ssl_list_ciphersuites());
    }
    else
    {
        mbedtls_ssl_conf_ciphersuites(&mbed_ctrl_ptr->config, mbed_cfg_ptr->ciphersuites);
    }

    /* session */
    if (mbed_cfg_ptr->bits.session_match && tls_ctrl_ptr->remote_host_name != QOSA_NULL)
    {
        /*Match*/
        qurl_tls_session_ctrl_t *tls_session_ctrl_ptr = QOSA_NULL;

        if (tls_ctrl_ptr->match_remote_port > 0 && tls_ctrl_ptr->match_remote_port < 65536)
        {
            tls_session_ctrl_ptr = mbed_session_alloc(tls_ctrl_ptr->remote_host_name, tls_ctrl_ptr->match_remote_port);
        }

        if (tls_session_ctrl_ptr != QOSA_NULL)
        {
            qurl_mutex_lock(tls_session_ctrl_ptr->lock, QURL_WAIT_FOREVER);
            mbedtls_ssl_set_session(&mbed_ctrl_ptr->ssl, &tls_session_ctrl_ptr->session);
            qurl_mutex_unlock(tls_session_ctrl_ptr->lock);
            if (ret != 0)
            {
                qurl_log(QURL_CFG_LOG_TLS, " failed\n  ! mbedtls_ssl_set_session returned -0x%x\n\n", (unsigned int)-ret);
                ret = QURL_ECODE_TLS_SET_SESSION_FAILED;
            }
            mbed_session_free(tls_session_ctrl_ptr);
        }
    }

    /*log debugging*/
#if (QURL_CFG_LOG_TLS == 2)
    mbedtls_debug_set_threshold(5);
    mbedtls_ssl_conf_dbg(&mbed_ctrl_ptr->config, mbed_log, QOSA_NULL);
#endif

    /*Under negotiation*/
    tls_ctrl_ptr->backend_state = QURL_TLS_BACKEND_STATE_CONN_2;

    return QURL_OK;
}

/**
 * @brief Handshake
 *
 * @param tls_ctrl_ptr
 * @return qurl_ecode_t
 */
static qurl_ecode_t mbed_connect_step2(qurl_tls_ctrl_t *tls_ctrl_ptr)
{
    qurl_ecode_t             ret = 0;
    qurl_tls_backend_ctrl_t *mbed_ctrl_ptr = (qurl_tls_backend_ctrl_t *)tls_ctrl_ptr->tls_ptr;

    /*Handshake*/
    ret = mbedtls_ssl_handshake(&mbed_ctrl_ptr->ssl);
    if (ret == MBEDTLS_ERR_SSL_WANT_READ)
    {
        tls_ctrl_ptr->backend_state = QURL_TLS_BACKEND_STATE_CONN_2_READING;
        return QURL_OK;
    }
    else if (ret == MBEDTLS_ERR_SSL_WANT_WRITE)
    {
        tls_ctrl_ptr->backend_state = QURL_TLS_BACKEND_STATE_CONN_2_WRITING;
        return QURL_OK;
    }
    else if (ret)
    {
        qurl_log(QURL_CFG_LOG_TLS, "mbedtls_ssl_handshake returned -0x%x\r\n", -ret);
        return QURL_ECODE_TLS_CONNECT_ERR;
    }

    tls_ctrl_ptr->backend_state = QURL_TLS_BACKEND_STATE_CONN_3;

    return QURL_OK;
}

static qurl_ecode_t mbed_connect_step3(qurl_tls_ctrl_t *tls_ctrl_ptr)
{
    qurl_tls_cfg_t          *mbed_cfg_ptr = (qurl_tls_cfg_t *)tls_ctrl_ptr->tls_cfg_ptr;
    qurl_tls_backend_ctrl_t *mbed_ctrl_ptr = (qurl_tls_backend_ctrl_t *)tls_ctrl_ptr->tls_ptr;

    (void)mbed_ctrl_ptr;

    /*[lzm][todo]session cache operations*/
    if (mbed_cfg_ptr->bits.session_share && tls_ctrl_ptr->remote_host_name != QOSA_NULL
        && (tls_ctrl_ptr->match_remote_port > 0 && tls_ctrl_ptr->match_remote_port < 65536))
    {
        qurl_tls_session_ctrl_t *tls_session_ctrl_ptr = QOSA_NULL;
        /*Save session*/
        tls_session_ctrl_ptr = mbed_session_new(tls_ctrl_ptr);
        if (tls_session_ctrl_ptr != QOSA_NULL)
        {
            mbed_sessions_add(tls_session_ctrl_ptr);
        }
    }

    tls_ctrl_ptr->backend_state = QURL_TLS_BACKEND_STATE_CONN_DONE;

    return QURL_OK;
}

/**
 * @brief General connection, supporting both blocking and non-blocking modes
 *
 * @param[in] tls_ctrl_ptr
 * @param[in] noblocking
 * @param[out] done_ptr
 * @return qurl_ecode_t
 */
static qurl_ecode_t mbed_connect(qurl_tls_ctrl_t *tls_ctrl_ptr, qurl_bit_t noblocking, qurl_bit_t *done_ptr)
{
    qurl_ecode_t    ret = 0;
    qurl_timeval_t  now_timeval = {0};
    qurl_time_t     timeout_ms = 0;
    qurl_tls_cfg_t *cfg_ptr = (qurl_tls_cfg_t *)tls_ctrl_ptr->tls_cfg_ptr;
    fd_set          fds_read = {0};
    fd_set          fds_write = {0};
    fd_set          fds_err = {0};
    qurl_socket_t   max_fd = -1;

    if (tls_ctrl_ptr->state == QURL_TLS_STATE_COMPLETE)
    {
        /*Already connected, no need to reconnect.*/
        *done_ptr = QOSA_TRUE;
        return QURL_OK;
    }

    /*Timer*/
    if (tls_ctrl_ptr->start_timeval.s == 0 && tls_ctrl_ptr->start_timeval.ms == 0)
    {
        tls_ctrl_ptr->start_timeval = qurl_time_now();
    }

    /*1. Start the TLS connection initialization process*/
    if (tls_ctrl_ptr->backend_state == QURL_TLS_BACKEND_STATE_CONN_1)
    {
        ret = mbed_connect_step1(tls_ctrl_ptr);
        if (ret != QURL_OK)
        {
            return ret;
        }
    }

    /**
	 * 2. hanshark handshake.
	 *  Blocking I/O: Returns the result immediately after completing step2 and blocks.
	 *  Non-blocking I/O: requires looping select to monitor file descriptors, and also timing.
	 */
    while (tls_ctrl_ptr->backend_state == QURL_TLS_BACKEND_STATE_CONN_2 || tls_ctrl_ptr->backend_state == QURL_TLS_BACKEND_STATE_CONN_2_READING
           || tls_ctrl_ptr->backend_state == QURL_TLS_BACKEND_STATE_CONN_2_WRITING)
    {
        /*Calculate time difference*/
        /** [lzm][todo]Need to combine the total time limit to obtain the final blocking time for select.*/
        now_timeval = qurl_time_now();
        timeout_ms = qurl_time_diff_ms(tls_ctrl_ptr->start_timeval, now_timeval);

        timeout_ms = (cfg_ptr->negotiate_timeout * 1000) - timeout_ms;
        if (timeout_ms < 0)
        {
            qurl_log(QURL_CFG_LOG_TLS, "negotiate timeout\r\n");
            return QURL_ECODE_TLS_NEGOTIATE_TIMEOUT;
        }

        /*Expecting network IO*/
        if ((tls_ctrl_ptr->backend_state == QURL_TLS_BACKEND_STATE_CONN_2_READING) || (tls_ctrl_ptr->backend_state == QURL_TLS_BACKEND_STATE_CONN_2_WRITING))
        {
            qurl_socket_t r_fd = (tls_ctrl_ptr->backend_state == QURL_TLS_BACKEND_STATE_CONN_2_READING ? tls_ctrl_ptr->sock_fd : QURL_SOCKET_BAD);
            qurl_socket_t w_fd = (tls_ctrl_ptr->backend_state == QURL_TLS_BACKEND_STATE_CONN_2_WRITING ? tls_ctrl_ptr->sock_fd : QURL_SOCKET_BAD);

            FD_ZERO(&fds_read);
            FD_ZERO(&fds_write);
            FD_ZERO(&fds_err);
            max_fd = (qurl_socket_t)-1;
            FD_SET(r_fd, &fds_read);
            FD_SET(w_fd, &fds_write);
            FD_SET(r_fd, &fds_err);
            FD_SET(w_fd, &fds_err);
            max_fd = QURL_MAX(r_fd, w_fd);

            ret = qurl_select(max_fd, &fds_read, &fds_write, &fds_err, (noblocking ? 0 : timeout_ms));
            if (ret < 0)
            {
                qurl_log(QURL_CFG_LOG_TLS, "negotiate err:[-0x%d]\r\n", -ret);
                return QURL_ECODE_TLS_NEGOTIATE_ERR;
            }
            else if (ret == 0)
            {
                if (noblocking)
                {
                    *done_ptr = QOSA_FALSE;
                    return QURL_OK;
                }
                else
                {
                    qurl_log(QURL_CFG_LOG_TLS, "negotiate timeout\r\n");
                    return QURL_ECODE_TLS_NEGOTIATE_TIMEOUT;
                }
            }
        }

        ret = mbed_connect_step2(tls_ctrl_ptr);
        if (ret != QURL_OK
            || (noblocking
                && ((tls_ctrl_ptr->backend_state == QURL_TLS_BACKEND_STATE_CONN_2) || (tls_ctrl_ptr->backend_state == QURL_TLS_BACKEND_STATE_CONN_2_READING)
                    || (tls_ctrl_ptr->backend_state == QURL_TLS_BACKEND_STATE_CONN_2_WRITING))))
        {
            /** 1. Error, return directly
			 *  2. Normal, but non-blocking, and still in the handshake process, also return first: [lzm][todo] support one perform executing multiple cores
			 */
            return ret;
        }
    }

    /*3. After the TLS handshake is successful, read and write operations can actually be performed, but we still have step 3 to handle some post-handshake processing.*/
    if (tls_ctrl_ptr->backend_state == QURL_TLS_BACKEND_STATE_CONN_3)
    {
        ret = mbed_connect_step3(tls_ctrl_ptr);
        if (ret != QURL_OK)
        {
            return ret;
        }
    }

    /*Check SSL connection status, update external status*/
    if (tls_ctrl_ptr->backend_state == QURL_TLS_BACKEND_STATE_CONN_DONE)
    {
        tls_ctrl_ptr->state = QURL_TLS_STATE_COMPLETE;
        *done_ptr = QOSA_TRUE;
    }
    else
    {
        *done_ptr = QOSA_FALSE;
    }

    /*Reset the TLS internal state machine*/
    tls_ctrl_ptr->backend_state = QURL_TLS_BACKEND_STATE_CONN_1;

    return QURL_OK;
}

/*Adapter Layer Interface*/

/**
 * @brief mbedtls resource global initialization
 *
 * @param[in] tls_ctrl_ptr
 * @return qurl_ecode_t
 */
static qurl_ecode_t tls_mbed_init(void)
{
    if (g_sessions_g_ctrl.bits.inited == QOSA_FALSE)
    {
        qurl_mutex_create(&g_sessions_g_ctrl.lock);
        g_sessions_g_ctrl.max_cnt = QURL_CFG_TLS_SESSION_CACHE_MAX_CNT_DEFAULT;

        qurl_list_init(&g_sessions_g_ctrl.sessions_list);

        g_sessions_g_ctrl.bits.inited = QOSA_TRUE;
    }
    return QURL_OK;
}

/**
 * @brief Clear mbedtls global resources
 *
 * @param[in] tls_ctrl_ptr
 * @return qurl_ecode_t
 */
static qurl_ecode_t tls_mbed_deinit(void)
{
    return QURL_OK;
}

/**
 * @brief Get mbedtls version (string)
 *
 * @param buf
 * @param size
 * @return long
 */
static long tls_mbed_version(char *buf, long size)
{
    return qurl_snprintf(buf, size, "mbedTLS/%s", MBEDTLS_VERSION_STRING);
}

/**
 * @brief Is there any data pending processing?
 *
 * @param tls_ctrl_ptr
 * @return qurl_bit_t
 */
static qurl_bit_t tls_mbed_data_pending(qurl_tls_ctrl_t *tls_ctrl_ptr)
{
    qurl_tls_backend_ctrl_t *mbed_ctrl_ptr = (qurl_tls_backend_ctrl_t *)tls_ctrl_ptr->tls_ptr;

    return mbedtls_ssl_get_bytes_avail(&mbed_ctrl_ptr->ssl) != 0;
}

/**
 * @brief Blocking connection
 *
 * @param tls_ctrl_ptr
 * @return qurl_ecode_t
 */
static qurl_ecode_t tls_mbed_connect(qurl_tls_ctrl_t *tls_ctrl_ptr)
{
    qurl_bit_t done = QOSA_FALSE;

    return mbed_connect(tls_ctrl_ptr, QOSA_FALSE, &done);
}

/**
 * @brief Non-blocking connection
 *
 * @param tls_ctrl_ptr
 * @return qurl_ecode_t
 */
static qurl_ecode_t tls_mbed_connect_nonblocking(qurl_tls_ctrl_t *tls_ctrl_ptr, qurl_bit_t *done_ptr)
{
    return mbed_connect(tls_ctrl_ptr, QOSA_TRUE, done_ptr);
}

/**
 * @brief Close resources
 *
 * @param tls_ctrl_ptr
 * @return qurl_ecode_t
 */
static qurl_ecode_t tls_mbed_close(qurl_tls_ctrl_t *tls_ctrl_ptr)
{
    qurl_tls_backend_ctrl_t *mbed_ctrl_ptr = (qurl_tls_backend_ctrl_t *)tls_ctrl_ptr->tls_ptr;

    if (mbed_ctrl_ptr != QOSA_NULL)
    {
        mbedtls_pk_free(&mbed_ctrl_ptr->pk);
        mbedtls_x509_crt_free(&mbed_ctrl_ptr->clicert);
        mbedtls_x509_crt_free(&mbed_ctrl_ptr->cacert);
        mbedtls_ssl_config_free(&mbed_ctrl_ptr->config);
        mbedtls_ssl_free(&mbed_ctrl_ptr->ssl);
        mbedtls_ctr_drbg_free(&mbed_ctrl_ptr->ctr_drbg);
        mbedtls_entropy_free(&mbed_ctrl_ptr->entropy);
    }

    return QURL_OK;
}

/**
 * @brief Read data
 *
 * @param[in] tls_ctrl_ptr
 * @param[in] buf
 * @param[in] buf_size
 * @param[out] ecode_ptr
 * @return long
 */
long tls_mbed_read(qurl_tls_ctrl_t *tls_ctrl_ptr, unsigned char *buf, long buf_size, qurl_ecode_t *ecode_ptr)
{
    int                      ret = 0;
    qurl_tls_backend_ctrl_t *mbed_ctrl_ptr = (qurl_tls_backend_ctrl_t *)tls_ctrl_ptr->tls_ptr;

    *ecode_ptr = QURL_OK;

    ret = mbedtls_ssl_read(&mbed_ctrl_ptr->ssl, (unsigned char *)buf, buf_size);
    if (ret <= 0)
    {
        qurl_log(QURL_CFG_LOG_TLS, "ret:[-0x%x]\r\n", -ret);
        if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY)
        {
            /*Received a "close_notify" alert: the other party has notified us that the connection is about to close.*/
            *ecode_ptr = QURL_ECODE_TLS_PEER_CLOSE_NOTIFY;
            return 0;
        }
        *ecode_ptr = (ret == MBEDTLS_ERR_SSL_WANT_READ) ? QURL_ECODE_TLS_READ_AGAIN : QURL_ECODE_TLS_READ_ERR;
        return -1;
    }

    return ret;
}

/**
 * @brief
 *
 * @param[in] tls_ctrl_ptr
 * @param[out] buf
 * @param[in] buf_size
 * @param[out] ecode_ptr
 * @return long
 */
long tls_mbed_write(qurl_tls_ctrl_t *tls_ctrl_ptr, unsigned char *buf, long buf_size, qurl_ecode_t *ecode_ptr)
{
    int                      ret = 0;
    qurl_tls_backend_ctrl_t *mbed_ctrl_ptr = (qurl_tls_backend_ctrl_t *)tls_ctrl_ptr->tls_ptr;

    *ecode_ptr = QURL_OK;

    ret = mbedtls_ssl_write(&mbed_ctrl_ptr->ssl, (unsigned char *)buf, buf_size);
    if (ret < 0)
    {
        qurl_log(QURL_CFG_LOG_TLS, "ret:[-0x%x]\r\n", -ret);
        *ecode_ptr = (ret == MBEDTLS_ERR_SSL_WANT_WRITE) ? QURL_ECODE_TLS_WRITE_AGAIN : QURL_ECODE_TLS_WRITE_ERR;
        return -1;
    }

    return ret;
}

/*Registration Interface*/
const qurl_tls_adapter_t g_tls_port_mbedtls = {
    .info = {QURL_TLS_BACKEND_TYPE_MBEDTLS, "mbedtls"},
    .tls_backend_ctrl_size = sizeof(qurl_tls_backend_ctrl_t),

    .init_func = tls_mbed_init,
    .deinit_func = tls_mbed_deinit,
    .version_func = tls_mbed_version,
    .data_pending_func = tls_mbed_data_pending,

    .connect_func = tls_mbed_connect,
    .connect_nonblocking_func = tls_mbed_connect_nonblocking,
    .close_func = tls_mbed_close,
    .read_func = tls_mbed_read,
    .write_func = tls_mbed_write,
};

#endif
