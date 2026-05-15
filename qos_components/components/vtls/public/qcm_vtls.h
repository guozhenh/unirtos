/*****************************************************************/ /**
* @file vtls.h
* @brief VTLS (Virtual TLS) implementation header file
* @author larson.li@quectel.com
* @date 2023-08-09
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2023-08-09 <td>1.0 <td>Larson.Li <td> Initial version
* </table>
**********************************************************************/
#ifndef __QOSA_VTLS_H__
#define __QOSA_VTLS_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_queue_list.h"
#include "qcm_vtls_cfg.h"

/* SSL backend-specific data; each SSL backend declares it differently */
struct ssl_backend_data;

/** VTLS API function module error base offset code */
#define QCM_ERRCODE_VTLS_BASE          (QOSA_COMPONENT_VTLS << 16)

/** VTLS IO function return status, equivalent to read wouldblock */
#define QCM_VTLS_IO_ERR_SSL_WANT_READ  -1
/** VTLS IO function return status, equivalent to write wouldblock */
#define QCM_VTLS_IO_ERR_SSL_WANT_WRITE -2
/** VTLS IO function return status, equivalent to tcp socket connection reset */
#define QCM_VTLS_IO_ERR_SSL_CONN_RESET -3

/** Maximum number of ignore items, note that it needs to be the same as the count in qcm_vtls_x509_badcrl_e */
#define QCM_VTLS_SSL_CERT_ITEM_NUM     20
/**
 * @enum qcm_vtls_x509_badcrl_e
 * @brief Used to ignore specified x509 certificate error items during SSL handshake verification
 *        If you need to add more, modify the QCM_VTLS_SSL_CERT_ITEM_NUM macro count
 */
typedef enum
{
    QCM_VTLS_X509_BADCERT_EXPIRED = 0x01,         /*!< The certificate validity has expired. */
    QCM_VTLS_X509_BADCERT_REVOKED = 0x02,         /*!< The certificate has been revoked (is on a CRL). */
    QCM_VTLS_X509_BADCERT_CN_MISMATCH = 0x04,     /*!< The certificate Common Name (CN) does not match with the expected CN. */
    QCM_VTLS_X509_BADCERT_NOT_TRUSTED = 0x08,     /*!< The certificate is not correctly signed by the trusted CA. */
    QCM_VTLS_X509_BADCRL_NOT_TRUSTED = 0x10,      /*!< The CRL is not correctly signed by the trusted CA. */
    QCM_VTLS_X509_BADCRL_EXPIRED = 0x20,          /*!< The CRL is expired. */
    QCM_VTLS_X509_BADCERT_MISSING = 0x40,         /*!< Certificate was missing. */
    QCM_VTLS_X509_BADCERT_SKIP_VERIFY = 0x80,     /*!< Certificate verification was skipped. */
    QCM_VTLS_X509_BADCERT_OTHER = 0x0100,         /*!< Other reason (can be used by verify callback) */
    QCM_VTLS_X509_BADCERT_FUTURE = 0x0200,        /*!< The certificate validity starts in the future. */
    QCM_VTLS_X509_BADCRL_FUTURE = 0x0400,         /*!< The CRL is from the future */
    QCM_VTLS_X509_BADCERT_KEY_USAGE = 0x0800,     /*!< Usage does not match the keyUsage extension. */
    QCM_VTLS_X509_BADCERT_EXT_KEY_USAGE = 0x1000, /*!< Usage does not match the extendedKeyUsage extension. */
    QCM_VTLS_X509_BADCERT_NS_CERT_TYPE = 0x2000,  /*!< Usage does not match the nsCertType extension. */
    QCM_VTLS_X509_BADCERT_BAD_MD = 0x4000,        /*!< The certificate is signed with an unacceptable hash. */
    QCM_VTLS_X509_BADCERT_BAD_PK = 0x8000,        /*!< The certificate is signed with an unacceptable PK alg (eg RSA vs ECDSA). */
    QCM_VTLS_X509_BADCERT_BAD_KEY = 0x010000,     /*!< The certificate is signed with an unacceptable key (eg bad curve, RSA too short). */
    QCM_VTLS_X509_BADCRL_BAD_MD = 0x020000,       /*!< The CRL is signed with an unacceptable hash. */
    QCM_VTLS_X509_BADCRL_BAD_PK = 0x040000,       /*!< The CRL is signed with an unacceptable PK alg (eg RSA vs ECDSA). */
    QCM_VTLS_X509_BADCRL_BAD_KEY = 0x080000,      /*!< The CRL is signed with an unacceptable key (eg bad curve, RSA too short). */

    QCM_VTLS_X509_BAD_MAX
} qcm_vtls_x509_badcrl_e;

typedef enum
{
    QCM_VTLS_RESULT_OK = 0,
    QCM_VTLS_FAILED_INIT_ERR = 1 | QCM_ERRCODE_VTLS_BASE, /*!< Initialization error */
    QCM_VTLS_SSL_CONNECT_ERR,                             /*!< SSL connection error */
    QCM_VTLS_SSL_INVALID_PARAM_ERR,                       /*!< SSL connection invalid parameter */
    QCM_VTLS_PEER_FAILED_VERIFICATION,                    /*!< Certificate verification error */
    QCM_VTLS_SSL_READ_WRITE_EAGAIN,                       /*!< Non-blocking IO function return status, need to call again after event notification arrives */
    QCM_VTLS_SSL_OPERATION_TIMEDOUT,                      /*!< SSL connection operation timeout */
    QCM_VTLS_SSL_MEMORY_ERR,                              /*!< Internal memory allocation failure */
    QCM_VTLS_SSL_PEER_CLOSE_NOTIFY_ERR,                   /*!< Notification preparing to close, normal result code */

    QCM_VTLS_RESULT_MAX
} qcm_vtls_result_status_e;

/**
 * @enum qcm_ssl_backends_e
 * @struct SSL protocol library backend support type
 */
typedef enum
{
    QCM_SSL_BACKEND_NONE = 0,    /*!< SSL not supported */
    QCM_SSL_BACKEND_MBEDTLS = 1, /*!< Support mbedtls SSL library */

    QCM_SSL_BACKEND_MAX
} qcm_ssl_backends_e;

/**
 * @enum qcm_ssl_connect_state_e
 * @brief SSL internal connection state during non-blocking connection
 */
typedef enum
{
    QCM_SSL_CONNECT_1,         /*!< Initial phase SSL initialization configuration */
    QCM_SSL_CONNECT_2,         /*!< SSL connection authentication phase */
    QCM_SSL_CONNECT_2_READING, /*!< SSL asynchronous Socket state data reading phase */
    QCM_SSL_CONNECT_2_WRITING, /*!< SSL asynchronous Socket state data writing phase */
    QCM_SSL_CONNECT_3,         /*!< SSL connection completed, final information saving */
    QCM_SSL_CONNECT_DONE       /*!< SSL connection completed */
} qcm_ssl_connect_state_e;

/**
 * @enum qcm_ssl_connection_state_e
 * @brief SSL protocol connection state
 */
typedef enum
{
    QCM_SSL_CONNECTION_NONE,        /*!< SSL connection initial state */
    QCM_SSL_CONNECTION_NEGOTIATING, /*!< Negotiating in progress */
    QCM_SSL_CONNECTION_COMPLETE     /*!< Negotiation completed */
} qcm_ssl_connection_state_e;

/**
 * @struct qcm_ssl_session_t
 * @brief Session save
 */
typedef struct
{
    qosa_q_link_type_t link;             /*!< Link list node. */
    qosa_time_t        last_save_time;   /*!< Last time session was saved */
    char              *remote_host_name; /*!< Remote server host name, must be domain name according to protocol, if not domain name, SNI cannot be enabled */
    qosa_uint16_t      remote_port;      /*!< Remote server port number */
    void              *sessionid;        /*!< SSL session ID */
} qcm_ssl_session_t;

/**
 * @struct qcm_ssl_connect_data
 * @brief SSL connection state and connection type
 */
typedef struct
{
    qcm_ssl_connection_state_e state;                     /*!< Corresponding to SSL top-level state */
    qcm_ssl_connect_state_e    connecting_state;          /*!< Corresponding to SSL internal connection state changes */
    void                      *backend;                   /*!< Corresponding to SSL backend internal structure storage information */
    qcm_ssl_config_t           ssl_config;                /*!< Current SSL connection's config configuration information */
    char                      *hostname;                  /*!< hostname for verification */
    int                        port;                      /*!< remote port at origin */
    qosa_time_t                cur_connect_time;          /*!< Current time when SSL handshake connection started */
    qosa_timer_t               dtls_retransmission_timer; /*!< DTLS retransmission timer */
} qcm_ssl_connect_data_t;

/**
 * @struct qcm_ssl_backend
 * @brief Corresponding to SSL backend protocol implementation type information
 */
struct qcm_ssl_backend
{
    qcm_ssl_backends_e id;   /*!< Corresponding to SSL library type ID */
    const char        *name; /*!< Corresponding to SSL library string name */
};
typedef struct qcm_ssl_backend qcm_ssl_backend;

/**
 * @struct qcm_ssl_adapter
 * @brief Backend SSL implementation interface
 */
struct qcm_ssl_adapter
{
    struct qcm_ssl_backend info;
    qosa_size_t            sizeof_ssl_backend_data;
    int (*init)(void);
    void (*cleanup)(void);
    qosa_size_t (*version)(char *buffer, qosa_size_t size);
    /* !!!! Set to bool type mainly because some open source ssl cannot display its internal buffer cache length !!!! */
    qosa_bool_t (*data_pending)(qcm_ssl_connect_data_t *connssl);
    int (*connect_blocking)(qcm_ssl_connect_data_t *connssl);
    int (*connect_nonblocking)(qcm_ssl_connect_data_t *connssl, qosa_bool_t *done);
    void (*close_one)(qcm_ssl_connect_data_t *connssl);
    qosa_size_t (*read)(qcm_ssl_connect_data_t *connssl, char *buf, qosa_size_t buffersize, qcm_vtls_result_status_e *curlcode);
    qosa_size_t (*write)(qcm_ssl_connect_data_t *connssl, const void *mem, qosa_size_t len, qcm_vtls_result_status_e *curlcode);
    qosa_bool_t (*check_ciphersuit)(int cs_id);
    const int* (*get_ciphersuit)(void);
    void (*sessionid_free)(void *sessionid);
};

int qcm_ssl_init(void);

/**
 * @brief Used to create SSL connection object
 *
 * @return qcm_ssl_connect_data *
 *       - Success, return corresponding allocated address
 *       - Failure, return OSA_NULL
 */
qcm_ssl_connect_data_t *qcm_ssl_new(qcm_ssl_config_t *ssl_config_ptr);

/**
 * @brief Used to recycle resources allocated by qcm_ssl_new
 *  Need to execute qcm_ssl_close function first to release SSL session before calling
 *
 * @param[in] qcm_ssl_connect_data * connssl
 *          - SSL connection address pointer
 *
 */
void qcm_ssl_free(qcm_ssl_connect_data_t *connssl);

/**
 * @brief Send SSL connection interface, blocking interface, exit to check corresponding SSL connection status
 *
 * @param[in] struct qcm_ssl_connect_data * connssl
 *          - SSL internal connection configuration pointer
 *
 * @param[in] qosa_uint32_t max_timeout
 *          - Maximum waiting time, timeout will exit
 *
 * @return qcm_vtls_result_status_e
 *       - Success returns QCM_VTLS_RESULT_OK
 *       - Failure returns non-zero value
 */
qcm_vtls_result_status_e qcm_ssl_connect(qcm_ssl_connect_data_t *connssl);

/**
 * @brief Send SSL connection interface, blocking interface, exit to check corresponding SSL connection status
 *
 * @param[in] struct qcm_ssl_connect_data * connssl
 *          - SSL internal connection configuration pointer
 *
 * @param[in] qosa_bool_t *done
 *
 * @return int
 *       - Success returns QCM_VTLS_RESULT_OK
 *       - Failure returns non-zero value
 */
qcm_vtls_result_status_e qcm_ssl_connect_nonblocking(qcm_ssl_connect_data_t *connssl, qosa_bool_t *done);

/**
 * @brief SSL resource release and close
 *
 * @param[in] qcm_ssl_connect_data * connssl
 *          - SSL internal connection configuration pointer
 *
 */
void qcm_ssl_close(qcm_ssl_connect_data_t *connssl);

/**
 * @brief Get SSL version related information
 *
 * @param[in] char * buffer
 *          - Address space for storing version information
 *
 * @param[in] qosa_size_t size
 *          - Corresponding buffer space size
 *
 * @return qosa_size_t
 *       - Return actual assigned buffer length
 */
qosa_size_t qcm_ssl_version(char *buffer, qosa_size_t size);

/**
 * @brief Get SSL cache data information status
 *
 * @param[in] qcm_ssl_connect_data * connssl
 *          - Corresponding ssl internal connection pointer
 *
 * @return qosa_bool_t
 *       - Return OSA_TRUE if there is data
 *       - Return OSA_FALSE if there is no data
 */
qosa_bool_t qcm_ssl_data_pending(qcm_ssl_connect_data_t *connssl);

/**
 * @brief Data reading interface after successful SSL connection
 *
 * @param[in] qcm_ssl_connect_data * connssl
 *          - Corresponding ssl internal connection pointer
 *
 * @param[in] char * buf
 *          - Read data content pointer
 *
 * @param[in] qosa_size_t buffersize
 *          - Corresponding buffer space size
 *
 * @param[out] qcm_vtls_result_status_e * curlcode
 *          - Corresponding API interface execution status
 *
 * @return qosa_size_t
 *       - Return actual read amount size
 */
qosa_size_t qcm_ssl_read(qcm_ssl_connect_data_t *connssl, char *buf, qosa_size_t buffersize, qcm_vtls_result_status_e *curlcode);

/**
 * @brief Data sending interface after successful SSL connection
 *
 * @param[in] qcm_ssl_connect_data * connssl
 *          - Corresponding ssl internal connection pointer
 *
 * @param[in] char * buf
 *          - Write data content pointer
 *
 * @param[in] qosa_size_t buffersize
 *          - Actual data length in corresponding buffer
 *
 * @param[in] qcm_vtls_result_status_e * curlcode
 *          - Corresponding API interface execution status
 *
 * @return qosa_size_t
 *       - Return actual read amount size
 */
qosa_size_t qcm_ssl_write(qcm_ssl_connect_data_t *connssl, char *buf, qosa_size_t buffersize, qcm_vtls_result_status_e *curlcode);

/**
 * @brief Clear all SSL sessions
 *
 */
void qcm_ssl_clean_all_sessionid(void);

/**
 * @brief Set SSL session save
 *
 * @param[in] qcm_ssl_connect_data_t * connssl
 *          - Connection pointer address corresponding to current SSL connection
 *
 * @param[in] void * ssl_sessionid
 *          - Corresponding ssl session address to be saved
 *
 * @return qcm_vtls_result_status_e
 *       - Success returns QCM_VTLS_RESULT_OK
 *       - Failure returns others
 */
qcm_vtls_result_status_e qcm_ssl_add_sessionid(qcm_ssl_connect_data_t *connssl, void *ssl_sessionid);

/**
 * @brief Get SSL session ID
 *
 * @param[in] qcm_ssl_connect_data_t * connssl
 *          - Connection pointer address corresponding to current SSL connection
 *
 * @param[out] void * * ssl_sessionid
 *          - Get corresponding ssl session address
 *
 * @return qcm_vtls_result_status_e
 *       - Success returns QCM_VTLS_RESULT_OK
 *       - Failure returns others
 */
qcm_vtls_result_status_e qcm_ssl_get_sessionid(qcm_ssl_connect_data_t *connssl, void **ssl_sessionid);

/**
 * @brief Check if the corresponding ciphersuit algorithm is valid
 *
 * @param[in] int cs_id
 *          - Corresponding encryption suite ID
 *
 * @return qosa_bool_t
 *       - Return OSA_TRUE on success
 *       - Return OSA_FALSE on failure
 */
qosa_bool_t qcm_ssl_check_ciphersuit_is_valid(int cs_id);

/**
 * @brief Obtain the ciphersuit algorithm supported by the system
 *
 * @return const int *
 *       - Return the array pointer of the ciphersuit algorithm supported by the system
 */
const int *qcm_ssl_get_support_ciphersuit(void);

/**
 * @brief If SNI or session is enabled, hostinfo connection information must be configured,
 *        used to save host host name and corresponding port number
 *
 * @param[in] const char * hostname
 *          - Corresponding remote connection domain name
 *
 * @param[in] qosa_uint16_t port
 *          - Corresponding remote port number for connection
 *
 * @return qcm_vtls_result_status_e
 */
qcm_vtls_result_status_e qcm_ssl_set_hostinfo(qcm_ssl_connect_data_t *connssl, const char *hostname, qosa_uint16_t port);

/**
 * @brief Set global CA certificate used by SSL, configuration will invalidate user's ca_cert_path configuration in qcm_ssl_config_t structure
 *
 * @param[in] int index
 *          - CA certificate path sequence number to configure
 *
 * @param[in] char * path
 *          - Corresponding CA certificate path address
 *
 * @return qcm_vtls_result_status_e
 */
qcm_vtls_result_status_e qcm_ssl_set_cacertex_path(int index, char *path);

/**
 * @brief Get globally configured CA certificate path,
 *
 * @param[in] int index
 *           - CA certificate path sequence number to get
 *
 * @param[out] char ** path
 *           - Corresponding CA certificate path address
 *
 * @return qcm_vtls_result_status_e
 */
qcm_vtls_result_status_e qcm_ssl_get_cacertex_path(int index, char **path);

#endif /* __QOSA_VTLS_H__ */
