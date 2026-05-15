/** @file         qurl_dbuf.h
 *  @brief        Brief description: dbuf, which stands for dynamic data packet, dynamic\data
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-10-11 16:08:54
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#ifndef QURL_DBUF_H
#define QURL_DBUF_H

#include "qurl_code_in.h"
#include "qurl_def.h"
#include "qurl_def_basic.h"
#include "qurl_list.h"

/*dbuf limitation*/
#define QURL_DBUF_NO_LIMIT      (-1)

/*The minimum granularity of each dbuf*/
#define QURL_DBUF_NODE_MIN_SIZE (256)

/*Open and closed source usage*/
typedef void *qurl_dbuf_t;

struct qurl_dbuf_ctrl_s
{
    long        limit_size; /*!< The size limit of dbuf*/
    long        total_size; /*!< The total space size of the dbuf chain. Note: The available free space for writing is not simply (total - valid); the gaps in between must also be considered.*/
    long        valid_size; /*!< The valid data size of the dbuf chain*/
    qurl_list_t data_list;  /*!< Current data linked list of dbuf*/
};
typedef struct qurl_dbuf_ctrl_s qurl_dbuf_ctrl_t;

struct qurl_dbuf_node_s
{
    qurl_list_node_t node;     /*!< Linked list node*/

    long           total_size; /*!< Total size of the dbuf node*/
    long           valid_size; /*!< Valid data size of the dbuf node*/
    long           r_index;    /*!< Read cursor*/
    long           w_index;    /*!< Write cursor*/
    unsigned char *buf;        /*!< Data space*/
};
typedef struct qurl_dbuf_node_s qurl_dbuf_node_t;

qurl_ecode_t qurl_dbuf_new(qurl_dbuf_t *dbuf_ptr, long limit_size);

qurl_ecode_t qurl_dbuf_delete(qurl_dbuf_t dbuf);

long qurl_dbuf_vaild_size(qurl_dbuf_t dbuf);

qurl_ecode_t qurl_dbuf_concat_head(qurl_dbuf_t dbuf, qurl_dbuf_t concat_dbuf);

qurl_ecode_t qurl_dbuf_concat_tail(qurl_dbuf_t dbuf, qurl_dbuf_t concat_dbuf);

qurl_ecode_t qurl_dbuf_push_data(qurl_dbuf_t dbuf, void *data_ptr, long data_size);

qurl_ecode_t qurl_dbuf_push_data_vfmt(qurl_dbuf_t dbuf, const char *fmt, va_list args);

qurl_ecode_t qurl_dbuf_push_data_fmt(qurl_dbuf_t dbuf, const char *fmt, ...);

qurl_ecode_t qurl_dbuf_pull_data(qurl_dbuf_t dbuf, void *data_ptr, long pull_size);

qurl_ecode_t qurl_dbuf_pull_data_try(qurl_dbuf_t dbuf, void *data_ptr, long *pull_size_ptr);

qurl_ecode_t qurl_dbuf_clear_data(qurl_dbuf_t dbuf);

qurl_ecode_t qurl_dbuf_read_data(qurl_dbuf_t dbuf, void *data_ptr, long read_size, long offset);

qurl_ecode_t qurl_dbuf_read_data_try(qurl_dbuf_t dbuf, void *data_ptr, long *read_size_ptr, long offset);

qurl_ecode_t qurl_dbuf_find(qurl_dbuf_t dbuf, unsigned char *data_ptr, unsigned char data_len, long offset);

#endif /* Head define end*/
