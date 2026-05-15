/*****************************************************************/ /**
* @file qcm_vtls.c
* @brief
* @author larson.li@quectel.com
* @date 2023-08-09
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description
* <tr><td>2023-08-09 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#include "qcm_vtls.h"
#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_log.h"
#include "mbedtls.h"
#include "qosa_rtc.h"
#include "qosa_log.h"
#include "qosa_queue_list.h"

#define USE_MBEDTLS 1

const struct qcm_ssl_adapter *qcm_ssl =
#if defined(USE_MBEDTLS)
    &qcm_ssl_mbedtls;
#elif defined(USE_OPENSSL)

#elif defined(USE_WOLFSSL)

#else
#error "Missing struct qcm_ssl for selected SSL backend"
#endif

static qosa_bool_t   init_ssl = QOSA_FALSE;
static qosa_q_type_t g_session_save;
static qosa_mutex_t  g_session_mutex;

static void qcm_ssl_certex_int(void);

/**
 * @brief Initialize the SSL init function, call it at least once.
 *
 * @param[in] void
 *
 * @return int
 */
int qcm_ssl_init(void)
{
    /* make sure this is only done once */
    if (init_ssl)
        return 1;
    init_ssl = QOSA_TRUE; /* never again */
    qosa_q_init(&g_session_save);
    qosa_mutex_create(&g_session_mutex);
    qcm_ssl_certex_int();
    return qcm_ssl->init();
}

/**
 * @brief Clear the oldest session ID
 */
static qcm_vtls_result_status_e qcm_ssl_clean_long_time_session(void)
{
    qcm_ssl_session_t *store_session = QOSA_NULL;
    qcm_ssl_session_t *next_session = QOSA_NULL;
    qcm_ssl_session_t *temp = QOSA_NULL;

    store_session = (qcm_ssl_session_t *)qosa_q_check(&g_session_save);
    temp = store_session;
    while (store_session != QOSA_NULL)
    {
        next_session = qosa_q_next(&g_session_save, &store_session->link);

        if (next_session != QOSA_NULL)
        {
            if (temp->last_save_time > next_session->last_save_time)
            {
                temp = next_session;
            }
        }
        else
        {
            qosa_q_delete(&g_session_save, &temp->link);
            qcm_ssl->sessionid_free(temp->sessionid);
            qosa_free(temp->sessionid);
            qosa_free(temp->remote_host_name);
            qosa_free(temp);
        }
        store_session = next_session;
    }
    return QCM_VTLS_RESULT_OK;
}

/**
 * @brief Retrieve the corresponding session ID based on hostname and port, while also traversing current sessions to check for timeouts.
 *
 * @param[in] char * hostname
 *          - Host name to be retrieved
 *
 * @param[in] qosa_uint16_t port
 *          - The host port number to be retrieved
 *
 * @param[in] qosa_uint32_t timeout
 *          - Maximum session timeout
 *
 * @return qcm_ssl_session_t *
 *       - If the search is successful, return the actual session id pointer
 *       - Returns OSA_NULL on failure
 */
static qcm_ssl_session_t *qcm_ssl_search_session(char *hostname, qosa_uint16_t port, qosa_uint32_t timeout)
{
    qcm_ssl_session_t *store_session = QOSA_NULL;
    qcm_ssl_session_t *temp = QOSA_NULL;

    store_session = (qcm_ssl_session_t *)qosa_q_check(&g_session_save);
    while (store_session != QOSA_NULL)
    {
        if (qosa_strncmp(store_session->remote_host_name, hostname, qosa_strlen(hostname)) == 0 && store_session->remote_port == port)
        {
            return store_session;
        }

        //Timeout check and delete expired sessions
        if (qosa_get_system_time_seconds() - store_session->last_save_time > timeout)
        {
            temp = (qcm_ssl_session_t *)qosa_q_next(&g_session_save, &store_session->link);
            qosa_q_delete(&g_session_save, &store_session->link);

            qcm_ssl->sessionid_free(store_session->sessionid);
            qosa_free(store_session->sessionid);
            qosa_free(store_session->remote_host_name);
            qosa_free(store_session);
            store_session = temp;
        }
        else
        {
            store_session = (qcm_ssl_session_t *)qosa_q_next(&g_session_save, &store_session->link);
        }
    }

    return QOSA_NULL;
}

/**
 * @brief Get SSL session ID
 *
 * @param[in] qcm_ssl_connect_data_t * connssl
 *          - The connection pointer address corresponding to the current SSL connection
 *
 * @param[out] void * * ssl_sessionid
 *          - Get the corresponding SSL session address
 *
 * @return qcm_vtls_result_status_e
 *       - Returns QCM_VTLS_RESULT_OK on success
 *       - Returns other values on failure
 */
qcm_vtls_result_status_e qcm_ssl_get_sessionid(qcm_ssl_connect_data_t *connssl, void **ssl_sessionid)
{
    qcm_ssl_session_t *store_session = QOSA_NULL;
    qosa_mutex_lock(g_session_mutex, QOSA_WAIT_FOREVER);
    store_session = qcm_ssl_search_session(connssl->hostname, connssl->port, 300);
    if (store_session == QOSA_NULL)
    {
        qosa_mutex_unlock(g_session_mutex);
        return QCM_VTLS_SSL_INVALID_PARAM_ERR;
    }

    *ssl_sessionid = QOSA_NULL;
    *ssl_sessionid = store_session->sessionid;

    qosa_mutex_unlock(g_session_mutex);
    return QCM_VTLS_RESULT_OK;
}

/**
 * @brief Set SSL session saving.
 *
 * @param[in] qcm_ssl_connect_data_t * connssl
 *          - The connection pointer address corresponding to the current SSL connection
 *
 * @param[in] void * ssl_sessionid
 *          - Corresponding SSL session address to be saved
 *
 * @return qcm_vtls_result_status_e
 *       - Returns QCM_VTLS_RESULT_OK on success
 *       - Returns other values on failure
 */
qcm_vtls_result_status_e qcm_ssl_add_sessionid(qcm_ssl_connect_data_t *connssl, void *ssl_sessionid)
{
    qcm_ssl_session_t *store_session = QOSA_NULL;
    qosa_mutex_lock(g_session_mutex, QOSA_WAIT_FOREVER);
    //This method cannot be used; only a while loop can be used.
    store_session = qcm_ssl_search_session(connssl->hostname, connssl->port, 300);
    if (store_session == QOSA_NULL)
    {
        if (qosa_q_cnt(&g_session_save) >= 5)
        {
            //If more than 5 SSL sessions are stored, clear the oldest session ID.
            qcm_ssl_clean_long_time_session();
        }

        store_session = (qcm_ssl_session_t *)qosa_malloc(sizeof(qcm_ssl_session_t));
        if (store_session == QOSA_NULL)
        {
            qosa_mutex_unlock(g_session_mutex);
            return QCM_VTLS_SSL_MEMORY_ERR;
        }
        qosa_memset(store_session, 0, sizeof(qcm_ssl_session_t));

        store_session->remote_host_name = qosa_malloc(qosa_strlen(connssl->hostname));
        if (store_session->remote_host_name == QOSA_NULL)
        {
            qosa_free(store_session);
            qosa_mutex_unlock(g_session_mutex);
            return QCM_VTLS_SSL_MEMORY_ERR;
        }
        qosa_memset(store_session->remote_host_name, 0, qosa_strlen(connssl->hostname));

        qosa_memcpy(store_session->remote_host_name, connssl->hostname, qosa_strlen(connssl->hostname));
        store_session->remote_port = connssl->port;
        store_session->last_save_time = qosa_get_system_time_seconds();
        store_session->sessionid = ssl_sessionid;
        qosa_q_link(store_session, &store_session->link);
        qosa_q_put(&g_session_save, &store_session->link);
    }
    else
    {
        store_session->last_save_time = qosa_get_system_time_seconds();
        qcm_ssl->sessionid_free(store_session->sessionid);
        qosa_free(store_session->sessionid);

        store_session->sessionid = ssl_sessionid;
    }
    qosa_mutex_unlock(g_session_mutex);

    return QCM_VTLS_RESULT_OK;
}

/**
 * @brief Clear all SSL sessions
 *
 */
void qcm_ssl_clean_all_sessionid(void)
{
    qcm_ssl_session_t *store_session = QOSA_NULL;
    qcm_ssl_session_t *temp = QOSA_NULL;

    qosa_mutex_lock(g_session_mutex, QOSA_WAIT_FOREVER);
    store_session = (qcm_ssl_session_t *)qosa_q_check(&g_session_save);
    while (store_session != QOSA_NULL)
    {
        temp = (qcm_ssl_session_t *)qosa_q_next(&g_session_save, &store_session->link);
        qosa_q_delete(&g_session_save, &store_session->link);

        qcm_ssl->sessionid_free(store_session->sessionid);
        qosa_free(store_session->sessionid);
        qosa_free(store_session->remote_host_name);
        qosa_free(store_session);

        store_session = temp;
        temp = QOSA_NULL;
    }
    qosa_mutex_unlock(g_session_mutex);
}

/**
 * @brief For creating SSL connection objects
 *
 * @return qcm_ssl_connect_data *
 *       - Success, returns the corresponding requested address
 *       - Failure, returns OSA_NULL
 */
qcm_ssl_connect_data_t *qcm_ssl_new(qcm_ssl_config_t *ssl_config_ptr)
{
    qcm_ssl_connect_data_t *new_ptr = QOSA_NULL;
    new_ptr = (qcm_ssl_connect_data_t *)qosa_malloc(sizeof(qcm_ssl_connect_data_t));
    if (new_ptr == QOSA_NULL)
    {
        return QOSA_NULL;
    }

    qosa_memset(new_ptr, 0, sizeof(qcm_ssl_connect_data_t));
    new_ptr->backend = qosa_malloc(qcm_ssl->sizeof_ssl_backend_data);
    if (!new_ptr->backend)
    {
        qosa_free(new_ptr);
        return QOSA_NULL;
    }

    qosa_memset(new_ptr->backend, 0, qcm_ssl->sizeof_ssl_backend_data);
    qosa_memcpy(&new_ptr->ssl_config, ssl_config_ptr, sizeof(qcm_ssl_config_t));
    return new_ptr;
}

/**
 * @brief If SNI or session is enabled, hostinfo connection information must be configured,
 *        To store the host name and the corresponding port number
 *
 * @param[in] const char * hostname
 *          - Corresponding remote connection domain name
 *
 * @param[in] qosa_uint16_t port
 *          - Corresponding remote port number
 *
 * @return qcm_vtls_result_status_e
 */
qcm_vtls_result_status_e qcm_ssl_set_hostinfo(qcm_ssl_connect_data_t *connssl, const char *hostname, qosa_uint16_t port)
{
    char *ptr = QOSA_NULL;
    int   len = qosa_strlen(hostname);

    //Copy the host domain name; if SNI is not enabled, the client does not need to perform this step.
    ptr = qosa_malloc(len + 1);
    if (ptr == QOSA_NULL)
    {
        return QCM_VTLS_SSL_MEMORY_ERR;
    }
    ptr[len] = 0x00;

    if (connssl->hostname != QOSA_NULL)
    {
        qosa_free(connssl->hostname);
    }
    connssl->hostname = ptr;

    qosa_strcpy(connssl->hostname, hostname);
    connssl->port = port;

    return QCM_VTLS_RESULT_OK;
}

/**
 * @brief Used to reclaim resources allocated by qcm_ssl_new
 *  Before calling, you must first execute the qcm_ssl_close function to release the SSL session.
 *
 * @param[in] qcm_ssl_connect_data * connssl
 *          - SSL connection address pointer
 *
 */
void qcm_ssl_free(qcm_ssl_connect_data_t *connssl)
{
    if (connssl)
    {
        if (connssl->hostname)
        {
            qosa_free(connssl->hostname);
            connssl->hostname = QOSA_NULL;
        }
        if (connssl->backend)
        {
            qosa_free(connssl->backend);
            connssl->backend = QOSA_NULL;
        }
        qosa_free(connssl);
    }
}

/**
 * @brief Send SSL connection interface, a blocking interface; exit to check the corresponding SSL connection status.
 *
 * @param[in] struct qcm_ssl_connect_data * connssl
 *          - SSL internal link configuration pointer
 *
 * @param[in] qosa_uint32_t max_timeout
 *          - If the maximum wait time is exceeded, it is considered a timeout and will exit.
 *
 * @return qcm_vtls_result_status_e
 *       - Returns QCM_VTLS_RESULT_OK on success
 *       - Returns a non-zero value on failure
 */
qcm_vtls_result_status_e qcm_ssl_connect(qcm_ssl_connect_data_t *connssl)
{
    int result;

    /* mark this is being ssl-enabled from here on. */
    connssl->state = QCM_SSL_CONNECTION_NEGOTIATING;

    result = qcm_ssl->connect_blocking(connssl);

    return result;
}

/**
 * @brief Send SSL connection interface, a blocking interface. After exiting, you can check the corresponding SSL connection status.
 *
 * @param[in] struct qcm_ssl_connect_data * connssl
 *          - SSL internal link configuration pointer
 *
 * @param[in] qosa_bool_t *done
 *
 * @return qcm_vtls_result_status_e
 *       - Returns QCM_VTLS_RESULT_OK on success
 *       - Returns a non-zero value on failure
 */
qcm_vtls_result_status_e qcm_ssl_connect_nonblocking(qcm_ssl_connect_data_t *connssl, qosa_bool_t *done)
{
    int result;

    /* mark this is being ssl requested from here on. */
    result = qcm_ssl->connect_nonblocking(connssl, done);

    return result;
}

/**
 * @brief SSL resource release and shutdown
 *
 * @param[in] qcm_ssl_connect_data * connssl
 *          - SSL internal link configuration pointer
 *
 */
void qcm_ssl_close(qcm_ssl_connect_data_t *connssl)
{
    qcm_ssl->close_one(connssl);
    connssl->state = QCM_SSL_CONNECTION_NONE;
}

/**
 * @brief Obtain SSL version related information
 *
 * @param[in] char * buffer
 *          - Address space related to storing version information
 *
 * @param[in] qosa_size_t size
 *          - Corresponding buffer space size
 *
 * @return qosa_size_t
 *       - Returns the actual assigned buffer length size
 */
qosa_size_t qcm_ssl_version(char *buffer, qosa_size_t size)
{
    return qcm_ssl->version(buffer, size);
}

/**
 * @brief Get SSL cache data information status
 *
 * @param[in] qcm_ssl_connect_data * connssl
 *          - Corresponding SSL internal link pointer
 *
 * @return qosa_bool_t
 *       - If there is data, return OSA_TRUE
 *       - If there is no data, return OSA_FALSE
 */
qosa_bool_t qcm_ssl_data_pending(qcm_ssl_connect_data_t *connssl)
{
    return qcm_ssl->data_pending(connssl);
}

/**
 * @brief Data reading interface after SSL connection is established
 *
 * @param[in] qcm_ssl_connect_data * connssl
 *          - Corresponding SSL internal link pointer
 *
 * @param[in] char * buf
 *          - read reads the data content pointer
 *
 * @param[in] qosa_size_t buffersize
 *          - Corresponding buffer space size
 *
 * @param[out] qcm_vtls_result_status_e * curlcode
 *          - Corresponding API interface execution status
 *
 * @return qosa_size_t
 *       - Returns the actual number of bytes read
 */
qosa_size_t qcm_ssl_read(qcm_ssl_connect_data_t *connssl, char *buf, qosa_size_t buffersize, qcm_vtls_result_status_e *curlcode)
{
    return qcm_ssl->read(connssl, buf, buffersize, curlcode);
}

/**
 * @brief Data transmission interface after SSL connection is established
 *
 * @param[in] qcm_ssl_connect_data * connssl
 *          - Corresponding SSL internal link pointer
 *
 * @param[in] char * buf
 *          - write data content pointer
 *
 * @param[in] qosa_size_t buffersize
 *          - Corresponding actual data length inside the buffer
 *
 * @param[in] qcm_vtls_result_status_e * curlcode
 *          - Corresponding API interface execution status
 *
 * @return qosa_size_t
 *       - Returns the actual number of bytes read
 */
qosa_size_t qcm_ssl_write(qcm_ssl_connect_data_t *connssl, char *buf, qosa_size_t buffersize, qcm_vtls_result_status_e *curlcode)
{
    return qcm_ssl->write(connssl, buf, buffersize, curlcode);
}

/**
 * @brief Check if the corresponding ciphersuite algorithm is valid
 *
 * @param[in] int cs_id
 *          - ID of the corresponding cipher suite
 *
 * @return qosa_bool_t
 *       - Returns OSA_TRUE if successful
 *       - Returns OSA_FALSE on failure
 */
qosa_bool_t qcm_ssl_check_ciphersuit_is_valid(int cs_id)
{
    return qcm_ssl->check_ciphersuit(cs_id);
}

/**
 * @brief Get the system-supported ciphersuite algorithms
 *
 * @return const int *
 *       - Returns a pointer to the array of ciphersuit algorithms supported by the system
 */
const int *qcm_ssl_get_support_ciphersuit(void)
{
    return qcm_ssl->get_ciphersuit();
}

static struct
{
    char        *path[SSL_MAX_CA_CERT_EX_CNT];
    qosa_mutex_t lock;
} g_certex;

static void qcm_ssl_certex_int(void)
{
    int kk = 0;
    while (kk < SSL_MAX_CA_CERT_EX_CNT)
    {
        g_certex.path[kk] = QOSA_NULL;
        kk++;
    }
    qosa_mutex_create(&g_certex.lock);
}

/**
 * @brief Set the global SSL CA certificate, the configuration will override the user's ca_cert_path configuration in the qcm_ssl_config_t structure.
 *
 * @param[in] int index
 *          - To configure the CA certificate path index
 *
 * @param[in] char * path
 *          - Corresponding CA certificate path address
 *
 * @return qcm_vtls_result_status_e
 */
qcm_vtls_result_status_e qcm_ssl_set_cacertex_path(int index, char *path)
{
    if (!(index >= 0 && index < SSL_MAX_CA_CERT_EX_CNT))
    {
        return QCM_VTLS_SSL_INVALID_PARAM_ERR;
    }

    qosa_mutex_lock(g_certex.lock, QOSA_WAIT_FOREVER);
    if (g_certex.path[index] != QOSA_NULL)
    {
        qosa_free(g_certex.path[index]);
    }
    if (QOSA_NULL == path)
    {
        g_certex.path[index] = QOSA_NULL;
    }
    else
    {
        g_certex.path[index] = qosa_strdup(path);
    }
    qosa_mutex_unlock(g_certex.lock);

    return QCM_VTLS_RESULT_OK;
}

/**
 * @brief Get the CA certificate path corresponding to the global configuration,
 *
 * @param[in] int index
 *           - To obtain the CA certificate path index
 *
 * @param[out] char ** path
 *           - Corresponding CA certificate path address
 *
 * @return qcm_vtls_result_status_e
 */
qcm_vtls_result_status_e qcm_ssl_get_cacertex_path(int index, char **path)
{
    if (!(index >= 0 && index < SSL_MAX_CA_CERT_EX_CNT))
    {
        return QCM_VTLS_SSL_INVALID_PARAM_ERR;
    }

    *path = g_certex.path[index];
    return QCM_VTLS_RESULT_OK;
}

/**
 * @brief Check if the global CA certificate path is set
 *
 * @return qosa_bool_t
 *       - If empty, OSA_TRUE
 *       - If not empty, OSA_FALSE
 */
qosa_bool_t qcm_ssl_check_is_set_cacertex(void)
{
    int         i = 0;
    qosa_bool_t cacertex_is_empty = QOSA_TRUE;

    for (i = 0; i < SSL_MAX_CA_CERT_EX_CNT; i++)
    {
        if (QOSA_NULL != g_certex.path[i])
        {
            cacertex_is_empty = QOSA_FALSE;
            break;
        }
    }

    return cacertex_is_empty;
}

/**
 * @brief Get the global CA certificate path address
 */
char **qcm_ssl_global_cacertex_path(void)
{
    return g_certex.path;
}
