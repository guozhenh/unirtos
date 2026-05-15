/** @file         qurl_list.c
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-10-10 15:04:48
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
*/

#include "qurl_list.h"

/**
 * @brief Plug into the tail
 * @param[in] *data: Must be a dynamically allocated pointer, as qurl_free will be used to release it during cleanup.
 * @retval
 */
qurl_slist_t *qurl_slist_add(qurl_slist_t *list, void *data)
{
    qurl_slist_t *last_node = NULL;
    qurl_slist_t *new_node = NULL;

    new_node = qurl_malloc(sizeof(qurl_slist_t));
    if (new_node == NULL)
    {
        return NULL; /*Failed*/
    }
    new_node->data = data;
    new_node->next = NULL;

    if (list == NULL)
    {
        return new_node; /*You are the boss*/
    }

    last_node = list;
    while (last_node->next != NULL)
    {
        last_node = last_node->next;
    }
    last_node->next = new_node;

    return list;
}

/**
 * @brief Inserting into a linked list by copying a string
 * @retval
 */
qurl_slist_t *qurl_slist_add_strdup(qurl_slist_t *list, char *str)
{
    char *data = NULL;
    int   str_len = strlen(str);

    if (str == NULL || str_len == 0)
    {
        return NULL; /*Insertion failed*/
    }

    data = (char *)qurl_malloc(str_len + 1);
    if (data == NULL)
    {
        return NULL;
    }
    data[str_len] = '\0';
    memcpy(data, str, str_len);

    list = qurl_slist_add(list, data);
    if (list == NULL)
    {
        qurl_free(data);
    }

    return list;
}

void qurl_slist_del_all(qurl_slist_t *list)
{
    qurl_slist_t *next = NULL;
    qurl_slist_t *node = NULL;

    if (list == NULL)
    {
        return;
    }

    node = list;
    do {
        next = node->next;
        if (node->next != NULL)
        {
            qurl_free(node->data);
        }
        free(node);
        node = next;
    } while (next);
}

/*General Purpose Bidirectional Circular Linked List*/
int qurl_list_init(qurl_list_t *list)
{
    if (list == NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    list->prev = list;
    list->next = list;

    return QURL_OK;
}

qurl_bit_t qurl_list_is_empty(qurl_list_t *list)
{
    return (qurl_bit_t)(list->next == list);
}

void qurl_list_add(qurl_list_t *list_node, qurl_list_t *new_node)
{
    new_node->prev = list_node;
    new_node->next = list_node->next;
    list_node->next = new_node;
    new_node->next->prev = new_node;
}

void qurl_list_add_head(qurl_list_t *list, qurl_list_t *node)
{
    qurl_list_add(list, node);
}

void qurl_list_add_tail(qurl_list_t *list, qurl_list_t *node)
{
    qurl_list_add(list->prev, node);
}

void qurl_list_delete(qurl_list_t *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = NULL;
    node->prev = NULL;
}

void qurl_list_delete_head(qurl_list_t *list)
{
    if (!qurl_list_is_empty(list))
    {
        qurl_list_delete(list->next);
    }
}

void qurl_list_delete_tail(qurl_list_t *list)
{
    if (!qurl_list_is_empty(list))
    {
        qurl_list_delete(list->prev);
    }
}

/*Note: The sentinel of the concatenated list `concat_list` will be discarded here!!!*/
void qurl_list_concat(qurl_list_t *list, qurl_list_t *concat_list)
{
    if (list == NULL || concat_list == NULL)
    {
        return;
    }

    list->prev->next = concat_list->next;
    concat_list->next->prev = list->prev;
    list->prev = concat_list->prev;
    concat_list->prev->next = list;
}
