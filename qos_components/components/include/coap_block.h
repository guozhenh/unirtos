/*****************************************************************/ /**
* @file coap_block.h
* @brief
* @author larson.li@quectel.com
* @date 2024-09-12
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-09-12 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/

#ifndef _COAP_BLOCK_H_
#define _COAP_BLOCK_H_

#include "qosa_def.h"
#include "qosa_sys.h"

/*
 * LWM2M block data
 *
 * Temporary data needed to handle block1 request and block2 responses.
 */
typedef enum
{
    BLOCK_1, /*!< block1 block type, used for block transfer of request data */
    BLOCK_2, /*!< block2 block type, used for block transfer of response data */
} block_type_t;

/** @brief Block data identifier union, uses different identification methods based on block type */
typedef union _block_data_identifier_
{
    char        *uri; /*!< Resource URI string, used for BLOCK_1 type */
    qosa_int32_t mid; /*!< Message ID, used for BLOCK_2 type, represents last requested mid or expected block mid */
} block_data_identifier_t;

typedef struct _coap_block_data_ coap_block_data_t;

/**
 * @brief Linked list structure for recording block data
 */
/** @brief Linked list structure for recording block data */
struct _coap_block_data_
{
    struct _coap_block_data_ *next;            /*!< Next block node */
    block_type_t              blockType;       /*!< Block type */
    block_data_identifier_t   identifier;      /*!< Block data identifier */
    qosa_uint8_t             *blockBuffer;     /*!< Data buffer pointer */
    qosa_size_t               blockBufferSize; /*!< Buffer size */
    qosa_uint32_t             blockNum;        /*!< Last received message block number */
    qosa_uint16_t             mid;             /*!< Last received message mid */
};

// defined in block.c
/**
 * @brief Test if processing block1 block data is normal
 *
 * @param[in,out] coap_block_data_t *blockDataHead
 *          - Block data packet
 * @param[in] const char * uri
 *          - Target host URI
 * @param[in] qosa_uint16_t mid
 *          - Block mid
 * @param[in] const qosa_uint8_t * buffer
 *          - Block data buffer
 * @param[in] qosa_size_t length
 *          - Block data length
 * @param[in] qosa_uint16_t blockSize
 *          - Block size
 * @param[in] qosa_uint32_t blockNum,
 *          - Block number
 * @param[in] qosa_size_t length
 *          - This block data length
 * @param[in] qosa_bool_t blockMore
 *          - Whether there are more blocks, 1=yes, 0=no
 * @param[out] qosa_uint8_t ** outputBuffer,
 *          - Store reassembled complete data
 * @param[out] qosa_size_t * outputLength
 *          - Complete data length
 *
 * @return qosa_uint8_t
 *       - Returns COAP_NO_ERROR on success, indicating block data reception completed; COAP_231_CONTINUE, indicating more blocks need to be received
 *       - Returns error code on failure
 */
qosa_uint8_t qcm_coap_raw_block1_handler(
    coap_block_data_t **pBlockDataHead,
    const char         *uri,
    qosa_uint16_t       mid,
    qosa_uint8_t       *buffer,
    qosa_size_t         length,
    qosa_uint16_t       blockSize,
    qosa_uint32_t       blockNum,
    qosa_bool_t         blockMore,
    qosa_uint8_t      **outputBuffer,
    qosa_size_t        *outputLength
);

/**
 * @brief Process block1 block data
 *
 * @param[in,out] coap_block_data_t *blockDataHead
 *          - Block data packet
 * @param[in] const char * uri
 *          - Target host URI
 * @param[in] const qosa_uint8_t * buffer
 *          - Block data buffer
 * @param[in] qosa_size_t length
 *          - Block data length
 * @param[in] qosa_uint16_t blockSize
 *          - Block size
 * @param[in] qosa_uint32_t blockNum,
 *          - Block number
 * @param[in] qosa_size_t length
 *          - This block data length
 * @param[in] qosa_bool_t blockMore
 *          - Whether there are more blocks, 1=yes, 0=no
 * @param[out] qosa_uint8_t ** outputBuffer,
 *          - Store reassembled complete data
 * @param[out] qosa_size_t * outputLength
 *          - Complete data length
 *
 * @return qosa_uint8_t
 *       - Returns COAP_NO_ERROR on success, indicating block data reception completed; COAP_231_CONTINUE, indicating more blocks need to be received
 *       - Returns error code on failure
 */
qosa_uint8_t qcm_coap_block1_handler(
    coap_block_data_t **pBlockDataHead,
    const char         *uri,
    const qosa_uint8_t *buffer,
    qosa_size_t         length,
    qosa_uint16_t       blockSize,
    qosa_uint32_t       blockNum,
    qosa_bool_t         blockMore,
    qosa_uint8_t      **outputBuffer,
    qosa_size_t        *outputLength
);

/**
 * @brief Delete block data related to specified uri
 *
 * @param[in,out] coap_block_data_t *blockDataHead
 *          - Block data packet
 * @param[in] char *uri
 *          - Specified uri
 */
void qcm_block1_delete(coap_block_data_t **pBlockDataHead, char *uri);

/**
 * @brief Process block2 block data
 *
 * @param[in,out] coap_block_data_t *blockDataHead
 *          - Block data packet
 * @param[in] qosa_uint16_t mid
 *          - Block mid
 * @param[in] const qosa_uint8_t * buffer
 *          - Block data buffer
 * @param[in] qosa_size_t length
 *          - Block data length
 * @param[in] qosa_uint16_t blockSize
 *          - Block size
 * @param[in] qosa_uint32_t blockNum,
 *          - Block number
 * @param[in] qosa_size_t length
 *          - This block data length
 * @param[in] qosa_bool_t blockMore
 *          - Whether there are more blocks, 1=yes, 0=no
 * @param[out] qosa_uint8_t ** outputBuffer,
 *          - Store reassembled complete data
 * @param[out] qosa_size_t * outputLength
 *          - Complete data length
 *
 * @return qosa_uint8_t
 *       - Returns COAP_NO_ERROR on success, indicating block data reception completed; COAP_231_CONTINUE, indicating more blocks need to be received
 *       - Returns error code on failure
 */
qosa_uint8_t qcm_coap_block2_handler(
    coap_block_data_t **pBlockDataHead,
    qosa_uint16_t       mid,
    const qosa_uint8_t *buffer,
    qosa_size_t         length,
    qosa_uint16_t       blockSize,
    qosa_uint32_t       blockNum,
    qosa_bool_t         blockMore,
    qosa_uint8_t      **outputBuffer,
    qosa_size_t        *outputLength
);

/**
 * @brief Update expected mid for next block in block2 data block transfer
 *
 * @param[in,out] coap_block_data_t *blockDataHead
 *          - Block data packet
 * @param[in] qosa_uint16_t currentMid
 *          - Current block mid
 * @param[in] qosa_uint16_t expectedMid
 *          - Expected mid for next block
 */
void qcm_coap_block2_set_expected_mid(coap_block_data_t *blockDataHead, qosa_uint16_t currentMid, qosa_uint16_t expectedMid);

/**
 * @brief Free block resources
 *
 * @param[in] coap_block_data_t *blockData
 *          - Block data packet
 */
void qcm_free_block_data(coap_block_data_t *blockData);

/**
 * @brief Traverse block data linked list, delete block data with specified mid, and free resources
 *
 * @param[in,out] coap_block_data_t *blockDataHead
 *          - Block data packet
 * @param[in] qosa_uint16_t mid
 *          - Specified block mid
 */
void qcm_block2_delete(coap_block_data_t **pBlockDataHead, qosa_uint16_t mid);

#endif /* _COAP_BLOCK_H_ */
