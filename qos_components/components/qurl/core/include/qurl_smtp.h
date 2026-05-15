/*
 * Portions of this file are derived from curl/libcurl's lib/smtp.h.
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
#ifndef QURL_SMTP_H
#define QURL_SMTP_H

#include "qurl_config.h"
#include "qurl_code_in.h"

#if QURL_CFG_ENABLE_SMTP

extern const qurl_port_prot_t g_qurl_port_prot_smtp;
#if QURL_CFG_ENABLE_TLS
extern const qurl_port_prot_t g_qurl_port_prot_smtps;
#endif

void qurl_smtp_cfg_init(qurl_core_usr_cfg_smtp_t *cfg);
void qurl_smtp_cfg_delete(qurl_core_usr_cfg_smtp_t *cfg);

#endif
#endif /* HEADER_CURL_SMTP_H */
