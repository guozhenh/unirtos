/** @file         qurl_mbedtls.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-11-09 20:19:51
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
*/

#ifndef QURL_MBEDTLS_H
#define QURL_MBEDTLS_H

#include "qurl_tls_def.h"

#if (QURL_CFG_ENABLE_TLS && QURL_CFG_ENABLE_TLS_MBEDTLS)

extern const qurl_tls_adapter_t g_tls_port_mbedtls;

#endif

#endif /* Head define end*/
