/** @file         qurl_select.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-25 14:57:00
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
*/

#ifndef QURL_SELECT_H
#define QURL_SELECT_H

#include "qurl_def_in.h"

#define QURL_SELECT_BIT_READ   0x01
#define QURL_SELECT_BIT_WRITE  0x02
#define QURL_SELECT_BIT_ERROR  0x04
#define QURL_SELECT_BIT_RWE    (QURL_SELECT_BIT_READ | QURL_SELECT_BIT_WRITE | QURL_SELECT_BIT_ERROR)

#define qurl_socket_reable(x)  qurl_socket_check(x, QURL_SELECT_BIT_READ | QURL_SELECT_BIT_ERROR, 0)
#define qurl_socket_weable(x)  qurl_socket_check(x, QURL_SELECT_BIT_WRITE | QURL_SELECT_BIT_ERROR, 0)
#define qurl_socket_rweable(x) qurl_socket_check(x, QURL_SELECT_BIT_RWE, 0)

qurl_ecode_t qurl_select(qurl_socket_t maxfd, fd_set *fds_read, fd_set *fds_write, fd_set *fds_err, qurl_time_t timeout_ms);

qurl_ecode_t qurl_socket_check(qurl_socket_t checkfd, unsigned char bitmaps, qurl_time_t timeout_ms);

#endif /* Head define end*/
