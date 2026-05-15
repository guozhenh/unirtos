/** @file         qurl_conn.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-11 15:15:04
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
*/

#ifndef QURL_CONN_H
#define QURL_CONN_H

#include "qurl_code_in.h"
#include "qurl_def_in.h"
#include "qurl_log.h"

#if QURL_CFG_ENABLE_TLS
qurl_ecode_t qurl_conn_upgrade_to_tls(qurl_conn_t *conn_ptr, qurl_core_ctrl_t *core_ptr);
#endif /* QURL_CFG_ENABLE_TLS */

qurl_ecode_t qurl_conn_create(qurl_core_ctrl_t *core_ptr);

qurl_ecode_t qurl_conn_delete(qurl_conn_t *conn_ptr);

qurl_ecode_t qurl_conn_listen(qurl_conn_t *conn_ptr, int backlog);

qurl_ecode_t qurl_conn_accept(qurl_conn_t *conn_ptr, int sock_index);

qurl_bit_t qurl_conn_is_connected(qurl_conn_t *conn_ptr, int sock_index);

qurl_ecode_t qurl_conn_connect(qurl_conn_t *conn_ptr, int sock_index, qurl_bit_t *done_ptr);

qurl_ecode_t qurl_conn_close(qurl_conn_t *conn_ptr, int sock_index);

qurl_ecode_t qurl_conn_conn_free(qurl_conn_t *conn_ptr, int sock_index);

qurl_ecode_t qurl_conn_send_cache(qurl_conn_t *conn_ptr, int sock_index);

qurl_ecode_t qurl_conn_recv_cache(qurl_conn_t *conn_ptr, int sock_index, long max_size);

long qurl_conn_recv_cache_paending(qurl_conn_t *conn_ptr, int sock_index);

qurl_ecode_t qurl_conn_core_resolve_check(qurl_conn_t *conn_ptr);

qurl_ecode_t qurl_conn_core_resolve(qurl_conn_t *conn_ptr);

qurl_ecode_t qurl_conn_core_connect(qurl_conn_t *conn_ptr);

qurl_ecode_t qurl_conn_core_connected_check(qurl_conn_t *conn_ptr, int sock_index, qurl_bit_t *connected_ptr);

qurl_ecode_t qurl_conn_core_prot_connected_check(qurl_conn_t *conn_ptr, int sock_index, qurl_bit_t *connected_ptr);

qurl_ecode_t qurl_conn_core_send_prot_connect(qurl_conn_t *conn_ptr, qurl_bit_t *done);

qurl_ecode_t qurl_conn_core_prot_connect(qurl_conn_t *conn_ptr, qurl_bit_t *done);

qurl_ecode_t qurl_conn_core_req(qurl_conn_t *conn_ptr, qurl_bit_t *done);

qurl_ecode_t qurl_conn_core_reqing(qurl_conn_t *conn_ptr, qurl_bit_t *done);

qurl_ecode_t qurl_conn_core_req_more(qurl_conn_t *conn_ptr, int *completep_ptr);

qurl_ecode_t qurl_conn_core_trans_data(qurl_conn_t *conn_ptr, qurl_bit_t *done);

qurl_ecode_t qurl_conn_core_trans_done(qurl_conn_t *conn_ptr, qurl_ecode_t ecode, qurl_bit_t premature);

#endif /* Head define end*/
