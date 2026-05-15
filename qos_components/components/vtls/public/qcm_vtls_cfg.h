/*****************************************************************/ /**
* @file qcm_vtls_cfg.h
* @brief
* @author larson.li@quectel.com
* @date 2023-08-22
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2023-08-22 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_VTLS_CFG_H__
#define __QOSA_VTLS_CFG_H__

#include "qosa_def.h"
#include "qosa_sys.h"

/** Maximum number of certificates that can be configured for OSA SSL */
#define SSL_MAX_CA_CERT_CNT    10
/** Number of common certificates for all SSL connections that can be configured for OSA SSL */
#define SSL_MAX_CA_CERT_EX_CNT 6

/**
 * @enum qcm_ssl_version_type_e
 * @brief SSL available algorithm types
 */
typedef enum
{
    QCM_SSL_VERSION_0 = 0,   /*!< SSL protocol ver. 3.0 */
    QCM_SSL_VERSION_1 = 1,   /*!< TLS protocol ver. 1.0 */
    QCM_SSL_VERSION_2 = 2,   /*!< TLS protocol ver. 1.1 */
    QCM_SSL_VERSION_3 = 3,   /*!< TLS protocol ver. 1.2 */
    QCM_SSL_VERSION_ALL = 4, /*!< NOTE: select ALL */
    QCM_SSL_VERSION_5 = 5,   /*!< TLS protocol ver. 1.3 */
} qcm_ssl_version_type_e;

typedef enum
{
    QCM_SSL_VERIFY_NULL = 0x0000,          /*!< SSL no authentication, do not verify server certificate */
    QCM_SSL_VERIFY_SERVER = 0x0001,        /*!< SSL verify server certificate */
    QCM_SSL_VERIFY_CLIENT_SERVER = 0x0002, /*!< SSL connection two-way authentication, verify server certificate, server verifies client certificate */
} qcm_ssl_authmode_e;

typedef enum
{
    QCM_SSL_TLS_PROTOCOL = 0,  /*!< SSL uses TLS connection protocol, only applicable to TCP */
    QCM_SSL_DTLS_PROTOCOL = 1, /*!< SSL uses DTLS connection protocol, only applicable to UDP communication */
} qcm_ssl_transport_type_e;

typedef enum
{
    QCM_SSL_CLIENT_CERT_FILE = 0,   /*!< Client certificate is provided and saved as a file */
    QCM_SSL_CLIENT_CERT_BUFFER = 1, /*!< Client certificate is provided and saved as a buffer */
} qcm_ssl_client_cert_type_e;

typedef enum
{
    QCM_SSL_VER_DTLS10 = 0,   /**< DTLS protocol ver. 1.0. > */
    QCM_SSL_VER_DTLS12 = 1,   /**< DTLS protocol ver. 1.2. > */
    QCM_SSL_VER_DTLS_ALL = 2, /**< Support DTLS1.0 DTLS1.2 server optional */
} qcm_ssl_dtls_version_type_e;

typedef enum
{
    QCM_SSL_PSK_DISABLED = 0, /**< Disable DTLS PSK function > */
    QCM_SSL_PSK_ENABLED = 1,  /**< Enable DTLS PSK function > */
} qcm_ssl_psk_type_e;

/** User configured custom socket IO read function */
typedef int (*vtls_io_read)(void *ctx, unsigned char *buf, qosa_size_t len);

/** User configured custom socket IO write function */
typedef int (*vtls_io_write)(void *ctx, const unsigned char *buf, qosa_size_t len);

/** User configured custom socket IO select function implementation */
typedef int (*vtls_io_select)(void *ctx, qosa_bool_t read_flag, qosa_bool_t write_flag, qosa_uint32_t wait_time);

/**
 * @struct qcm_ssl_config
 * @brief User-defined SSL configuration
 */
typedef struct
{
    qcm_ssl_version_type_e      ssl_version;                         /*!< SSL algorithm version type TLS1.0 TLS1.1 TLS1.2 TLS1.3 ALL */
    qcm_ssl_transport_type_e    transport;                           /*!< SSL transport type DTLS or TLS */
    int                        *ciphersuites;                        /*!< Configure specified supported algorithm types default 0xFFFF */
    qcm_ssl_authmode_e          auth_mode;                           /*!< Authentication type none option request */
    qosa_bool_t                 sni_enable;                          /*!< Whether SNI extension is enabled */
    char                       *ca_cert_path[SSL_MAX_CA_CERT_CNT];   /*!< Store the path and name where CA root certificates are stored */
    char                       *own_cert_path;                       /*!< Store client certificate path name */
    char                       *own_key_path;                        /*!< Store client public key path name */
    char                       *own_key_pwd;                         /*!< Client public key password string */
    char                       *ca_cert_buffer[SSL_MAX_CA_CERT_CNT]; /*!< Store CA root certificate content */
    qcm_ssl_client_cert_type_e  client_cert_type;                    /*!< Whether client certificate type is buffer mode or file mode */
    char                       *own_cert_buffer;                     /*!< Client certificate content address pointer */
    char                       *own_key_buff;                        /*!< Client certificate key content address pointer */
    int                         ssl_negotiate_timeout;               /*!< SSL handshake timeout */
    int                         ignore_invalid_certsign;             /*!< Ignore a certain error value in the corresponding item of the certificate */
    qosa_uint32_t               ignore_certitem;                     /*!< Whether to ignore certificate verification */
    int                         ignore_multi_certchain_verify;       /*!< Ignore multi-level certificate chain verification */
    qcm_ssl_dtls_version_type_e dtls_version;                        /*!< DTLS version DTLS1.1 DTLS1.2 */
    int                         psk_enable;                          /*!< Whether to enable DTLS PSK function */
    char                       *psk_identity;                        /*!< Corresponding DTLS PSK identity can only be a string */
    char                       *psk_key;                             /*!< DTLS PSK character can include character 0x00 */
    char                        psk_key_len;                         /*!< psk key character length */
    char                        psk_format;                          /*!< PSK key data format: 0 - String  1 - Hex */
    qosa_bool_t                 renegotiation;                       /*!< Whether to enable TLS session renegotiation */
    qosa_ptr                    socket_fd;                           /*!< Original socket FD handle (can be a cast pointer address) */
    qosa_uint8_t                ssl_log_debug;                       /*!< Whether SSL debug log is open */
    qosa_bool_t                 session_cache_enable;                /*!< Select to enable or disable session cache function */
    qosa_uint16_t               dtls_mtu_size;                       /*!< Configure dtls mtu size */
    qosa_bool_t                 close_time_mode;                     /*!< Whether SSL close delay time is enabled */
    char                       *alpn_name;                           /*!< Store SSL ALPN information */
    qosa_bool_t                 mfl_support;                         /*!< Whether to support MFL extension */
    qosa_uint8_t                mfl_size;                            /*!< Configure MFL extension size */

    vtls_io_read  io_read;                                           /*!< Configure the read IO interface function used by SSL */
    vtls_io_write io_write;                                          /*!< Configure the write IO interface function used by SSL */

    /*!!!!!!!!!!!!!!!!!
     *Note if the IO socket is in non-blocking mode,
     *and ssl_connect needs to be blocked, this must be configured
     */
    vtls_io_select io_select; /*!< Configure the select IO function interface used by SSL */
} qcm_ssl_config_t;

#endif /* __QOSA_VTLS_CFG_H__ */
