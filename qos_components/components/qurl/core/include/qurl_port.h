/** @file         qurl_port.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-12 17:47:16
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
*/

#ifndef QURL_PORT_H
#define QURL_PORT_H

#include "qurl_def_in.h"
#include "qurl_platform_port.h"

qurl_ecode_t qurl_port_prot_match_by_scheme(const char *scheme, const qurl_port_prot_t **protocol_pptr);

qurl_timeval_t qurl_time_now(void);

qurl_time_t qurl_time_diff_ms(qurl_timeval_t old_timeval, qurl_timeval_t new_timeval);

#if defined(QURL_CFG_PORT_SOCKET_RESOLVE_FUNC) && !defined(QURL_CFG_PORT_SOCKET_RESOLVE_ASYNC_FUNC)
qurl_ecode_t qurl_socket_resolve_async(const char *hostname, qurl_resolve_cb found, void *callback_arg, unsigned char dns_addrtype, int nw_id);
#endif

#endif /* Head define end*/
