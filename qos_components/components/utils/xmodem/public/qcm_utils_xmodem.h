/*****************************************************************//**
* @file qcm_utils_xmodem.h
* @brief 
* @author larson.li@quectel.com
* @date 2025-12-18
* 
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd. 
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
* 
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2025-12-18 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __QCM_UTILS_XMODEM_H__
#define __QCM_UTILS_XMODEM_H__

#include "qosa_def.h"
#include "qosa_sys.h"

/* Protocol Payload Sizes */
#define QCM_XMODEM_DATA_SIZE_SOH  128  /* 128 bytes data */
#define QCM_XMODEM_DATA_SIZE_STX  1024 /* 1024 bytes data */

/* Protocol Overhead Sizes */
#define QCM_XMODEM_HEAD_SIZE      1 /* Header byte */
#define QCM_XMODEM_SEQ_SIZE       2 /* Seq + ~Seq */
#define QCM_XMODEM_CRC_SIZE       2 /* CRC16 */
#define QCM_XMODEM_OVERHEAD       (QCM_XMODEM_HEAD_SIZE + QCM_XMODEM_SEQ_SIZE + QCM_XMODEM_CRC_SIZE)

/* Helper macro for max buffer size required */
#define QCM_XMODEM_MAX_FRAME_SIZE (QCM_XMODEM_DATA_SIZE_STX + QCM_XMODEM_OVERHEAD)

    typedef enum
{
    QCM_XMODEM_ERR_OK = 0,         /* Operation successful */
    QCM_XMODEM_ERR_UNKNOW = 1,     /* Unknown error */
    QCM_XMODEM_ERR_INVAL_PARM = 2, /* Invalid parameter error */
    QCM_XMODEM_ERR_NOMEM = 3,      /* Insufficient memory / buffer too small */
} qcm_xmodem_err_e;

typedef enum
{
    QCM_XMODEM_128 = 0x01,
    QCM_XMODEM_1024 = 0x02,
} qcm_xmodem_protocol_e;

typedef enum
{
    QCM_XMODEM_EOT = 0x04,   /* End of Transmission */
    QCM_XMODEM_ACK = 0x06,   /* Acknowledge */
    QCM_XMODEM_NAK = 0x15,   /* Negative Acknowledge */
    QCM_XMODEM_CAN = 0x18,   /* Cancel */
    QCM_XMODEM_CTRLZ = 0x1A, /* Padding Character */
} qcm_xmodem_control_byte_e;

/**
 * @brief Xmodem context structure to handle state.
 * This allows multiple xmodem sessions and avoids global variables.
 */
typedef struct
{
    qcm_xmodem_protocol_e type;       /* Protocol type (128 or 1K) */
    qosa_uint8_t          packet_seq; /* Current packet sequence number */
} qcm_xmodem_ctx_t;

/**
 * @brief Calculate CRC16 (CCITT)
 */
qosa_uint16_t qcm_xmodem_crc16(const qosa_uint8_t *ptr, qosa_uint16_t count);

/**
 * @brief Initialize the Xmodem context
 * @param ctx Pointer to context structure
 * @param xmodem_type Protocol type
 */
void qcm_xmodem_init(qcm_xmodem_ctx_t *ctx, qcm_xmodem_protocol_e xmodem_type);

/**
 * @brief Encode a data block into an Xmodem frame.
 * 
 * @note This function DOES NOT allocate memory. It writes to dest_buf.
 * 
 * @param ctx           [IN/OUT] Context pointer
 * @param src_data      [IN]     Raw data to send
 * @param src_len       [IN]     Length of raw data (must be <= protocol data size)
 * @param dest_buf      [OUT]    Buffer to store the encoded frame
 * @param dest_size     [IN]     Size of the destination buffer
 * @param out_len       [OUT]    Actual length of the encoded frame
 * 
 * @return qcm_xmodem_err_e 
 */
qcm_xmodem_err_e qcm_xmodem_encode(qcm_xmodem_ctx_t *ctx, const qosa_uint8_t *src_data, int src_len, qosa_uint8_t *dest_buf, int dest_size, int *out_len);


#endif /* __QCM_UTILS_XMODEM_H__ */
