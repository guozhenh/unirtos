/** @file         qurl_main.c
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-08-31 16:58:29
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
*/

#include "qurl.h"
#include "qurl_http_app.h"
#include "qurl_http_app_auth.h"
#include "qurl_ftp_app.h"

int main(void)
{
    // qurl_test_unit();

    qurl_global_init();

    /* demo */
    // qurl_http_app_init();
    qurl_ftp_app_init();
    // qurl_http_app_auth_init();
    // qurl_http_app_get_redirect_init();

    // qurl_global_cleanup();

    return 0;
}
