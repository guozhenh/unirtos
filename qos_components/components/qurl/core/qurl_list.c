/** @file         qurl_list.c
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-10-12 15:04:48
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#include "qurl_log.h"
#include "qurl_list.h"

#define QURL_LIST_RIGOR (0)

#define qurl_list_lock(q)
#define qurl_list_unlock(q)
#define qurl_list_lock_init(q)
#define qurl_list_lock_delete(q)

static inline void list_add(qurl_list_node_t *list_node, qurl_list_node_t *new_node)
{
    new_node->prev_ptr = list_node;
    new_node->next_ptr = list_node->next_ptr;
    list_node->next_ptr = new_node;
    new_node->next_ptr->prev_ptr = new_node;
}

static inline void list_delete(qurl_list_node_t *node)
{
    node->prev_ptr->next_ptr = node->next_ptr;
    node->next_ptr->prev_ptr = node->prev_ptr;
    node->next_ptr = QOSA_NULL;
    node->prev_ptr = QOSA_NULL;
}

qurl_ecode_t qurl_list_init(qurl_list_t *list_ptr)
{
    QURL_ASSERT(list_ptr != QOSA_NULL);
    list_ptr->list.next_ptr = (qurl_list_node_t *)(&list_ptr->list);
    list_ptr->list.prev_ptr = (qurl_list_node_t *)(&list_ptr->list);

    list_ptr->cnt = 0;

    qurl_list_lock_init(list_ptr);

    return QURL_OK;
}

qurl_ecode_t qurl_list_node_init(qurl_list_node_t *new_node_ptr)
{
    QURL_ASSERT(new_node_ptr != QOSA_NULL);

    new_node_ptr->next_ptr = QOSA_NULL;
    new_node_ptr->prev_ptr = QOSA_NULL;

    return QURL_OK;
}

int qurl_list_cnt(qurl_list_t *list_ptr)
{
    QURL_ASSERT(list_ptr != QOSA_NULL);

    return list_ptr->cnt;
}

void qurl_list_put_head(qurl_list_t *list_ptr, qurl_list_node_t *new_node_ptr)
{
    QURL_ASSERT(list_ptr != QOSA_NULL);
    QURL_ASSERT(new_node_ptr != QOSA_NULL);

    qurl_list_lock(list_ptr);

    list_add((qurl_list_node_t *)&(list_ptr->list), new_node_ptr);
    list_ptr->cnt++;

    qurl_list_unlock(list_ptr);
    return;
}

void qurl_list_put_tail(qurl_list_t *list_ptr, qurl_list_node_t *new_node_ptr)
{
    QURL_ASSERT(list_ptr != QOSA_NULL);
    QURL_ASSERT(new_node_ptr != QOSA_NULL);

    qurl_list_lock(list_ptr);

    list_add(list_ptr->list.prev_ptr, new_node_ptr);
    list_ptr->cnt++;

    qurl_list_unlock(list_ptr);
    return;
}

void qurl_list_put_next(qurl_list_t *list_ptr, qurl_list_node_t *list_node_ptr, qurl_list_node_t *new_node_ptr)
{
#if QURL_LIST_RIGOR /*Of course, internal components must be confident.*/
    qurl_list_node_t *node_ptr = QOSA_NULL;
#endif

    QURL_ASSERT(list_ptr != QOSA_NULL);
    QURL_ASSERT(list_node_ptr != QOSA_NULL);
    QURL_ASSERT(new_node_ptr != QOSA_NULL);

    qurl_list_lock(list_ptr);

#if QURL_LIST_RIGOR /*Of course, internal components must be confident.*/
    node_ptr = (qurl_list_node_t *)(&list_ptr->list);
    list_node_ptr->next_ptr = new_node_ptr;

    /*This while loop is somewhat time-consuming, but it ensures that the node list_node_ptr exists within list_ptr.*/
    while (node_ptr->next_ptr != new_node_ptr && node_ptr->next_ptr != (qurl_list_node_t *)(&list_ptr->list))
    {
        node_ptr = node_ptr->next_ptr;
        QURL_ASSERT(node_ptr != QOSA_NULL);
        if (node_ptr == QOSA_NULL)
        {
            qurl_list_unlock(list_ptr);
            return;
        }
    }
#endif
    list_add(list_node_ptr, new_node_ptr);

    list_ptr->cnt++;

    qurl_list_unlock(list_ptr);

    return;
}

void qurl_list_put_prev(qurl_list_t *list_ptr, qurl_list_node_t *list_node_ptr, qurl_list_node_t *new_node_ptr)
{
#if QURL_LIST_RIGOR /*Of course, internal components must be confident.*/
    qurl_list_node_t *node_ptr = QOSA_NULL;
#endif

    QURL_ASSERT(list_ptr != QOSA_NULL);
    QURL_ASSERT(list_node_ptr != QOSA_NULL);
    QURL_ASSERT(new_node_ptr != QOSA_NULL);

    qurl_list_lock(list_ptr);

#if QURL_LIST_RIGOR /*Of course, internal components must be confident.*/
    node_ptr = (qurl_list_node_t *)(&list_ptr->list);
    list_node_ptr->prev_ptr->next_ptr = new_node_ptr;

    /*This while loop is somewhat time-consuming, but it ensures that the node list_node_ptr exists within list_ptr.*/
    while (node_ptr->next_ptr != new_node_ptr && node_ptr->next_ptr != (qurl_list_node_t *)(&list_ptr->list))
    {
        node_ptr = node_ptr->next_ptr;
        QURL_ASSERT(node_ptr != QOSA_NULL);
        if (node_ptr == QOSA_NULL)
        {
            qurl_list_unlock(list_ptr);
            return;
        }
    }
#endif
    list_add(list_node_ptr->prev_ptr, new_node_ptr);

    list_ptr->cnt++;

    qurl_list_unlock(list_ptr);

    return;
}

void *qurl_list_get_head(qurl_list_t *list_ptr)
{
    qurl_list_node_t *node_ptr = QOSA_NULL;
    qurl_list_node_t *ret_node_ptr = QOSA_NULL;

    QURL_ASSERT(list_ptr != QOSA_NULL);

    qurl_list_lock(list_ptr);

    node_ptr = list_ptr->list.next_ptr;

    if (list_ptr->cnt > 0 && node_ptr != (qurl_list_node_t *)&(list_ptr->list))
    {
        list_delete(node_ptr);
        list_ptr->cnt--;

        ret_node_ptr = node_ptr;
    }

    qurl_list_unlock(list_ptr);

    return (void *)ret_node_ptr;
}

void *qurl_list_get_tail(qurl_list_t *list_ptr)
{
    qurl_list_node_t *node_ptr = QOSA_NULL;
    qurl_list_node_t *ret_node_ptr = QOSA_NULL;

    QURL_ASSERT(list_ptr != QOSA_NULL);

    qurl_list_lock(list_ptr);

    node_ptr = list_ptr->list.prev_ptr;

    if (list_ptr->cnt > 0 && node_ptr != (qurl_list_node_t *)&(list_ptr->list))
    {
        list_delete(node_ptr);
        list_ptr->cnt--;

        ret_node_ptr = node_ptr;
    }

    qurl_list_unlock(list_ptr);

    return (void *)ret_node_ptr;
}

void *qurl_list_check_head(qurl_list_t *list_ptr)
{
    qurl_list_node_t *node_ptr = QOSA_NULL;
    qurl_list_node_t *ret_node_ptr = QOSA_NULL;

    QURL_ASSERT(list_ptr != QOSA_NULL);

    if (list_ptr == QOSA_NULL)
    {
        return QOSA_NULL;
    }
    qurl_list_lock(list_ptr);

    node_ptr = list_ptr->list.next_ptr;

    if (list_ptr->cnt > 0 && node_ptr != (qurl_list_node_t *)&(list_ptr->list))
    {
        ret_node_ptr = node_ptr;
    }

    qurl_list_unlock(list_ptr);

    return (void *)ret_node_ptr;
}

void *qurl_list_check_tail(qurl_list_t *list_ptr)
{
    qurl_list_node_t *node_ptr = QOSA_NULL;
    qurl_list_node_t *ret_node_ptr = QOSA_NULL;

    QURL_ASSERT(list_ptr != QOSA_NULL);

    qurl_list_lock(list_ptr);

    node_ptr = list_ptr->list.prev_ptr;

    if (list_ptr->cnt > 0 && node_ptr != (qurl_list_node_t *)&(list_ptr->list))
    {
        ret_node_ptr = node_ptr;
    }

    qurl_list_unlock(list_ptr);

    return (void *)ret_node_ptr;
}

void *qurl_list_check_next(qurl_list_t *list_ptr, qurl_list_node_t *node_ptr)
{
    qurl_list_node_t *ret_node_ptr = QOSA_NULL;

    QURL_ASSERT(node_ptr != QOSA_NULL);

    qurl_list_lock(list_ptr);
    if ((void *)node_ptr->next_ptr != (void *)(&list_ptr->list))
    {
        ret_node_ptr = node_ptr->next_ptr;
    }
    qurl_list_unlock(list_ptr);

    return (void *)ret_node_ptr;
}

void *qurl_list_check_prev(qurl_list_t *list_ptr, qurl_list_node_t *node_ptr)
{
    qurl_list_node_t *ret_node_ptr = QOSA_NULL;

    QURL_ASSERT(node_ptr != QOSA_NULL);

    qurl_list_lock(list_ptr);
    if ((void *)node_ptr->prev_ptr != (void *)(&list_ptr->list))
    {
        ret_node_ptr = node_ptr->prev_ptr;
    }
    qurl_list_unlock(list_ptr);

    return (void *)ret_node_ptr;
}

void *qurl_list_search(qurl_list_t *list_ptr, qurl_list_cmp_cb cmp_func, void *cmp_val)
{
    qurl_list_node_t *node_ptr = QOSA_NULL;

    QURL_ASSERT(cmp_func != QOSA_NULL);

    qurl_list_lock(list_ptr);

    node_ptr = (qurl_list_node_t *)qurl_list_check_head(list_ptr);

    while (node_ptr != QOSA_NULL)
    {
        if (cmp_func(node_ptr, cmp_val) != 0)
        {
            return node_ptr;
        }
        node_ptr = (qurl_list_node_t *)qurl_list_check_next(list_ptr, node_ptr);
    }

    qurl_list_unlock(list_ptr);

    return QOSA_NULL;
}

void qurl_list_search_each_safe(qurl_list_t *list_ptr, qurl_list_cmp_cb cmp_func, void *cmp_val)
{
    qurl_list_node_t *pos_node_ptr = QOSA_NULL;
    qurl_list_node_t *next_node_ptr = QOSA_NULL;

    QURL_ASSERT(cmp_func != QOSA_NULL);

    qurl_list_lock(list_ptr);

    for (pos_node_ptr = list_ptr->list.next_ptr, next_node_ptr = pos_node_ptr->next_ptr; pos_node_ptr != &list_ptr->list;
         pos_node_ptr = next_node_ptr, next_node_ptr = pos_node_ptr->next_ptr)
    {
        if (cmp_func(pos_node_ptr, cmp_val) != 0)
        {
            break;
        }
    }

    qurl_list_unlock(list_ptr);
}

/*Note: The sentinel of the concatenated list `concat_list` will be discarded here!!!*/
void qurl_list_concat_head(qurl_list_t *list_ptr, qurl_list_t *concat_list_ptr)
{
    QURL_ASSERT(list_ptr != QOSA_NULL);
    QURL_ASSERT(concat_list_ptr != QOSA_NULL);

    if (list_ptr == QOSA_NULL || concat_list_ptr == QOSA_NULL)
    {
        return;
    }

    qurl_list_lock(list_ptr);

    if (concat_list_ptr->list.prev_ptr == &concat_list_ptr->list)
    {
        qurl_list_unlock(list_ptr);
        return;
    }

    concat_list_ptr->list.prev_ptr->next_ptr = list_ptr->list.next_ptr;
    list_ptr->list.next_ptr->prev_ptr = concat_list_ptr->list.prev_ptr;
    concat_list_ptr->list.next_ptr->prev_ptr = &list_ptr->list;
    list_ptr->list.next_ptr = concat_list_ptr->list.next_ptr;

    concat_list_ptr->list.prev_ptr = &concat_list_ptr->list;
    concat_list_ptr->list.next_ptr = &concat_list_ptr->list;

    list_ptr->cnt += concat_list_ptr->cnt;
    concat_list_ptr->cnt = 0;

    qurl_list_unlock(list_ptr);
}

/*Note: The sentinel of the concatenated list `concat_list` will be discarded here!!!*/
void qurl_list_concat_tail(qurl_list_t *list_ptr, qurl_list_t *concat_list_ptr)
{
    QURL_ASSERT(list_ptr != QOSA_NULL);
    QURL_ASSERT(concat_list_ptr != QOSA_NULL);

    if (list_ptr == QOSA_NULL || concat_list_ptr == QOSA_NULL)
    {
        return;
    }

    qurl_list_lock(list_ptr);

    if (concat_list_ptr->list.prev_ptr == &concat_list_ptr->list)
    {
        qurl_list_unlock(list_ptr);
        return;
    }

    list_ptr->list.prev_ptr->next_ptr = concat_list_ptr->list.next_ptr;
    concat_list_ptr->list.next_ptr->prev_ptr = list_ptr->list.prev_ptr;
    concat_list_ptr->list.prev_ptr->next_ptr = &list_ptr->list;
    list_ptr->list.prev_ptr = concat_list_ptr->list.prev_ptr;

    concat_list_ptr->list.prev_ptr = &concat_list_ptr->list;
    concat_list_ptr->list.next_ptr = &concat_list_ptr->list;

    list_ptr->cnt += concat_list_ptr->cnt;
    concat_list_ptr->cnt = 0;

    qurl_list_unlock(list_ptr);
}

void qurl_list_node_delete(qurl_list_t *list_ptr, qurl_list_node_t *delete_node_ptr)
{
#if QURL_LIST_RIGOR
    qurl_list_node_t *node_ptr = QOSA_NULL;
    int               qcount = 0;
#endif

    QURL_ASSERT(list_ptr != QOSA_NULL);
    QURL_ASSERT(delete_node_ptr != QOSA_NULL);
    qurl_list_lock(list_ptr);

#if QURL_LIST_RIGOR
    for (qcount = list_ptr->cnt, node_ptr = (qurl_list_node_t *)(&list_ptr->list); node_ptr->next_ptr != delete_node_ptr && qcount > 0;
         node_ptr = node_ptr->next_ptr, qcount--)
        ;
    QURL_ASSERT(node_ptr != QOSA_NULL);
    if (qcount <= 0)
    {
        qurl_list_unlock(list_ptr);
        return;
    }
#endif

    list_delete(delete_node_ptr);
    list_ptr->cnt--;

    qurl_list_unlock(list_ptr);
    return;
}

void qurl_list_delete(qurl_list_t *list_ptr)
{
    QURL_ASSERT(list_ptr != QOSA_NULL);

    qurl_list_lock(list_ptr);
    list_ptr->list.next_ptr = QOSA_NULL;
    list_ptr->list.prev_ptr = QOSA_NULL;

    list_ptr->cnt = 0;
    qurl_list_unlock(list_ptr);

    qurl_list_lock_delete(list_ptr);

    return;
}
