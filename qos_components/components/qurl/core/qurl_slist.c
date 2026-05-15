/** @file         qurl_slist.c
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

#include "qurl_config.h"
#include "qurl_def_basic.h"
#include "qurl_slist.h"

/**
 * @brief Plug into the tail
 * @param[in] *data: Must be a dynamically allocated pointer, as qurl_free will be used to release it during cleanup.
 * @retval
 */
qurl_slist_t qurl_slist_add(qurl_slist_t slist, void *data)
{
    qurl_slist_ctrl_t *last_node = QOSA_NULL;
    qurl_slist_ctrl_t *new_node = QOSA_NULL;
    qurl_slist_ctrl_t *list = (qurl_slist_ctrl_t *)slist;

    new_node = qurl_malloc(sizeof(qurl_slist_ctrl_t));
    if (new_node == QOSA_NULL)
    {
        return QOSA_NULL; /*Failed*/
    }
    new_node->data = data;
    new_node->next = QOSA_NULL;

    if (list == QOSA_NULL)
    {
        return new_node; /*You are the boss*/
    }

    last_node = list;
    while (last_node->next != QOSA_NULL)
    {
        last_node = last_node->next;
    }
    last_node->next = new_node;

    return list;
}

/**
 * @brief Inserting a linked list by copying a string
 * @retval
 */
qurl_slist_t qurl_slist_add_strdup(qurl_slist_t slist, char *str)
{
    char              *data = QOSA_NULL;
    int                str_len = 0;
    qurl_slist_ctrl_t *list = (qurl_slist_ctrl_t *)slist;

    if (str == QOSA_NULL || qurl_strlen(str) == 0)
    {
        return QOSA_NULL; /*Insertion failed*/
    }
    str_len = qurl_strlen(str);
    data = (char *)qurl_malloc(str_len + 1);
    if (data == QOSA_NULL)
    {
        return QOSA_NULL;
    }
    data[str_len] = '\0';
    qurl_memcpy(data, str, str_len);

    list = qurl_slist_add(list, data);
    if (list == QOSA_NULL)
    {
        qurl_free(data);
    }

    return list;
}

void qurl_slist_del_all(qurl_slist_t slist)
{
    qurl_slist_ctrl_t *next = QOSA_NULL;
    qurl_slist_ctrl_t *node = QOSA_NULL;
    qurl_slist_ctrl_t *list = (qurl_slist_ctrl_t *)slist;

    if (list == QOSA_NULL)
    {
        return;
    }

    node = list;
    do {
        next = node->next;
        if (node->data != QOSA_NULL)
        {
            qurl_free(node->data);
        }
        qurl_free(node);
        node = next;
    } while (next);
}
