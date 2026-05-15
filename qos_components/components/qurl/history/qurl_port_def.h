/** @file         qurl_port_def.h
 *  @brief        Brief Description
 *  @details      Detailed description: Belongs to middleware
 *  @author       lzm
 *  @date         2023-09-05 14:31:49
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#ifndef QURL_PORT_DEF_H
#define QURL_PORT_DEF_H

#include "qurl_config.h"
#include "qurl_def.h"
#include "qurl_code.h"
#include "qurl_log.h"

/** Declare some data structures*/
struct qurl_conn_s;
typedef struct qurl_conn_s qurl_conn_t;
struct qurl_port_prot_s;
typedef struct qurl_port_prot_s qurl_port_prot_t;
struct qurl_core_s;
typedef struct qurl_core_s qurl_core_t;

/**
 * @struct qurl_conn_t
 * @brief The only field and variable data in this connection.
 */
struct qurl_conn_s
{
    qurl_core_t *core_ptr;   /*!< Owner of this connection*/

    qurl_socket_t sock_fd;   /*!< read socket */
    qurl_socket_t w_sock_fd; /*!< write socket */

    /*Dual-interface handle. Applicable scenarios include: HTTP redirection, where the new address protocol may be HTTPS.*/
    qurl_port_prot_t *port_handler;       /*!< Protocol interface handle in use*/
    qurl_port_prot_t *basic_port_handler; /*!< Initial protocol interface handle.*/

    qurl_socket_t sock[2];                /* two sockets, the second is used for the data transfer when doing FTP */
};
typedef struct qurl_conn_s qurl_conn_t;

/**
 * @struct qurl_port_prot_t
 * @brief Port callback function data structure, primarily used for protocol interfaces
 */
struct qurl_port_prot_s
{
    const char *scheme; /*!< URL scheme name. */

    /*A supplementary function for setup_connection_internals(), used before connection.*/
    qurl_ecode_t (*setup_connect_func)(qurl_core_t *core_ptr, qurl_conn_t *conn_ptr);

    /*Two functions that must be configured according to the protocol*/
    qurl_ecode_t (*do_it_func)(qurl_core_t *core_ptr, qurl_bit_t *done_ptr);
    qurl_ecode_t (*do_done_func)(qurl_core_t *core_ptr, qurl_ecode_t ecode, qurl_bit_t done);

    /*Split qurl_do() into two parts, allowing do_more() after do. For example: FTP-related operations can be performed after executing PASV/PORT commands.*/
    qurl_ecode_t (*do_more_func)(qurl_core_t *core_ptr, int *completep_ptr);

    /*A step in the connection process, which can be used to set up the protocol. If the connection is not completed, the caller should keep calling qurl_connecting() until it is finished.*/
    qurl_ecode_t (*connect_it_func)(qurl_core_t *core_ptr, qurl_bit_t *done_ptr);

    qurl_ecode_t (*connecting_func)(qurl_core_t *core_ptr, qurl_bit_t *done_ptr);
    qurl_ecode_t (*doing_func)(qurl_core_t *core_ptr, qurl_bit_t *done_ptr);

    /*Multiplexing is called during the PROTOCONNECT phase, returning the fd set for use.*/
    int (*proto_getsock_func)(qurl_core_t *core_ptr, qurl_conn_t *conn_ptr, qurl_socket_t *socks_ptr);
    /*Multiplexing is called during the DOING phase, returning the fd set for use.*/
    int (*doing_getsock_func)(qurl_core_t *core_ptr, qurl_conn_t *conn_ptr, qurl_socket_t *socks_ptr);
    /*Multiplexing is called during the DO_MORE phase, returning the fd set for use.*/
    int (*domore_getsock_func)(qurl_core_t *core_ptr, qurl_conn_t *conn_ptr, qurl_socket_t *socks_ptr);
    /*Called from the multiplexing interface during the DO_DONE, PERFORM, and WAITPERFORM phases to return the fd set. If this function is not set, qurl will use a generic default.*/
    int (*perform_getsock_func)(qurl_core_t *core_ptr, qurl_conn_t *conn_ptr, qurl_socket_t *socks_ptr);

    /*As one of the protocol-related functions for the disconnection step, called by qurl_disconnect().
     * If the handler is called because the connection is considered to have failed, then dead_connection is set to TRUE.
     * After disconnection, the connection will be (re)associated with the transmission.*/
    qurl_core_t (*disconnect_func)(qurl_core_t *core_ptr, qurl_conn_t *conn_ptr, qurl_bit_t dead_connection);

    /*If used, this function is called from transfer.c:readwrite_data(), allowing the protocol to perform additional read/write operations.*/
    qurl_ecode_t (*readwrite_func)(qurl_core_t *core_ptr, qurl_conn_t *conn_ptr, ssize_t *nread, qurl_bit_t *readmore);

    /*This function can perform various checks on the connection.*/
    unsigned int (*conn_check_func)(qurl_core_t *core_ptr, qurl_conn_t *conn_ptr, unsigned int check_flags);

    /*Attach this transmission to this connection*/
    void (*attach_func)(qurl_core_t *core_ptr, qurl_conn_t *conn_ptr);

    int          def_port; /* default port */
    qurl_prot_t  protocol; /*Protocol Type*/
    qurl_prot_t  family;   /*Protocol Family*/
    unsigned int flags;    /*Additional specific features*/
};
typedef struct qurl_port_prot_s qurl_port_prot_t;

#endif /* Head define end*/
