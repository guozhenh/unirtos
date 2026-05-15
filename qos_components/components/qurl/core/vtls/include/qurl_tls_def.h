/** @file         qurl_tls_def.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-11-09 20:19:40
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#ifndef QURL_TLS_DEF_H
#define QURL_TLS_DEF_H

#include "qurl_config.h"
#include "qurl_code.h"
#include "qurl_def.h"
#include "qurl_def_basic.h"

/* code */
/**
 * @enum  qurl_tls_backend_type_e
 * @brief tls backend type
 */
typedef enum
{
    QURL_TLS_BACKEND_TYPE_NONE = 0, /*!< Does not support TLS*/
    QURL_TLS_BACKEND_TYPE_MBEDTLS,  /*!< MBEDTLS */
    QURL_TLS_BACKEND_TYPE_LAST,     /*!< For auxiliary use*/
} qurl_tls_backend_type_e;

/**
 * @enum  qurl_tls_state_e
 * @brief tls frontend status*/
typedef enum
{
    QURL_TLS_STATE_NONE = 0,    /*!< Initial state*/
    QURL_TLS_STATE_NEGOTIATING, /*!< Under negotiation*/
    QURL_TLS_STATE_COMPLETE,    /*!< Negotiation completed*/
} qurl_tls_state_e;

/**
 * @enum  qurl_tls_backend_state_e
 * @brief tls backend status*/
typedef enum
{
    QURL_TLS_BACKEND_STATE_CONN_1 = 0, /*!< The first stage of connection, which may include some initialization and handshake preparation work.*/
    QURL_TLS_BACKEND_STATE_CONN_2, /*!< The second stage of connection, typically involving some steps in the SSL handshake process, such as negotiating encryption algorithms, exchanging certificates, etc.*/
    QURL_TLS_BACKEND_STATE_CONN_2_READING, /*!< The read operation is in progress during the second phase of the connection, possibly processing data sent by the server.*/
    QURL_TLS_BACKEND_STATE_CONN_2_WRITING, /*!< The write operation is in progress during the second phase of the connection, possibly sending data to the server.*/
    QURL_TLS_BACKEND_STATE_CONN_3,         /*!< The third stage of connection, which may include some final steps of handshake and confirmation operations.*/
    QURL_TLS_BACKEND_STATE_CONN_DONE,      /*!< The connection has been completed, the connection has been established and data transmission can proceed.*/
} qurl_tls_backend_state_e;

/**
 * @enum  qurl_tls_opt_e
 * @brief qurl configuration options
 */
typedef enum
{
    QURL_TLS_OPT_SOCK_FD = 0,       /*!< Configure fd information*/
    QURL_TLS_OPT_REMOTE_HOST_NAME,  /*!< Configure remote domain name*/
    QURL_TLS_OPT_REMOTE_PORT,       /*!< Configure remote port*/
    QURL_TLS_OPT_MATCH_REMOTE_PORT, /*!< Configure auxiliary remote port*/
} qurl_tls_opt_e;

/*Some declarations*/
struct qurl_tls_adapter_s;
typedef struct qurl_tls_adapter_s qurl_tls_adapter_t;

/**
 * @struct qurl_tls_session_handle_t
 * @brief session handle
 */
typedef void *qurl_tls_session_handle_t;

/**
 * @struct qurl_tls_ctrl_t
 * @brief TLS control block
 */
struct qurl_tls_ctrl_s
{
    /*Status Information*/
    qurl_tls_state_e         state;         /*!< TLS status*/
    qurl_tls_backend_state_e backend_state; /*!< TLS internal state*/

    /*Configuration*/
    void *tls_cfg_ptr; /*!< Configuration information used for TLS connections. Third-party libraries are all qurl_tls_cfg_t; if it is platform adaptation, it is defined by the platform.*/
    /*Interface*/
    void *tls_ptr; /*!< Control block for third-party TLS library*/
    //qurl_tls_adapter_t *adapter_ptr; /*!< TLS adapter interface */

    /*Information*/
    /** Configuration Information*/
    qurl_socket_t sock_fd;          /*!< The file descriptor for this connection operation*/
    char         *remote_host_name; /*!< Remote domain name*/
    int           remote_port;      /*!< Remote port*/
    int match_remote_port; /*!< The port number for session saving and matching. Usually the same as remote_port, but for dual-connection protocol stacks like FTPS, the data connection typically matches the session of the control connection.*/
    /** Internal Information*/
    qurl_timeval_t start_timeval; /*!< Time to start handshake*/

    /*RAM*/
    long cur_w_limit; /*!< [202402291600] Remaining length limit for this round of tls_write. <=0: No limit (first round of this cycle); >0: Length limit for the remaining input in this round.*/
};
typedef struct qurl_tls_ctrl_s qurl_tls_ctrl_t;

/**
 * @struct qurl_tls_backend_info_t
 * @brief tls backend information
 */
struct qurl_tls_backend_info_s
{
    qurl_tls_backend_type_e id;   /*!< id, also the backend TLS type*/
    const char             *name; /*!< Backend TLS string name*/
};
typedef struct qurl_tls_backend_info_s qurl_tls_backend_info_t;

/**
 * @struct qurl_tls_adapter_t qurl_tls_adapter_t
 * @brief The southbound interface layer of qurl_tls is adapted by third-party libraries.
 */
struct qurl_tls_adapter_s
{
    /*Information*/
    qurl_tls_backend_info_t info;                  /*!< [lzm][note]Static selection of backend library, currently not utilizing the capability of this info*/
    long                    tls_backend_ctrl_size; /*!< Backend maintains the length of the TLS space*/

    /*Basic operations*/
    qurl_ecode_t (*init_func)(void);                            /*!< Interface initialization*/
    qurl_ecode_t (*deinit_func)(void);                          /*!< Deinitialize interface*/
    long (*version_func)(char *buf, long size);                 /*!< Get the backend TLS version name*/
    qurl_bit_t (*data_pending_func)(qurl_tls_ctrl_t *ctrl_ptr); /*!< Whether there is unprocessed data in the TLS connection*/

    /*Operation*/
    qurl_ecode_t (*connect_func)(qurl_tls_ctrl_t *ctrl_ptr);                                                   /*!< Blocking connection*/
    qurl_ecode_t (*connect_nonblocking_func)(qurl_tls_ctrl_t *ctrl_ptr, qurl_bit_t *done_ptr);                 /*!< Non-blocking connection*/
    qurl_ecode_t (*close_func)(qurl_tls_ctrl_t *ctrl_ptr);                                                     /*!< Close connection*/
    long (*read_func)(qurl_tls_ctrl_t *ctrl_ptr, unsigned char *buf, long buf_size, qurl_ecode_t *ecode_ptr);  /*!< Read data*/
    long (*write_func)(qurl_tls_ctrl_t *ctrl_ptr, unsigned char *buf, long buf_size, qurl_ecode_t *ecode_ptr); /*!< Send data*/
};
typedef struct qurl_tls_adapter_s qurl_tls_adapter_t;

#endif /* Head define end*/
