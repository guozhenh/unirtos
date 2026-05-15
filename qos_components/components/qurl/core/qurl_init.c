/** @file         qurl_init.c
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-01 13:37:42
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#include "qurl_def_in.h"
#include "qurl_log.h"
#include "qurl_code_in.h"
#include "qurl_core.h"
#include "qurl_tls.h"

static unsigned int g_qurl_global_init_num = 0;

/**
 * @brief Global initialization of the qurl library.
 * @note This API is not thread-safe.
 * @return Return the ecode
 */
qurl_ecode_t qurl_global_init(void)
{
    qurl_ecode_t ret = QURL_OK;

    qurl_logd("init:[%d]\r\n", g_qurl_global_init_num + 1);

    if (g_qurl_global_init_num++)
    {
        return QURL_OK;
    }

    /*[lzm][note]Global library initialization content can be implemented here.*/
#if QURL_CFG_ENABLE_TLS
    qurl_tls_init();
#endif /* QURL_CFG_ENABLE_TLS */

    ret = qurl_core_sec_init();

    return ret;
}

/**
 * @brief Global deinitialization of the qurl library.
 * @note This API is not thread-safe.
 * @return Return the ecode
 */
qurl_ecode_t qurl_global_deinit(void)
{
    qurl_logd("init:[%d]\r\n", g_qurl_global_init_num - 1);

    if (--g_qurl_global_init_num > 0)
    {
        return QURL_OK;
    }

    /*[lzm][note]Here, the global library de-initialization content can be implemented.*/
    qurl_core_sec_deinit();
#if QURL_CFG_ENABLE_TLS
    qurl_tls_deinit();
#endif /* QURL_CFG_ENABLE_TLS */

    return QURL_OK;
}
