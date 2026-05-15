/*
 * Portions of this file are derived from curl/libcurl's lib/pingpong.h.
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
/** @file         qurl_pingpong.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-11-30 14:09:21
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#ifndef QURL_PINGPONG_H
#define QURL_PINGPONG_H

#include "qurl_config.h"
#include "qurl_code_in.h"
#include "qurl_def.h"
#include "qurl_def_in.h"

#if QURL_CFG_ENABLE_PINGPONG

/**
 * Transmitted content
 */
typedef enum
{
    QURL_PP_TRANSFER_BODY, /*!< Indicates that the entire response body data needs to be transmitted.*/
    QURL_PP_TRANSFER_INFO, /*!< Indicates that only the response header information needs to be obtained, and the response body data does not need to be transmitted.*/
    QURL_PP_TRANSFER_NONE, /*!< Indicates that no response data needs to be obtained, meaning neither the response header information nor the response body data is required.*/
} qurl_pp_transfer_e;

/**
 * @struct qurl_pp_cfg_t
 * @brief PingPong configuration block
 */
struct qurl_pp_cfg_s
{
    qurl_time_t pong_timeout_ms; /*!< Command response timeout value*/

    /*Protocol Stack Functionality*/
    qurl_ecode_t (*statemachine_func)(qurl_conn_t *conn_ptr);                                    /*!< Protocol stack state machine operation function*/
    qurl_bit_t (*pong_func)(qurl_conn_t *conn_ptr, char *buf, long buf_len, int *pong_code_ptr); /*!< pong, receives data from the lower layer to the protocol stack, and confirms whether a complete response has been received.*/
};
typedef struct qurl_pp_cfg_s qurl_pp_cfg_t;

/**
 * @struct qurl_pp_ctrl_t
 * @brief PingPong control block
 */
struct qurl_pp_ctrl_s
{
    /*Information*/
    qurl_timeval_t ping_time;       /*!< Time when the command transmission is completed*/
    qurl_time_t    pong_timeout_ms; /*!< Command response timeout value*/
    /*Status*/
    qurl_bit_t pending_resp; /*!< Set to TRUE when the server response is pending or in progress; clear this flag after the last response has been read.*/
    /*Cache*/
    long        pong_cache_offset; /*!< The read offset of pong_cache, note that this value must be checked and updated after each read of pong_cache.*/
    qurl_dbuf_t ping_cache;        /*!< Ping send buffer in pingpong*/
    qurl_dbuf_t pong_cache;        /*!< Pong receive buffer in pingpong*/

    /*Protocol Stack Functionality*/
    qurl_ecode_t (*statemachine_func)(qurl_conn_t *conn_ptr);                                    /*!< Protocol stack state machine operation function*/
    qurl_bit_t (*pong_func)(qurl_conn_t *conn_ptr, char *buf, long buf_len, int *pong_code_ptr); /*!< pong, receives data from the lower layer to the protocol stack, and confirms whether a complete response has been received.*/
};
typedef struct qurl_pp_ctrl_s qurl_pp_ctrl_t;

qurl_ecode_t qurl_pp_init(qurl_pp_ctrl_t *pp_ctrl_ptr, qurl_pp_cfg_t *pp_cfg_ptr);

qurl_ecode_t qurl_pp_deinit(qurl_pp_ctrl_t *pp_ctrl_ptr);

qurl_bit_t qurl_pp_moredata(qurl_pp_ctrl_t *pp_ctrl_ptr);

qurl_time_t qurl_pp_state_timeout(qurl_conn_t *conn_ptr, qurl_pp_ctrl_t *pp_ctrl_ptr, qurl_bit_t disconnecting);

qurl_ecode_t qurl_pp_state_machine(qurl_conn_t *conn_ptr, qurl_pp_ctrl_t *pp_ctrl_ptr, qurl_bit_t block, qurl_bit_t disconnecting);

qurl_ecode_t qurl_pp_sendf(qurl_conn_t *conn_ptr, qurl_pp_ctrl_t *pp_ctrl_ptr, const char *fmt, ...);

qurl_ecode_t qurl_pp_clear_pingpong(qurl_conn_t *conn_ptr, qurl_pp_ctrl_t *pp_ctrl_ptr);

qurl_ecode_t qurl_pp_flushsend(qurl_conn_t *conn_ptr, qurl_pp_ctrl_t *pp_ctrl_ptr);

qurl_ecode_t qurl_pp_read_pong(qurl_conn_t *conn_ptr, qurl_pp_ctrl_t *pp_ctrl_ptr, int *pong_code_ptr, long *size_ptr);

int qurl_pp_getsock(qurl_conn_t *conn_ptr, qurl_pp_ctrl_t *pp_ctrl_ptr, qurl_socket_t *socks_ptr);

#endif

#endif /* Head define end*/
