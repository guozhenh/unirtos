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

#include "qurl_code.h"

/**
 * @struct  qurl_slist_t
 * @brief Non-universal unidirectional linked list data structure of qurl
 */
struct qurl_slist_s
{
    void                *data;
    struct qurl_slist_s *next;
};
typedef struct qurl_slist_s qurl_slist_t;

typedef struct qurl_list_s
{
    struct qurl_list_s *prev;
    struct qurl_list_s *next;
} qurl_list_t;

int qurl_list_init(qurl_list_t *list);

qurl_bit_t qurl_list_is_empty(qurl_list_t *list);

void qurl_list_add(qurl_list_t *list_node, qurl_list_t *new_node);

void qurl_list_add_head(qurl_list_t *list, qurl_list_t *node);

void qurl_list_add_tail(qurl_list_t *list, qurl_list_t *node);

void qurl_list_delete(qurl_list_t *node);

void qurl_list_delete_head(qurl_list_t *list);

void qurl_list_delete_tail(qurl_list_t *list);

void qurl_list_concat(qurl_list_t *list, qurl_list_t *concat_list);

#define qurl_list_first(list)          ((list)->next)
#define qurl_list_is_first(list, node) ((list)->next == node)

#define qurl_list_last(list)           ((list)->prev)
#define qurl_list_is_last(list, node)  ((list)->prev == node)

#define qurl_list_for_each(pos, list)  for (pos = (list)->next; (pos) != (list); pos = (pos)->next)

#define qurl_list_for_each_safe(pos, next_node, list)                                                                                                          \
    for (pos = (list)->next, next_node = (pos)->next; (pos) != (list); pos = next_node, next_node = (pos)->next)

#define qurl_list_for_each_prev(pos, list) for (pos = (list)->prev; pos != (list); pos = pos->prev)

#define qurl_list_for_each_prev_safe(pos, prev_node, list)                                                                                                     \
    for (pos = (list)->prev, prev_node = (pos)->prev; (pos) != (list); pos = prev_node, prev_node = (pos)->prev)

#define qurl_list_entry(node, type, member) ((type *)((char *)(node) - ((unsigned long)&((type *)0)->member)))

#define qurl_list_for_each_entry(node, list, type, member)                                                                                                     \
    for (node = qurl_list_entry((list)->next, type, member); &(node)->member != (list); node = qurl_list_entry((node)->member.next, type, member))

#define qurl_list_for_each_entry_safe(node, next_node, list, type, member)                                                                                     \
    for (node = qurl_list_entry((list)->next, type, member), next_node = qurl_list_entry((node)->member.next, type, member); &(node)->member != (list);        \
         node = next_node, next_node = qurl_list_entry((node)->member.next, type, member))

#define qurl_list_for_each_prev_entry(node, list, type, member)                                                                                                \
    for (node = qurl_list_entry((list)->prev, type, member); &(node)->member != (list); node = qurl_list_entry((node)->member.prev, type, member))

#define qurl_list_for_each_prev_entry_safe(node, prev_node, list, type, member)                                                                                \
    for (node = qurl_list_entry((list)->prev, type, member), prev_node = qurl_list_entry((node)->member.prev, type, member); &(node)->member != (list);        \
         node = prev_node, prev_node = qurl_list_entry((node)->member.prev, type, member))

#endif /* Head define end*/
