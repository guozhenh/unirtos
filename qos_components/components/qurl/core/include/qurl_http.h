/** @file         qurl_http.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-21 16:53:48
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#ifndef QURL_HTTP_H
#define QURL_HTTP_H

#include "qurl_config.h"
#include "qurl_code_in.h"

#if QURL_CFG_ENABLE_HTTP

extern const qurl_port_prot_t g_qurl_port_prot_http;
#if QURL_CFG_ENABLE_TLS
extern const qurl_port_prot_t g_qurl_port_prot_https;
#endif

#endif

#endif /* Head define end*/
