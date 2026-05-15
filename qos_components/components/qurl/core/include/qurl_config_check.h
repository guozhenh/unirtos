/** @file         qurl_config_check.h
 *  @brief        Brief description: Used to check if macro definition dependencies are normal
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-10-16 16:07:06
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
*/

#ifndef QURL_CONFIG_CHECK_H
#define QURL_CONFIG_CHECK_H

/* fs */
#if (QURL_CFG_ENABLE_FS != 0)

#if (!defined(QURL_CFG_PORT_FOPEN_FUNC) || !defined(QURL_CFG_PORT_FREAD_FUNC) || !defined(QURL_CFG_PORT_FGETSIZE_FUNC) || !defined(QURL_CFG_PORT_FCLOSE_FUNC))
#error "QURL_CFG_ENABLE_FS is enable, but QURL_CFG_PORT_FXXX_FUNC is undefined!!!"
#endif

#endif /*File system check*/

/* tls */
#define QURL_CFG_ENABLE_TLS_CHECK (QURL_CFG_ENABLE_TLS_MBEDTLS + QURL_CFG_ENABLE_TLS_OPENSSL + QURL_CFG_ENABLE_TLS_PLATFORM)
#if (QURL_CFG_ENABLE_TLS_CHECK > 1)
#error "TLS define error"
#endif

/* http */
#if (QURL_CFG_ENABLE_HTTP == 0)

#if (QURL_CFG_ENABLE_HTTP2 != 0)
#error "QURL_CFG_ENABLE_HTTP2 can't be enabled when QURL_CFG_ENABLE_HTTP is 0!!!"
#endif

#endif /*HTTP check*/

#endif /* Head define end*/
