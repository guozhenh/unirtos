/** @file         qurl_http_multiform.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-11-06 11:37:46
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#ifndef QURL_HTTP_MULTIFORM_H
#define QURL_HTTP_MULTIFORM_H

#include "qurl_config.h"
#include "qurl_code.h"
#include "qurl_def_basic.h"
#include "qurl_list.h"

#if QURL_CFG_ENABLE_HTTP

/**
 * @struct  qurl_http_multiform_state_e
 * @brief Multi-Form Transmission State Machine
 */
typedef enum
{
    QURL_HTTP_MULTIFORM_STATE_NONE,          /*!< Initial state*/
    QURL_HTTP_MULTIFORM_STATE_BOUNDARY,      /*!< Sending boundary field.*/
    QURL_HTTP_MULTIFORM_STATE_HEADER,        /*!< Single form request header.*/
    QURL_HTTP_MULTIFORM_STATE_CONTENT,       /*!< Content of the single form.*/
    QURL_HTTP_MULTIFORM_STATE_BOUNDARY_LAST, /*!< Sending last boundary*/
    QURL_HTTP_MULTIFORM_STATE_DONE,          /*!< Done*/
} qurl_http_multiform_state_e;

/**
 * @brief HTTP single-form control block
 */
struct qurl_http_form_ctrl_s
{
    /*Linked List*/
    qurl_list_node_t node; /*!< Linked list node*/

    /*Basic Form Information*/
    unsigned char    id;           /*!< The ID of this form in the multi-form context*/
    qurl_http_form_e content_type; /*!< Type of content_ptr*/
    long             content_len;  /*!< Length of the content field to be sent*/

    /*Form Runtime Data*/
    int  content_fd; /*!< When content_type is QURL_HTTP_FORM_CONTENT_FILE, it indicates the length of the file to be sent. Note: only -1 is invalid.*/
    long send_index; /*!< The send read index, which is the progress bar for sending data, e.g., 10, indicates that 10 bytes have been sent.*/

    /*Form group packaging data*/
    char                  *name_ptr;          /*!< The name field of the form*/
    char                  *filename_ptr;      /*!< The filename field of the form*/
    void                  *content_ptr;       /*!< Form content/parameters of read_content_func*/
    qurl_http_form_read_cb read_content_func; /*!< The content of a single form, only valid during QURL_HTTP_FORM_CONTENT_CB*/
    qurl_slist_ctrl_t     *headers_slist;     /*!< User-defined header for the form.*/
};
typedef struct qurl_http_form_ctrl_s qurl_http_form_ctrl_t;

/**
 * @brief HTTP Multi-Form Control Block
 */
struct qurl_http_multiform_ctrl_s
{
    /*Multiple Form Information*/
    unsigned char form_id_limit; /*!< [lzm][note]To be released. Number of forms limit: [0:limit)*/
    unsigned char form_id_cnt;   /*!< Number of forms, note the location of the statistics*/

    /*Operating Data*/
    qurl_http_multiform_state_e state;
    long boundary_send_index; /*!< The send read index of the boundary field, which is the progress bar for sending data. For example, 10 indicates that 10 bytes have been sent.*/
    long upload_total_len;    /*!< The total length of the body, including boundaries, used to supply the top-level Content-Length field.*/
    long upload_len;          /*!< Remaining length of the body to be reported*/
    long content_total_len;   /*!< Total content length of multiple forms*/
    long content_upload_len;  /*!< Length of content sent for multi-form*/
    qurl_http_form_ctrl_t *cur_form_ctrl_ptr; /*!< The currently traversed form*/

    /*boundary data*/
    long  boundary_len; /*!< Data length of the boundary_ptr field*/
    char *boundary_ptr; /*!< boundary field data*/

    /*Single Form Single Linked List*/
    qurl_list_t form_list;

    qurl_bit_t boundary_alloc; /*!< Indicates whether the boundary_ptr is dynamically managed by this data structure.*/
};
typedef struct qurl_http_multiform_ctrl_s qurl_http_multiform_ctrl_t;

qurl_ecode_t qurl_http_multiform_init(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr);

qurl_ecode_t qurl_http_multiform_delete(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr);

qurl_ecode_t qurl_http_multiform_set_id_limit(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr, unsigned char max_id_cnt);

char *qurl_http_multiform_get_boundary(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr);

long qurl_http_multiform_get_total_len(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr);

long qurl_http_multiform_get_len(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr);

qurl_ecode_t qurl_http_multiform_set_form(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr, unsigned char id, qurl_http_form_cfg_t *form_cfg_ptr);

qurl_ecode_t qurl_http_multiform_trans_init(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr);

qurl_ecode_t qurl_http_multiform_trans_deinit(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr);

qurl_ecode_t qurl_http_multiform_send_body(qurl_conn_t *conn_ptr);

qurl_bit_t qurl_http_multiform_send_body_is_done(qurl_conn_t *conn_ptr);

#endif

#endif /* Head define end*/
