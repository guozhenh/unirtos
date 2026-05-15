/** @file         qurl_http_app.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-05 21:06:43
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
*/

#ifndef QURL_HTTP_APP_AUTH_H
#define QURL_HTTP_APP_AUTH_H

#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "qurl_apps_config.h"

#ifdef QURL_CFG_APPS_HTTP

#include "qurl.h"

qurl_ecode_t qurl_http_app_auth_init(void);

#endif /* QURL_HTTP_APP_AUTH_H */
#endif /* Head define end*/
