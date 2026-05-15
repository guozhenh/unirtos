/*****************************************************************/ /**
* @file qosa_queue_list.h
* @brief
* @author larson.li@quectel.com
* @date 2023-04-20
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2023-04-20 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_QUEUE_LIST_H__
#define __QOSA_QUEUE_LIST_H__

#include "qosa_def.h"
#include "qosa_sys.h"

/** Queue magic number, used to verify if the queue has been initialized */
#define QOSA_QUEUE_MAGIC (0xDEADBEE2)

/*
 * ==========================================================================
 *   BEGIN: Linux Kernel list.h Core Implementation
 *   In order to make the project self-contained, the core part of Linux kernel's list.h
 *   is directly embedded here.
 *   The code is copyrighted by the Linux kernel developers.
 *   UniRTOS modify add qosa_ prefix to avoid conflict with other code.
 * ==========================================================================
 */

#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE, MEMBER) __compiler_offsetof(TYPE, MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((qosa_size_t) & ((TYPE*)0)->MEMBER)
#endif

#define qosa_container_of(ptr, type, member)                                                                                                                   \
    ({                                                                                                                                                         \
        const typeof(((type*)0)->member)* __mptr = (ptr);                                                                                                      \
        (type*)((char*)__mptr - offsetof(type, member));                                                                                                       \
    })

/**
 * @struct qosa_list_head
 * @brief Doubly linked list head structure, based on Linux kernel list implementation
 */
struct qosa_list_head
{
    struct qosa_list_head* next; /*!< Pointer to next node */
    struct qosa_list_head* prev; /*!< Pointer to previous node */
};

/** Initialize list head macro definition */
#define QOSA_LIST_HEAD_INIT(name)                                                                                                                              \
    {                                                                                                                                                          \
        &(name), &(name)                                                                                                                                       \
    }

/** Declare and initialize list head macro definition */
#define QOSA_LIST_HEAD(name) struct qosa_list_head name = QOSA_LIST_HEAD_INIT(name)

/**
 * @brief Initialize list head
 *
 * @param[in] list
 *          - List head pointer
 */
static inline void QOSA_INIT_LIST_HEAD(struct qosa_list_head* list)
{
    list->next = list;
    list->prev = list;
}

/**
 * @brief Internal list add function
 *
 * @param[in] new_node
 *          - New node pointer
 * @param[in] prev
 *          - Previous node pointer
 * @param[in] next
 *          - Next node pointer
 */
static inline void __qosa_list_add(struct qosa_list_head* new_node, struct qosa_list_head* prev, struct qosa_list_head* next)
{
    next->prev = new_node;
    new_node->next = next;
    new_node->prev = prev;
    prev->next = new_node;
}

/**
 * @brief Add node at the head of the list
 *
 * @param[in] new_node
 *          - New node pointer
 * @param[in] head
 *          - List head pointer
 */
static inline void qosa_list_add(struct qosa_list_head* new_node, struct qosa_list_head* head)
{
    __qosa_list_add(new_node, head, head->next);
}

/**
 * @brief Add node at the tail of the list
 *
 * @param[in] new_node
 *          - New node pointer
 * @param[in] head
 *          - List head pointer
 */
static inline void qosa_list_add_tail(struct qosa_list_head* new_node, struct qosa_list_head* head)
{
    __qosa_list_add(new_node, head->prev, head);
}

/**
 * @brief Internal list delete function
 *
 * @param[in] prev
 *          - Previous node pointer
 * @param[in] next
 *          - Next node pointer
 */
static inline void __qosa_list_del(struct qosa_list_head* prev, struct qosa_list_head* next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 * @brief Delete node from list
 *
 * @param[in] entry
 *          - Node pointer to delete
 */
static inline void qosa_list_del(struct qosa_list_head* entry)
{
    __qosa_list_del(entry->prev, entry->next);
    entry->next = QOSA_NULL; /* Or some poison value */
    entry->prev = QOSA_NULL; /* Or some poison value */
}

/**
 * @brief Check if list is empty
 *
 * @param[in] head
 *          - List head pointer
 * @return int
 *          - 1: List is empty
 *          - 0: List is not empty
 */
static inline int qosa_list_empty(const struct qosa_list_head* head)
{
    return head->next == head;
}

/** Get structure pointer containing the member through member pointer */
#define qosa_list_entry(ptr, type, member)       qosa_container_of(ptr, type, member)

/** Get the first entry in the list */
#define qosa_list_first_entry(ptr, type, member) qosa_list_entry((ptr)->next, type, member)

/** Traverse all entries in the list */
#define qosa_list_for_each_entry(pos, head, member)                                                                                                            \
    for (pos = qosa_list_entry((head)->next, typeof(*pos), member); &pos->member != (head); pos = qosa_list_entry(pos->member.next, typeof(*pos), member))

/*
 * ==========================================================================
 *   END: Linux Kernel list.h Core Implementation
 * ==========================================================================
 */

/**
 * @struct qosa_q_link_type_t
 * @brief Doubly linked list element node. Now an alias for Linux's list_head.
 */
typedef struct qosa_list_head qosa_q_link_type_t;

/**
 * @struct qosa_q_type_t
 * @brief Represents a queue. Uses Linux's list_head as the list anchor.
 */
typedef struct list_queue_s
{
    qosa_uint32_t         magic; /*!< Magic number to check for initialization. */
    struct qosa_list_head head;  /*!< List anchor for the queue items. */
    qosa_int32_t          cnt;   /*!< Keeps track of the number of items enqueued. */
    qosa_mutex_t          mutex; /*!< Mutex, to provide interface security. */
} qosa_q_type_t;

/**
 * @struct qosa_q_generic_item_type_t
 * @brief The first element MUST be qosa_q_link_type_t, now aliased to list_head.
 */
typedef struct
{
    qosa_q_link_type_t link;
} qosa_q_generic_item_type_t;

/** Get structure pointer containing the queue node through node pointer */
#define QOSA_Q_ENTRY(ptr, type, member) qosa_container_of(ptr, type, member)

/**
 * @brief Queue comparison function type definition
 *
 * @param[in] note_ptr
 *          - Queue node pointer
 * @param[in] value
 *          - Comparison value pointer
 * @return qosa_bool_t
 *          - QOSA_TRUE: Match
 *          - QOSA_FALSE: No match
 */
typedef qosa_bool_t (*qosa_q_compare_func)(void* note_ptr, void* value);

/**
 * @brief Initialize queue
 *
 * @param[in] q_ptr
 *          - Queue structure pointer
 * @return qosa_q_type_t*
 *          - Success: Return queue pointer
 *          - Failure: Return QOSA_NULL
 */
qosa_q_type_t* qosa_q_init(qosa_q_type_t* q_ptr);

/**
 * @brief Initialize queue node link
 *
 * @param[in] item_ptr
 *          - Queue item pointer
 * @param[in] link_ptr
 *          - Link pointer
 * @return qosa_q_link_type_t*
 *          - Success: Return link pointer
 *          - Failure: Return QOSA_NULL
 */
qosa_q_link_type_t* qosa_q_link(void* item_ptr, qosa_q_link_type_t* link_ptr);

/**
 * @brief Put node at the tail of the queue
 *
 * @param[in] q_ptr
 *          - Queue structure pointer
 * @param[in] link_ptr
 *          - Node link pointer to put
 */
void qosa_q_put(qosa_q_type_t* q_ptr, qosa_q_link_type_t* link_ptr);

/**
 * @brief Get node from the head of the queue
 *
 * @param[in] q_ptr
 *          - Queue structure pointer
 * @return void*
 *          - Success: Return node pointer
 *          - Failure: Return QOSA_NULL
 */
void* qosa_q_get(qosa_q_type_t* q_ptr);

/**
 * @brief Get node from the tail of the queue
 *
 * @param[in] q_ptr
 *          - Queue structure pointer
 * @return void*
 *          - Success: Return node pointer
 *          - Failure: Return QOSA_NULL
 */
void* qosa_q_last_get(qosa_q_type_t* q_ptr);

/**
 * @brief Get the count of nodes in the queue
 *
 * @param[in] q_ptr
 *          - Queue structure pointer
 * @return qosa_int32_t
 *          - Number of nodes in the queue
 */
qosa_int32_t qosa_q_cnt(qosa_q_type_t* q_ptr);

/**
 * @brief Check the head node of the queue (without deletion)
 *
 * @param[in] q_ptr
 *          - Queue structure pointer
 * @return void*
 *          - Success: Return node pointer
 *          - Failure: Return QOSA_NULL
 */
void* qosa_q_check(qosa_q_type_t* q_ptr);

/**
 * @brief Check the tail node of the queue (without deletion)
 *
 * @param[in] q_ptr
 *          - Queue structure pointer
 * @return void*
 *          - Success: Return node pointer
 *          - Failure: Return QOSA_NULL
 */
void* qosa_q_last_check(qosa_q_type_t* q_ptr);

/**
 * @brief Get the next node of the specified node
 *
 * @param[in] q_ptr
 *          - Queue structure pointer
 * @param[in] link_ptr
 *          - Current node link pointer
 * @return void*
 *          - Success: Return next node pointer
 *          - Failure: Return QOSA_NULL
 */
void* qosa_q_next(qosa_q_type_t* q_ptr, qosa_q_link_type_t* link_ptr);

/**
 * @brief Get the previous node of the specified node
 *
 * @param[in] q_ptr
 *          - Queue structure pointer
 * @param[in] link_ptr
 *          - Current node link pointer
 * @return void*
 *          - Success: Return previous node pointer
 *          - Failure: Return QOSA_NULL
 */
void* qosa_q_prev(qosa_q_type_t* q_ptr, qosa_q_link_type_t* link_ptr);

/**
 * @brief Insert new node before the specified node
 *
 * @param[in] q_ptr
 *          - Queue structure pointer
 * @param[in] q_insert_ptr
 *          - Node pointer to insert
 * @param[in] q_item_ptr
 *          - Specified node pointer
 */
void qosa_q_insert(qosa_q_type_t* q_ptr, qosa_q_link_type_t* q_insert_ptr, qosa_q_link_type_t* q_item_ptr);

/**
 * @brief Linear search for node in the queue
 *
 * @param[in] q_ptr
 *          - Queue structure pointer
 * @param[in] compare_func
 *          - Comparison function pointer
 * @param[in] compare_val
 *          - Comparison value pointer
 * @return void*
 *          - Success: Return matching node pointer
 *          - Failure: Return QOSA_NULL
 */
void* qosa_q_linear_search(qosa_q_type_t* q_ptr, qosa_q_compare_func compare_func, void* compare_val);

/**
 * @brief Delete specified node from the queue
 *
 * @param[in] q_ptr
 *          - Queue structure pointer
 * @param[in] q_delete_ptr
 *          - Node pointer to delete
 */
void qosa_q_delete(qosa_q_type_t* q_ptr, qosa_q_link_type_t* q_delete_ptr);

/**
 * @brief Destroy queue
 *
 * @param[in] q_ptr
 *          - Queue structure pointer
 */
void qosa_q_destroy(qosa_q_type_t* q_ptr);

/**
 * @brief Put node at the head of the queue
 *
 * @param[in] q_ptr
 *          - Queue structure pointer
 * @param[in] link_ptr
 *          - Node link pointer to put
 */
void qosa_q_put_head(qosa_q_type_t* q_ptr, qosa_q_link_type_t* link_ptr);

/**
 * @brief Check if queue is initialized
 *
 * @param[in] q_ptr
 *          - Queue structure pointer
 * @return qosa_bool_t
 *          - QOSA_TRUE: Initialized
 *          - QOSA_FALSE: Not initialized
 */
qosa_bool_t qosa_q_is_initialized(qosa_q_type_t* q_ptr);

#endif /* __QOSA_QUEUE_LIST_H__ */
