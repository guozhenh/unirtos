/** @file         qurl_io_socket.c
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-11-08 20:50:08
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#include "qurl_config.h"
#include "qurl_code_in.h"
#include "qurl_log.h"
#include "qurl_def_in.h"

qurl_ecode_t io_socket_open(qurl_conn_t *conn_ptr)
{
    return QURL_OK;
}

const qurl_port_io_t g_qurl_port_io_socket = {
    .name = "socket",
    .open_func = io_socket_open,
};
