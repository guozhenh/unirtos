/*
 * Portions of this file are derived from curl/libcurl's lib/ftp.h.
 *
 * Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * should be included in the curl distribution. The terms are also
 * available at https://curl.se/docs/copyright.html.
 *
 * For qurl redistribution, see:
 * - qos_components/components/qurl/THIRD_PARTY_NOTICES.md
 * - qos_components/components/qurl/licenses/curl.txt
 */
/** @file         qurl_ftp.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-11-28 20:34:13
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
*/

#ifndef QURL_FTP_H
#define QURL_FTP_H

#include "qurl_config.h"
#include "qurl_code_in.h"

#if QURL_CFG_ENABLE_FTP

extern const qurl_port_prot_t g_qurl_port_prot_ftp;

extern const qurl_port_prot_t g_qurl_port_prot_ftps;

#endif

#endif /* Head define end*/
