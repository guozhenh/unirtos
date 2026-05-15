/** @file         qurl_default_basic.h
 *  @brief        Brief description: Some basic functions provided internally, which can be overridden.
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-11-17 10:40:42
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
*/

#ifndef QURL_DEFAULT_H
#define QURL_DEFAULT_H

#include "qurl_config.h"

int qurl_default_vasprintf(char **str, const char *format, va_list args);

#ifndef QURL_CFG_PORT_ASPRINTF_FUNC
int qurl_default_asprintf(char **str, const char *format, ...);
#endif

#ifndef QURL_CFG_PORT_ASPRINTF_FUNC
int qurl_default_strtoofft(const char *str, char **endp, int base, long *num);
#endif

#ifndef QURL_CFG_PORT_TLS_BASE64_ENCODE_FUNC
qurl_ecode_t qurl_default_base64_encode(const unsigned char *src_data, long src_len, unsigned char *dst_buf, long dst_buf_len, long *dst_len);
#endif

#ifndef QURL_CFG_PORT_TLS_BASE64_DECODE_FUNC
qurl_ecode_t qurl_default_base64_decode(const unsigned char *src_data, long src_len, unsigned char *dst_buf, long dst_buf_len, long *dst_len);
#endif

#endif /* Head define end*/
