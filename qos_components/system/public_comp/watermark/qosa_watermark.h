/*****************************************************************/ /**
 * @file qosa_watermark.h
 * @brief Buffer watermark management system header file
 * @details
 * This module provides buffer watermark management functionality for
 * monitoring and managing data buffers in queues.
 * Supports high watermark, low watermark, empty queue, non-empty queue
 * and other event callbacks, as well as queue capacity limits.
 *
 * @date 2023-04-20
 *
 * @par EDIT HISTORY FOR MODULE
 * <table>
 * <tr><th>Date <th>Version <th>Author <th>Description"
 * <tr><td>2023-04-20 <td>1.0 <td>Larson.Li <td> Initial version
 * </table>
 **********************************************************************/
#ifndef __QOSA_WATERMARK_H__
#define __QOSA_WATERMARK_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_queue_list.h"
#include "qosa_buffer_block.h"

struct buffer_watermark_type_s;
typedef void (*qosa_wm_cb_ptr)(struct buffer_watermark_type_s*, void*);

/**
 * @enum qosa_buffer_wm_cb_type_e
 * @brief Watermark event type enumeration
 */
typedef enum
{
    QOSA_BUFFER_WM_LOW = 0,   /*!< Low watermark event: Triggered when queue data volume drops from above low watermark to below low watermark */
    QOSA_BUFFER_WM_HIGH,      /*!< High watermark event: Triggered when queue data volume rises from below high watermark to above high watermark */
    QOSA_BUFFER_WM_EMPTY,     /*!< Queue empty event: Triggered when queue changes from non-empty state to empty state */
    QOSA_BUFFER_WM_NON_EMPTY, /*!< Queue non-empty event: Triggered when queue changes from empty state to non-empty state */
    QOSA_BUFFER_WM_DNE_BYTES  /*!< Capacity limit event: Used to set queue capacity limit, does not trigger callback */
} qosa_buffer_wm_cb_type_e;

/**
 * @struct buffer_watermark_type_s
 * @brief Buffer watermark management structure
 * @details
 * This structure is used to manage buffer queue watermarks, supporting multiple event callback mechanisms,
 * including enqueue, dequeue, high/low watermark, empty/non-empty state change events notification.
 */
typedef struct buffer_watermark_type_s
{
    qosa_q_type_t* q_ptr;         /*!< Associated queue pointer: Points to the queue structure associated with this watermark */
    qosa_mutex_t   lock;          /*!< Queue lock: Mutex lock used to protect queue operations */
    qosa_uint32_t  sentinel;      /*!< Initialization verification sentinel: Used to verify if the watermark structure has been properly initialized */
    qosa_uint32_t  low_watermark; /*!< Low watermark: Low watermark marker, low watermark callback is triggered when queue data volume is below this value */
    qosa_uint32_t high_watermark; /*!< High watermark: High watermark marker, high watermark callback is triggered when queue data volume is above this value */
    qosa_uint32_t capacity_limit; /*!< Queue capacity limit: Maximum data volume allowed in the queue, data exceeding this value will be discarded */
    qosa_uint32_t current_bytes;  /*!< Current bytes in queue: Real-time reflection of total data in the queue */
    qosa_wm_cb_ptr wm_in_queue_cb;          /*!< Each enqueue callback function pointer: This callback is called whenever any data block is enqueued */
    void*          wm_in_queue_argv;        /*!< Enqueue callback user data: User-defined data passed to the enqueue callback function */
    qosa_wm_cb_ptr wm_out_queue_cb;         /*!< Each dequeue callback function pointer: This callback is called whenever any data block is dequeued */
    void*          wm_out_queue_argv;       /*!< Dequeue callback user data: User-defined data passed to the dequeue callback function */
    qosa_wm_cb_ptr wm_low_water_cb;         /*!< Low watermark callback function pointer: Triggered when queue data volume drops below low watermark */
    void*          wm_low_water_argv;       /*!< Low watermark callback user data: User-defined data passed to the low watermark callback function */
    qosa_wm_cb_ptr wm_high_water_cb;        /*!< High watermark callback function pointer: Triggered when queue data volume rises above high watermark */
    void*          wm_high_water_argv;      /*!< High watermark callback user data: User-defined data passed to the high watermark callback function */
    qosa_wm_cb_ptr wm_low_water_hal_cb;     /*!< Driver layer low watermark callback function pointer: Users do not need to care */
    void*          wm_low_water_hal_argv;   /*!< Driver layer low watermark callback user data: Users do not need to care */
    qosa_wm_cb_ptr wm_high_water_hal_cb;    /*!< Driver layer high watermark callback function pointer: Users do not need to care */
    void*          wm_high_water_hal_argv;  /*!< Driver layer high watermark callback user data: Users do not need to care */
    qosa_wm_cb_ptr wm_become_empty_cb;      /*!< Queue empty callback function pointer: Triggered when queue changes from non-empty state to empty state */
    void*          wm_become_empty_argv;    /*!< Queue empty callback user data: User-defined data passed to the queue empty callback function */
    qosa_wm_cb_ptr wm_become_nonempty_cb;   /*!< Queue non-empty callback function pointer: Triggered when queue changes from empty state to non-empty state */
    void*          wm_become_nonempty_argv; /*!< Queue non-empty callback user data: User-defined data passed to the queue non-empty callback function */
} qosa_buffer_watermark_t;

/**
 * @brief Initialize buffer watermark queue
 *
 * @param[in] wm_ptr Watermark structure pointer
 * @param[in] dne Queue capacity limit value (Do Not Exceed)
 * @param[in] queue Associated queue pointer
 *
 * @note This function will initialize the mutex, must call qosa_buffer_queue_destroy for cleanup after use
 */
extern void qosa_buffer_queue_init(qosa_buffer_watermark_t* wm_ptr, qosa_uint32_t dne, qosa_q_type_t* queue);

/**
 * @brief Enqueue data block to watermark queue
 *
 * @param[in] wm_ptr Watermark structure pointer
 * @param[in,out] pkt_head_ptr Pointer to pointer of data block, will be set to NULL after successful enqueue
 *
 * @note If queue is full (exceeds capacity limit), data block will be discarded
 */
extern void qosa_buffer_enqueue(qosa_buffer_watermark_t* wm_ptr, qosa_buffer_block_t** pkt_head_ptr);

/**
 * @brief Dequeue a data block from watermark queue
 *
 * @param[in] wm_ptr Watermark structure pointer
 * @return qosa_buffer_block_t* Dequeued data block pointer, returns NULL if queue is empty
 */
extern qosa_buffer_block_t* qosa_buffer_dequeue(qosa_buffer_watermark_t* wm_ptr);

/**
 * @brief Clear all data blocks in watermark queue
 *
 * @param[in] wm_ptr Watermark structure pointer
 *
 * @note This function will release all data blocks in the queue
 */
extern void qosa_buffer_empty_queue(qosa_buffer_watermark_t* wm_ptr);

/**
 * @brief Check if watermark queue is empty
 *
 * @param[in] wm_ptr Watermark structure pointer
 * @return qosa_bool_t Returns QOSA_TRUE if queue is empty, otherwise returns QOSA_FALSE
 */
qosa_bool_t qosa_buffer_is_wm_empty(qosa_buffer_watermark_t* wm_ptr);

/**
 * @brief Get current byte count in watermark queue
 *
 * @param[in] wm_ptr Watermark structure pointer
 * @return qosa_uint32_t Current byte count in queue
 */
qosa_uint32_t qosa_buffer_queue_cnt(qosa_buffer_watermark_t* wm_ptr);

/**
 * @brief Destroy watermark queue and release related resources
 *
 * @param[in] wm_ptr Watermark structure pointer
 *
 * @note This function will destroy the mutex and clear the queue, must be called when queue is no longer in use
 */
void qosa_buffer_queue_destroy(qosa_buffer_watermark_t* wm_ptr);

#endif /* __QOSA_WATERMARK_H__ */
