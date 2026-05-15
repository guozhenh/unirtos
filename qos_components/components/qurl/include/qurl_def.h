/** @file         qurl_def.h
 *  @brief        Brief description
 *  @details      Detailed description
 *  @author       lzm
 *  @date         2023-10-19 15:52:48
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Modification log:
 **********************************************************
 */

#ifndef QURL_DEF_H
#define QURL_DEF_H

#include "qurl_code.h"
#include <stdbool.h>

/**
 * @brief Variable type related
 */
typedef qosa_bool_t qurl_bit_t;

/**
 * @brief Global error code type
 */
typedef int qurl_ecode_t;

/**
 * @brief qurl core operation handle
 */
typedef void *qurl_core_t;

/**
 * @brief qurl slist operation handle
 */
typedef void *qurl_slist_t;

/* qurl callbacks */
typedef long (*qurl_write_head_cb)(unsigned char *buf, long size, void *arg);
typedef long (*qurl_write_cb)(unsigned char *buf, long size, void *arg);
typedef long (*qurl_read_head_cb)(unsigned char *buf, long size, void *arg);
typedef long (*qurl_read_cb)(unsigned char *buf, long size, void *arg);
typedef long (*qurl_http_form_read_cb)(unsigned char *buf, long size, void *arg);

/* socket */
/**
 * @struct qurl_so_linger_t
 * @brief socket opt SO_LINGER option configuration
 */
struct qurl_so_linger_s
{
    int l_onoff;  /* 0 = off, nozero = on */
    int l_linger; /* linger time, second */
};
typedef struct qurl_so_linger_s qurl_so_linger_t;

/* tls */
/**
 * @struct qurl_tls_cfg_bits_t
 * @brief tls configuration parameters
 */
struct qurl_tls_cfg_bits_s
{
    qurl_bit_t sni_enable;      /*!< SNI extension switch */
    qurl_bit_t session_match;   /*!< Match session, enabled by default */
    qurl_bit_t session_share;   /*!< Share session, enabled by default */
    qurl_bit_t renegotiation;   /*!< Renegotiation, disabled by default */
    qurl_bit_t close_time_mode; /*!< Close delay mode, disabled by default */
};
typedef struct qurl_tls_cfg_bits_s qurl_tls_cfg_bits_t;
/**
 * @struct qurl_tls_cfg_t
 * @brief tls configuration parameters
 */
struct qurl_tls_cfg_s
{
    qurl_tls_version_e version;                       /*!< tls version */
    int               *ciphersuites;                  /*!< Cipher suite list, ends with 0x0000, if pointer is NULL, means all cipher suites are supported. */
    qurl_tls_verify_e  verify;                        /*!< Certificate verification mode */
    qurl_slist_t       ca_cert_path_slist;            /*!< CA certificate path single linked list */
    char              *own_cert_path_ptr;             /*!< Client public key digital certificate path */
    char              *own_key_path_ptr;              /*!< Client private key file path */
    char              *own_key_pwd_ptr;               /*!< Client private key password string */
    long               negotiate_timeout;             /*!< Handshake timeout, unit: s */
    int                ignore_invalid_certsign;       /*!< Ignore invalid certificate signature */
    int                ignore_multi_certchain_verify; /*!< Ignore multi-level certificate chain verification */
    unsigned int       ignore_certitem;               /*!< Whether to ignore certain certificate verification */
    int                ssl_log_debug;                 /*!< Whether to print debug logs */
    int                mfl_support;                   /*!< Whether to support mfl */
    int                mfl_size;                      /*!< mfl length */
    char              *alpn_name;                     /*!< Store SSL ALPN information */
    int                psk_enable;                    /*!< Enable DTLS PSK function or not */
    char              *psk_identity;                  /*!< The corresponding DTLS PSK identity can only be a string. */
    char              *psk_key;                       /*!< The DTLS PSK characters can include the character 0x00. */
    char               psk_key_len;                   /*!< PSK key character length */
    char               psk_format;                    /*!< PSK key data format: 0 - String  1 - Hex */

    qurl_tls_cfg_bits_t bits;
};
typedef struct qurl_tls_cfg_s qurl_tls_cfg_t;

/* http */
/** qurl http form configuration data structure */
/**
 * @struct qurl_http_form_cfg_t
 * @brief
 */
struct qurl_http_form_cfg_s
{
    qurl_http_form_e content_type;            /*!< Type of content_ptr */
    long             content_len;             /*!< Length of content field to be sent */

    char                  *name_ptr;          /*!< Name field of the form */
    char                  *filename_ptr;      /*!< Filename field of the form */
    void                  *content_ptr;       /*!< Content of the form, parameter of read_content_func */
    qurl_http_form_read_cb read_content_func; /*!< Content of single form, only valid when QURL_HTTP_FORM_CONTENT_CB */
    qurl_slist_t headers_slist; /*!< User-defined headers of the form. Resources are maintained by the user, so please ensure their operational safety. */
};
typedef struct qurl_http_form_cfg_s qurl_http_form_cfg_t;

#endif /* Head define end*/
