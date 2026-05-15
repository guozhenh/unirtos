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

#ifndef QURL_SLIST_H
#define QURL_SLIST_H

#include "qurl_def.h"

/**
 * @struct  qurl_slist_ctrl_t
 * @brief Non-universal unidirectional linked list data structure of qurl
 */
struct qurl_slist_ctrl_s
{
    void                     *data;
    struct qurl_slist_ctrl_s *next;
};
typedef struct qurl_slist_ctrl_s qurl_slist_ctrl_t;

qurl_slist_t qurl_slist_add(qurl_slist_t slist, void *data);

qurl_slist_t qurl_slist_add_strdup(qurl_slist_t slist, char *str);

void qurl_slist_del_all(qurl_slist_t list);

#endif /* Head define end*/
