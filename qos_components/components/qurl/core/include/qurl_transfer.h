/** @file         qurl_transfer.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-06 17:21:42
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
*/

#ifndef QURL_TRANSFER_H
#define QURL_TRANSFER_H

#include "qurl_code_in.h"
#include "qurl_def_in.h"
#include "qurl_log.h"

#define qurl_trans_headersep(x) ((((x) == ':') || ((x) == ';')))

/**
 * @struct  qurl_trans_followtype_e
 * @brief Follow Type
 */
typedef enum
{
    QURL_TRANS_FOLLOWTYPE_NONE = 0,
    QURL_TRANS_FOLLOWTYPE_FAKE,  /*!< Only records information, without actually performing the follow operation.*/
    QURL_TRANS_FOLLOWTYPE_RETRY, /*!< This is a request retry, not an actual redirect follow.*/
    QURL_TRANS_FOLLOWTYPE_REDIR, /*!< A complete redirection follow.*/
} qurl_trans_followtype_e;

qurl_ecode_t qurl_trans_init(qurl_core_ctrl_t *core_ptr);

qurl_ecode_t qurl_trans_completed(qurl_core_ctrl_t *core_ptr);

char *qurl_trans_check_headers(qurl_conn_t *conn_ptr, const char *header);

qurl_ecode_t qurl_trans_follow(qurl_core_ctrl_t *core_ptr, char *new_url, qurl_trans_followtype_e type);

#endif /* Head define end*/
