/**
 * \file net.h
 *
 * \brief Deprecated header file that includes net_sockets.h
 *
 * \deprecated Superseded by mbedtls/net_sockets.h
 */
/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */
#if !defined(CONFIG_QCM_MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include "config-mini-tls1_1.h"
#endif

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
#include "mbedtls/net_sockets.h"
#if defined(MBEDTLS_DEPRECATED_WARNING)
#warning "Deprecated header file: Superseded by mbedtls/net_sockets.h"
#endif /* MBEDTLS_DEPRECATED_WARNING */
#endif /* !MBEDTLS_DEPRECATED_REMOVED */
