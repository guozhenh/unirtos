/*****************************************************************/ /**
* @file qosa_buffer_block.h
* @brief Buffer block management interface for UniRTOS
* @date 2023-04-20
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2023-04-20 <td>1.0 <td>Larson.Li <td> Init
* <tr><td>2023-04-25 <td>1.1 <td>Larson.Li <td> Add English Annotation
* </table>
**********************************************************************/
#ifndef __QOSA_BUFFER_BLOCK_H__
#define __QOSA_BUFFER_BLOCK_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_queue_list.h"

// Memory pool count low water mark
#define QOSA_MEMORY_LOW_ITEM_LIMIT  (CONFIG_QOSA_MEMORY_BUF_ITEM_CNT / 5)
// Memory pool count high water mark
#define QOSA_MEMORY_HIGH_ITEM_LIMIT (CONFIG_QOSA_MEMORY_BUF_ITEM_CNT / 2)

struct buffer_block_info_s;
typedef struct buffer_block_info_s qosa_buffer_block_t;
typedef qosa_ptr                   qosa_buffer_pool_id_t;

/**
 * @struct buffer_block_info_s
 * @brief Used to manage block data information structure
 */
struct buffer_block_info_s
{
    qosa_q_link_type_t          link;     /*!< Doubly linked list node, mainly used in watermark */
    struct buffer_block_info_s* next_ptr; /*!< Pointer to next bufferBLOCK node */
    qosa_uint32_t               size;     /*!< Maximum space size of current block in current node, corresponding to maximum size of single pool */
    qosa_uint32_t               used;     /*!< Data length in current BLOCK node */
    qosa_uint8_t*               data_ptr; /*!< Pointer to the actual saved data */

    qosa_buffer_pool_id_t pool_id;        /*!< pool corresponding to block application space */
};

/**
 * @brief Allocate a new buffer block from memory pool
 *
 * @return qosa_buffer_block_t*
 *          - Success: Returns pointer to allocated buffer block
 *          - Failure: Returns NULL
 */
qosa_buffer_block_t* qosa_buffer_new_block(void);

/**
 * @brief Free a single buffer block
 *
 * @param[in] block_ptr
 *          - Pointer to buffer block to be freed
 * @return qosa_buffer_block_t*
 *          - Returns pointer to next buffer block
 */
qosa_buffer_block_t* qosa_buffer_free_block(qosa_buffer_block_t* block_ptr);

/**
 * @brief Free all buffer blocks in the entire packet chain
 *
 * @param[in] pkt_head_ptr
 *          - Pointer to pointer to packet chain head
 */
void qosa_buffer_free_packet(qosa_buffer_block_t** pkt_head_ptr);

/**
 * @brief Calculate total data length of all buffer blocks in packet chain
 *
 * @param[in] block_ptr
 *          - Starting block pointer of packet chain
 * @return qosa_uint32_t
 *          - Total data length of packet chain
 */
qosa_uint32_t qosa_buffer_length_packet(qosa_buffer_block_t* block_ptr);

/**
 * @brief Get remaining space size at tail of buffer block
 *
 * @param[in] buf_ptr
 *          - Buffer block pointer
 * @return qosa_uint32_t
 *          - Remaining space size at tail of buffer block
 */
qosa_uint32_t qosa_buffer_tail_size(qosa_buffer_block_t* buf_ptr);

/**
 * @brief Insert data at the head of packet chain
 *
 * @param[in] pkt_head_ptr
 *          - Pointer to pointer to packet chain head
 * @param[in] buf
 *          - Data buffer to insert
 * @param[in] size
 *          - Size of data to insert
 * @return qosa_uint32_t
 *          - Actual size of inserted data
 */
qosa_uint32_t qosa_buffer_pushdown(qosa_buffer_block_t** pkt_head_ptr, void* buf, qosa_uint32_t size);

/**
 * @brief Append data at the tail of packet chain
 *
 * @param[in] pkt_head_ptr
 *          - Pointer to pointer to packet chain head
 * @param[in] buf
 *          - Data buffer to append
 * @param[in] size
 *          - Size of data to append
 * @return qosa_uint32_t
 *          - Actual size of appended data
 */
qosa_uint32_t qosa_buffer_pushdown_tail(qosa_buffer_block_t** pkt_head_ptr, const void* buf, qosa_uint32_t size);

/**
 * @brief Append one packet chain to the tail of another packet chain
 *
 * @param[in] pkt_head_ptr
 *          - Pointer to pointer to target packet chain head
 * @param[in] data_block_ptr
 *          - Pointer to pointer to packet chain head to append
 */
void qosa_buffer_append(qosa_buffer_block_t** pkt_head_ptr, qosa_buffer_block_t** data_block_ptr);

/**
 * @brief Extract data from the head of packet chain
 *
 * @param[in] pkt_head_ptr
 *          - Pointer to pointer to packet chain head
 * @param[out] buf
 *          - Buffer to store extracted data
 * @param[in] cnt
 *          - Size of data to extract
 * @return qosa_uint32_t
 *          - Actual size of extracted data
 */
qosa_uint32_t qosa_buffer_pullup(qosa_buffer_block_t** pkt_head_ptr, void* buf, qosa_uint32_t cnt);

/**
 * @brief Extract data from the tail of packet chain
 *
 * @param[in] pkt_head_ptr
 *          - Pointer to pointer to packet chain head
 * @param[out] buf
 *          - Buffer to store extracted data
 * @param[in] cnt
 *          - Size of data to extract
 * @return qosa_uint32_t
 *          - Actual size of extracted data
 */
qosa_uint32_t qosa_buffer_pullup_tail(qosa_buffer_block_t** pkt_head_ptr, void* buf, qosa_uint32_t cnt);

/**
 * @brief Extract data from specified offset position in packet chain
 *
 * @param[in] packet_ptr
 *          - Starting block pointer of packet chain
 * @param[in] offset
 *          - Offset position to extract data
 * @param[out] buf
 *          - Buffer to store extracted data
 * @param[in] len
 *          - Length of data to extract
 * @return qosa_uint32_t
 *          - Actual length of extracted data
 */
qosa_uint32_t qosa_buffer_extract(qosa_buffer_block_t* packet_ptr, qosa_uint32_t offset, void* buf, qosa_uint32_t len);

/**
 * @brief Insert data at specified offset position in packet chain
 *
 * @param[in] pkt_head_ptr
 *          - Pointer to pointer to packet chain head
 * @param[in] offset
 *          - Offset position to insert data
 * @param[in] buf
 *          - Data buffer to insert
 * @param[in] len
 *          - Length of data to insert
 * @return qosa_uint32_t
 *          - Actual length of inserted data
 */
qosa_uint32_t qosa_buffer_insert(qosa_buffer_block_t** pkt_head_ptr, qosa_uint32_t offset, void* buf, qosa_uint32_t len);

/**
 * @brief Copy part of packet chain data
 *
 * @param[out] dup_ptr
 *          - Pointer to pointer to copied packet chain head
 * @param[in] src_ptr
 *          - Starting block pointer of source packet chain
 * @param[in] offset
 *          - Offset position of data to copy in source packet chain
 * @param[in] cnt
 *          - Length of data to copy
 * @return qosa_uint32_t
 *          - Actual length of copied data
 */
qosa_uint32_t qosa_buffer_dup_packet(qosa_buffer_block_t** dup_ptr, qosa_buffer_block_t* src_ptr, qosa_uint32_t offset, qosa_uint32_t cnt);

/**
 * @brief Get number of available buffer blocks remaining in memory pool
 *
 * @return qosa_uint32_t
 *          - Number of available buffer blocks remaining
 */
qosa_uint32_t qosa_buffer_get_block_free_cnt(void);

/**
 * @brief Initialize buffer block management system
 */
void qosa_buffer_init(void);

#endif /* __QOSA_BUFFER_BLOCK_H__ */
