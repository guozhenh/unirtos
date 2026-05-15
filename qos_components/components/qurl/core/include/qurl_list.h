/** @file         qurl_list.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-10-10 15:05:46
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#ifndef QURL_LIST_H
#define QURL_LIST_H

#include "qurl_code_in.h"
#include "qurl_def.h"

/*Node*/
struct qurl_list_node_s
{
    struct qurl_list_node_s *next_ptr;
    struct qurl_list_node_s *prev_ptr;
};
typedef struct qurl_list_node_s qurl_list_node_t;

/*Sentry*/
struct qurl_list_s
{
    qurl_list_node_t list;
    int              cnt;
};
typedef struct qurl_list_s qurl_list_t;

typedef int (*qurl_list_cmp_cb)(void *node_ptr, void *cmp_val);

qurl_ecode_t qurl_list_init(qurl_list_t *list_ptr);

qurl_ecode_t qurl_list_node_init(qurl_list_node_t *new_node_ptr);

int qurl_list_cnt(qurl_list_t *list_ptr);

void qurl_list_put_head(qurl_list_t *list_ptr, qurl_list_node_t *new_node_ptr);

void qurl_list_put_tail(qurl_list_t *list_ptr, qurl_list_node_t *new_node_ptr);

void qurl_list_put_next(qurl_list_t *list_ptr, qurl_list_node_t *insert_node_ptr, qurl_list_node_t *new_node_ptr);

void qurl_list_put_prev(qurl_list_t *list_ptr, qurl_list_node_t *insert_node_ptr, qurl_list_node_t *new_node_ptr);

void *qurl_list_get_head(qurl_list_t *list_ptr);

void *qurl_list_get_tail(qurl_list_t *list_ptr);

void *qurl_list_check_head(qurl_list_t *list_ptr);

void *qurl_list_check_tail(qurl_list_t *list_ptr);

void *qurl_list_check_next(qurl_list_t *list_ptr, qurl_list_node_t *node_ptr);

void *qurl_list_check_prev(qurl_list_t *list_ptr, qurl_list_node_t *node_ptr);

void *qurl_list_search(qurl_list_t *list_ptr, qurl_list_cmp_cb cmp_func, void *cmp_val);

void qurl_list_search_each_safe(qurl_list_t *list_ptr, qurl_list_cmp_cb cmp_func, void *cmp_val);

void qurl_list_concat_head(qurl_list_t *list_ptr, qurl_list_t *concat_list_ptr);

void qurl_list_concat_tail(qurl_list_t *list_ptr, qurl_list_t *concat_list_ptr);

void qurl_list_node_delete(qurl_list_t *list_ptr, qurl_list_node_t *delete_node_ptr);

void qurl_list_delete(qurl_list_t *list_ptr);

#define qurl_list_node_entry(node, type, member) ((type *)((char *)(node) - ((unsigned long)&((type *)0)->member)))

#endif /* Head define end*/
