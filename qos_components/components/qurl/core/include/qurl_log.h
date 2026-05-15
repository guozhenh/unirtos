/** @file         qurl_log.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-01 16:46:56
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
*/

#ifndef QURL_LOG_H
#define QURL_LOG_H

#include "qurl_config.h"

#if (QURL_CFG_LOG)

#define QOS_LOG_TAG    LOG_TAG_QURL_API

#define qurl_loge(...) QOSA_LOG_E(QOS_LOG_TAG, ##__VA_ARGS__)
#define qurl_logw(...) QOSA_LOG_W(QOS_LOG_TAG, ##__VA_ARGS__)
#define qurl_logi(...) QOSA_LOG_I(QOS_LOG_TAG, ##__VA_ARGS__)
#define qurl_logd(...) QOSA_LOG_D(QOS_LOG_TAG, ##__VA_ARGS__)
#define qurl_logv(...) QOSA_LOG_V(QOS_LOG_TAG, ##__VA_ARGS__)

#else  /* QURL_CFG_LOG */
#define qurl_log(debug, msg, ...)
#endif /* QURL_CFG_LOG */

#endif /* Head define end*/
