/*
 * Portions of this file are derived from curl/libcurl's lib/pingpong.c.
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
/** @file         qurl_pingpong.c
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-11-30 14:09:06
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#include "qurl_config.h"
#include "qurl_code.h"
#include "qurl_def_in.h"
#include "qurl_log.h"
#include "qurl_port.h"
#include "qurl_conn.h"
#include "qurl_core.h"

#include "qurl_select.h"
#include "qurl_pingpong.h"

#include "qurl_default_basic.h"
#if QURL_CFG_ENABLE_PINGPONG

#define QURL_CFG_LOG_PP (QURL_CFG_LOG_DEBUG | QURL_CFG_LOG_FTP)

/*Internal Functions*/
static qurl_ecode_t pp_flushsend(qurl_conn_t *conn_ptr, qurl_pp_ctrl_t *pp_ctrl_ptr)
{
    qurl_ecode_t ret = QURL_OK;
    long         len = 0;

    len = qurl_dbuf_vaild_size(pp_ctrl_ptr->ping_cache);
    if (len > 0)
    {
        /*Push to conn*/
        qurl_dbuf_concat_tail(conn_ptr->conn_conn[QURL_SOCKET_INDEX_0].send_cache, pp_ctrl_ptr->ping_cache);
    }

    ret = qurl_conn_send_cache(conn_ptr, QURL_SOCKET_INDEX_0);
    if (ret != QURL_OK)
    {
        qurl_loge("pp send faild\r\n");
        return ret;
    }

    len = qurl_dbuf_vaild_size(conn_ptr->conn_conn[QURL_SOCKET_INDEX_0].send_cache);
    qurl_logd("pp unsend data len:[%ld]\r\n", len);
    if (len == 0) /*All transmissions completed*/
    {
        /*Record command transmission time*/
        pp_ctrl_ptr->ping_time = qurl_time_now();
    }

    return ret;
}

/* API */
qurl_bit_t qurl_pp_moredata(qurl_pp_ctrl_t *pp_ctrl_ptr)
{
    long pong_cache_size = 0;

    pong_cache_size = qurl_dbuf_vaild_size(pp_ctrl_ptr->pong_cache);

    /*No data sent && receive buffer has unretrieved data*/
    return ((qurl_dbuf_vaild_size(pp_ctrl_ptr->ping_cache) <= 0) && (pong_cache_size > pp_ctrl_ptr->pong_cache_offset)) ? QOSA_TRUE : QOSA_FALSE;
}

qurl_ecode_t qurl_pp_init(qurl_pp_ctrl_t *pp_ctrl_ptr, qurl_pp_cfg_t *pp_cfg_ptr)
{
    qurl_ecode_t ret = 0;

    pp_ctrl_ptr->pong_timeout_ms = pp_cfg_ptr->pong_timeout_ms;
    pp_ctrl_ptr->statemachine_func = pp_cfg_ptr->statemachine_func;
    pp_ctrl_ptr->pong_func = pp_cfg_ptr->pong_func;

    pp_ctrl_ptr->ping_time = qurl_time_now();

    pp_ctrl_ptr->pending_resp = QOSA_TRUE; /*Waiting for server response*/

    ret = qurl_dbuf_new(&pp_ctrl_ptr->ping_cache, QURL_DBUF_NO_LIMIT);
    if (ret != QURL_OK)
    {
        return ret;
    }
    ret = qurl_dbuf_new(&pp_ctrl_ptr->pong_cache, QURL_DBUF_NO_LIMIT);
    if (ret != QURL_OK)
    {
        qurl_dbuf_delete(pp_ctrl_ptr->ping_cache);
        return ret;
    }

    return QURL_OK;
}

qurl_ecode_t qurl_pp_deinit(qurl_pp_ctrl_t *pp_ctrl_ptr)
{
    qurl_dbuf_delete(pp_ctrl_ptr->ping_cache);
    qurl_dbuf_delete(pp_ctrl_ptr->pong_cache);

    return QURL_OK;
}

/**
 * @brief Returns the comprehensive timeout difference, unit ms
 *
 * @param conn_ptr
 * @param pp_ctrl_ptr
 * @param disconnecting
 * @return qurl_time_t <=0: already timed out, >0: remaining time until timeout
 */
qurl_time_t qurl_pp_state_timeout(qurl_conn_t *conn_ptr, qurl_pp_ctrl_t *pp_ctrl_ptr, qurl_bit_t disconnecting)
{
    qurl_timeval_t    now_timeval = {0};
    qurl_time_t       timeout_ms = 0;
    qurl_time_t       core_timeout_ms = 0;
    qurl_core_ctrl_t *core_ptr = conn_ptr->core_ptr;

    /* pingpong */
    now_timeval = qurl_time_now();
    timeout_ms = qurl_time_diff_ms(pp_ctrl_ptr->ping_time, now_timeval);
    timeout_ms = pp_ctrl_ptr->pong_timeout_ms - timeout_ms;

    /* core */
    /** Global timeout set && not yet disconnected*/
    if (core_ptr->usr_cfg.timeout_ms > 0 && !disconnecting)
    {
        core_timeout_ms = qurl_time_diff_ms(core_ptr->start_timeval, now_timeval);
        core_timeout_ms = core_ptr->usr_cfg.timeout_ms - core_timeout_ms;

        timeout_ms = QURL_MIN(timeout_ms, core_timeout_ms);
    }

    return timeout_ms;
}

/**
 * @brief Pingpong protocol stack state machine operation function.
 * The specific protocol stack state machine operation function will only be executed after new data is received.
 *
 * 1. Check and confirm timeout value
 * 2. Check for New Data
 * 3. Enter the protocol stack state machine operation
 *
 * @param conn_ptr
 * @param pp_ctrl_ptr
 * @param block
 * @param disconnecting
 * @return qurl_ecode_t
 */
qurl_ecode_t qurl_pp_state_machine(qurl_conn_t *conn_ptr, qurl_pp_ctrl_t *pp_ctrl_ptr, qurl_bit_t block, qurl_bit_t disconnecting)
{
    qurl_bit_t    deal_flag = QOSA_FALSE;
    qurl_ecode_t  ret = QURL_OK;
    qurl_time_t   timeout_ms = 0;
    qurl_time_t   block_time_ms = 0;
    qurl_socket_t sock = conn_ptr->conn_conn[QURL_SOCKET_INDEX_0].sock_fd; /*Command Connection*/

    timeout_ms = qurl_pp_state_timeout(conn_ptr, pp_ctrl_ptr, disconnecting);
    if (timeout_ms <= 0)
    {
        qurl_loge("timeout:%lld\r\n", timeout_ms);
        return QURL_ECODE_PERFORM_TIMEOUT;
    }

    /*Limit blocking time*/
    if (block)
    {
        block_time_ms = 1000; /* 1s */
        if (block_time_ms > timeout_ms)
        {
            block_time_ms = timeout_ms;
        }
    }
    else
    {
        block_time_ms = 0; /*non-blocking*/
    }

    /*Check if there is any unretrieved data*/
    /** Check if there is data in the lower-level cache*/
    if (qurl_conn_recv_cache_paending(conn_ptr, QURL_SOCKET_INDEX_0) > 0)
    {
        deal_flag = QOSA_TRUE;
    }
    /** Check if the pong cache of pp also has unretrieved data*/
    else if (qurl_pp_moredata(pp_ctrl_ptr))
    {
        deal_flag = QOSA_TRUE;
    }
    /*Check if there is new data in the socket interface*/
    else
    {
        long ping_len = 0;
        long send_len = 0;

        ping_len = qurl_dbuf_vaild_size(pp_ctrl_ptr->ping_cache);
        send_len = qurl_dbuf_vaild_size(conn_ptr->conn_conn[QURL_SOCKET_INDEX_0].send_cache);

        ret = qurl_socket_check(
            sock,
            (ping_len > 0 || send_len > 0) ? (QURL_SELECT_BIT_WRITE | QURL_SELECT_BIT_ERROR) : (QURL_SELECT_BIT_READ | QURL_SELECT_BIT_ERROR),
            block_time_ms
        );
        if (ret < 0)
        {
            qurl_loge("socket faild:0x%x\r\n", ret);
            return QURL_ECODE_PP_ERR;
        }
        if (ret > 0)
        {
            deal_flag = QOSA_TRUE;
        }
        ret = QURL_OK;
    }

    /*The state machine of the protocol stack operates only when there is new data or when data needs to be and can be sent.*/
    if (deal_flag)
    {
        ret = pp_ctrl_ptr->statemachine_func(conn_ptr);
    }

    return ret;
}

/**
 * @brief Format the data and write it into the send buffer
 * @note The string itself should not contain any CRLF, as this function will automatically add them.
 *
 * @param conn_ptr
 * @param pp_ctrl_ptr
 * @param fmt
 * @param ...
 * @return qurl_ecode_t
 */
qurl_ecode_t qurl_pp_sendf(qurl_conn_t *conn_ptr, qurl_pp_ctrl_t *pp_ctrl_ptr, const char *fmt, ...)
{
    qurl_ecode_t ret = QURL_OK;
    va_list      args;

    va_start(args, fmt);
    ret = qurl_dbuf_push_data_vfmt(pp_ctrl_ptr->ping_cache, fmt, args);
    va_end(args);
    if (ret != QURL_OK)
    {
        return ret;
    }

    ret = qurl_dbuf_push_data(pp_ctrl_ptr->ping_cache, "\r\n", 2);
    if (ret != QURL_OK)
    {
        return ret;
    }

#if QURL_CFG_LOG_PP
    {
        char *log_str = QOSA_NULL;
        va_start(args, fmt);
        qurl_vasprintf(&log_str, fmt, args);
        va_end(args);
        qurl_logd("\r\n>>> %s\r\n", log_str);
        qurl_free(log_str);
    }
#endif

    ret = pp_flushsend(conn_ptr, pp_ctrl_ptr);

    return ret;
}

/**
 * @brief Clear pingpong
 *
 * @param conn_ptr
 * @param pp_ctrl_ptr
 * @return qurl_ecode_t
 */
qurl_ecode_t qurl_pp_clear_pingpong(qurl_conn_t *conn_ptr, qurl_pp_ctrl_t *pp_ctrl_ptr)
{
    qurl_ecode_t ret = QURL_OK;
    long         len = 0;

    qurl_logd("enter\r\n");

    if (conn_ptr == QOSA_NULL || pp_ctrl_ptr == QOSA_NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    /*Clear ping*/
    qurl_dbuf_concat_tail(conn_ptr->conn_conn[QURL_SOCKET_INDEX_0].send_cache, pp_ctrl_ptr->ping_cache);
    do {
        len = qurl_dbuf_vaild_size(conn_ptr->conn_conn[QURL_SOCKET_INDEX_0].send_cache);
        if (len <= 0)
        {
            ret = QURL_OK;
            break;
        }

        len = QURL_MIN(len, QURL_CONN_DEAL_CACHE_SIZE);

        qurl_dbuf_pull_data_try(conn_ptr->conn_conn[QURL_SOCKET_INDEX_0].send_cache, conn_ptr->deal_cache_ptr, &len);
        conn_ptr->deal_cache_ptr[len] = 0;

        qurl_logd("clear ping data:[%d]%s\r\n", len, conn_ptr->deal_cache_ptr);
    } while (1);

    /*Clear pong*/
    do {
        ret = qurl_conn_recv_cache(conn_ptr, QURL_SOCKET_INDEX_0, QURL_CONN_DEAL_CACHE_SIZE); /*The second parameter is recommended to be modified.*/
        if (ret != QURL_OK && (qurl_ecode_e)ret != QURL_ECODE_TLS_READ_AGAIN)
        {
            break;
        }
        ret = QURL_OK;
        qurl_dbuf_concat_tail(pp_ctrl_ptr->pong_cache, conn_ptr->conn_conn[QURL_SOCKET_INDEX_0].recv_cache);
        /*[lzm][note]Here you can add a check for ret to see if it is due to the limit restriction of dbuf.*/

        len = qurl_dbuf_vaild_size(pp_ctrl_ptr->pong_cache);
        if (len <= 0)
        {
            break;
        }

        len = QURL_MIN(len, QURL_CONN_DEAL_CACHE_SIZE);

        qurl_dbuf_pull_data_try(pp_ctrl_ptr->pong_cache, conn_ptr->deal_cache_ptr, &len);
        conn_ptr->deal_cache_ptr[len] = 0;

        qurl_logd("clear pong data:[%d]%s\r\n", len, conn_ptr->deal_cache_ptr);
    } while (1);

    return ret;
}

/**
 * @brief Flush the send buffer to trigger data transmission
 *
 * @param conn_ptr
 * @param pp_ctrl_ptr
 * @return qurl_ecode_t
 */
qurl_ecode_t qurl_pp_flushsend(qurl_conn_t *conn_ptr, qurl_pp_ctrl_t *pp_ctrl_ptr)
{
    return pp_flushsend(conn_ptr, pp_ctrl_ptr);
}

/**
 * @brief Fetch data from conn and push it to the application protocol stack
 *
 * @param conn_ptr
 * @param pp_ctrl_ptr
 * @param pong_code_ptr The result code from the peer's pong response parsed by the protocol stack
 * @param size_ptr total length of this pong
 * @return qurl_ecode_t
 */
qurl_ecode_t qurl_pp_read_pong(qurl_conn_t *conn_ptr, qurl_pp_ctrl_t *pp_ctrl_ptr, int *pong_code_ptr, long *size_ptr)
{
    qurl_bit_t        continue_flag = QOSA_FALSE;
    qurl_ecode_t      ret = 0;
    long              last_len = 0;
    long              len = 0;
    long              find_index = 0;
    qurl_ecode_t      ret_select = 0;
    qurl_core_ctrl_t *core_ptr = conn_ptr->core_ptr;

    *pong_code_ptr = 0;
    if (size_ptr != QOSA_NULL)
    {
        *size_ptr = 0;
    }

    /*Check if writable*/
    ret_select = qurl_socket_check(conn_ptr->conn_conn[QURL_SOCKET_INDEX_0].sock_fd, QURL_SELECT_BIT_READ | QURL_SELECT_BIT_ERROR, 0);
    if (ret_select & QURL_SELECT_BIT_ERROR)
    {
        /*[lzm][note]To be optimized. This may be a case where the server has finished sending data and actively closed the data connection. This is normal behavior.*/
        qurl_loge("select ret:%x\r\n", ret_select);
        return QURL_ECODE_SOCK_SELECT_FAILED;
    }

    /*Receive and parse data*/
    continue_flag = QOSA_TRUE;
    do {
        //Loop parsing, exp:
        //220-FileZilla Server 1.9.4
        //220 Please visit https://filezilla-project.org/
        if (last_len <= 0)
        {
            ret = qurl_conn_recv_cache(conn_ptr, QURL_SOCKET_INDEX_0, QURL_CONN_DEAL_CACHE_SIZE); /*The second parameter is recommended to be modified.*/
            if (ret != QURL_OK && (qurl_ecode_e)ret != QURL_ECODE_TLS_READ_AGAIN)
            {
                //qurl_loge("ret =%d\r\n" , ret);
                return ret;
            }
            qurl_dbuf_concat_tail(pp_ctrl_ptr->pong_cache, conn_ptr->conn_conn[QURL_SOCKET_INDEX_0].recv_cache);

            /*[lzm][note]Here you can add a check for ret to see if it is due to the limit restriction of dbuf.*/
            len = qurl_dbuf_vaild_size(pp_ctrl_ptr->pong_cache);
            if (len <= 0)
            {
                /* [lab][202405062321] */
                if ((len == 0 && (ret_select & QURL_SELECT_BIT_READ) && (qurl_ecode_e)ret != QURL_ECODE_TLS_READ_AGAIN)
                    || conn_ptr->conn_conn[QURL_SOCKET_INDEX_0].bits.rx_closed != QOSA_FALSE)
                {
                    /*[lzm][note]pingpong is a functional component,
                     * It is impossible to determine whether the disconnection of TCP is a normal behavior in the current application protocol stack using pingpong.
                     * Therefore, the return value is marked as QURL_ECODE_SOCK_CONN_CLOSED, allowing the caller (application protocol stack) to determine whether it is normal behavior.
                     */
                    qurl_loge("pp connection closed\r\n");
                    return QURL_ECODE_SOCK_CONN_CLOSED;
                }
                return QURL_OK;
            }
        }
        /*Search for "\r\n"*/
        find_index = qurl_dbuf_find(pp_ctrl_ptr->pong_cache, (unsigned char *)"\n", 1, pp_ctrl_ptr->pong_cache_offset);
        if (find_index >= 0)
        {
            /*Process each line individually*/
            qurl_dbuf_pull_data(pp_ctrl_ptr->pong_cache, conn_ptr->deal_cache_ptr, find_index + 1);
            pp_ctrl_ptr->pong_cache_offset = 0;       //dbuf internal offset already applied
            len -= find_index + 1;                    /*Update the remaining value since the last read, used later to determine if new data has arrived.*/

            conn_ptr->deal_cache_ptr[find_index] = 0; /*Append the terminator*/

            qurl_logd("\r\n<<< %s\r\n", conn_ptr->deal_cache_ptr);

            /*Delivered to the user side*/
            if (core_ptr->usr_cfg.write_head_to_client_func != QOSA_NULL)
            {
                long resp_ret_len = 0;
                core_idle_timeout_feed_dog(core_ptr);
                resp_ret_len = core_ptr->usr_cfg.write_head_to_client_func(conn_ptr->deal_cache_ptr, find_index, core_ptr->usr_cfg.write_head_to_client_arg);
                if (resp_ret_len < find_index)
                {
                    return QURL_ECODE_WRITE_HEAD_TO_CLIENT_FAILED;
                }
                QURL_CORE_IDLE_TIMEOUT_CHECK_DOG_FUNC(core_ptr);
            }

            /*Delivered to the protocol stack for parsing*/
            /** Note, the connection's processing cache is used here, so the pong_func function stack must not be written repeatedly.*/
            if (pp_ctrl_ptr->pong_func(conn_ptr, (char *)conn_ptr->deal_cache_ptr, find_index + 1, pong_code_ptr))
            {
                continue_flag = QOSA_FALSE;
            }
            if (size_ptr != QOSA_NULL)
            {
                *size_ptr = find_index + 1;
            }
        }
        else
        {
            pp_ctrl_ptr->pong_cache_offset = len - 1; /*Theoretically, it is also necessary to subtract (matching string length - 1), which here happens to be 0.*/
        }

        last_len = len; /*Save the remaining value from the last read*/

#if 0
        /*[lzm][todo]If the pong_cache is too large and no carriage return is received, only the front part can be retained.*/
        if(continue_flag)
        {
            len = qurl_dbuf_vaild_size(pp_ctrl_ptr->pong_cache);
            if (len > threshold)
            {
                /*Keep the first 40 bytes*/
            }
        }
#endif

        ret_select = 0; /*Mark only the first occurrence as valid*/
    } while (continue_flag);

    pp_ctrl_ptr->pending_resp = QOSA_FALSE; /*Processing completed*/

    return QURL_OK;
}

int qurl_pp_getsock(qurl_conn_t *conn_ptr, qurl_pp_ctrl_t *pp_ctrl_ptr, qurl_socket_t *socks_ptr)
{
    int  bitmap = 0; /*Corresponding to the read and write of the monitored sockfd*/
    long ping_len = 0;
    long send_len = 0;

    if (conn_ptr->conn_conn[QURL_SOCKET_INDEX_0].sock_fd == QURL_SOCKET_BAD)
    {
        return bitmap;
    }
    socks_ptr[0] = conn_ptr->conn_conn[QURL_SOCKET_INDEX_0].sock_fd;

    ping_len = qurl_dbuf_vaild_size(pp_ctrl_ptr->ping_cache);
    send_len = qurl_dbuf_vaild_size(conn_ptr->conn_conn[QURL_SOCKET_INDEX_0].send_cache);

    if (ping_len > 0 || send_len > 0)
    {
        bitmap |= QURL_GETSOCK_WRITESOCK_FUNC(0);
    }
    else
    {
        bitmap |= QURL_GETSOCK_READSOCK_FUNC(0);
    }

    return bitmap;
}

#endif
