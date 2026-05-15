/*****************************************************************/ /**
* @file qcm_utils_xmodem.c
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

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qcm_utils_xmodem.h"

#define QCM_XMODEM_SOH 0x01 /* Start of Header (128 bytes) */
#define QCM_XMODEM_STX 0x02 /* Start of Text (1024 bytes) */

qosa_uint16_t qcm_xmodem_crc16(const qosa_uint8_t *ptr, qosa_uint16_t count)
{
    qosa_uint16_t crc = 0;
    qosa_uint16_t i;

    while (count--)
    {
        crc = crc ^ ((qosa_uint16_t)*ptr++ << 8);

        for (i = 0; i < 8; i++)
        {
            if (crc & 0x8000)
            {
                crc = (crc << 1) ^ 0x1021;
            }
            else
            {
                crc = crc << 1;
            }
        }
    }

    return crc;
}

void qcm_xmodem_init(qcm_xmodem_ctx_t *ctx, qcm_xmodem_protocol_e xmodem_type)
{
    if (ctx != QOSA_NULL)
    {
        ctx->packet_seq = 0; /* Will be incremented to 1 on first packet */
        ctx->type = xmodem_type;
    }
}

qcm_xmodem_err_e qcm_xmodem_encode(qcm_xmodem_ctx_t *ctx, const qosa_uint8_t *src_data, int src_len, qosa_uint8_t *dest_buf, int dest_size, int *out_len)
{
    qosa_uint8_t  header_byte = 0;
    int           payload_size = 0;
    int           frame_total_size = 0;
    qosa_uint16_t crc_value = 0;
    qosa_uint8_t *payload_ptr = QOSA_NULL;

    /* Parameter validation */
    if (ctx == QOSA_NULL || src_data == QOSA_NULL || dest_buf == QOSA_NULL || out_len == QOSA_NULL)
    {
        return QCM_XMODEM_ERR_INVAL_PARM;
    }

    /* Determine protocol parameters */
    if (ctx->type == QCM_XMODEM_128)
    {
        header_byte = QCM_XMODEM_SOH;
        payload_size = QCM_XMODEM_DATA_SIZE_SOH;
    }
    else
    {
        header_byte = QCM_XMODEM_STX;
        payload_size = QCM_XMODEM_DATA_SIZE_STX;
    }

    /* Validate data length vs protocol limit */
    if (src_len > payload_size)
    {
        return QCM_XMODEM_ERR_INVAL_PARM;
    }

    /* Validate destination buffer size */
    frame_total_size = QCM_XMODEM_HEAD_SIZE + QCM_XMODEM_SEQ_SIZE + payload_size + QCM_XMODEM_CRC_SIZE;
    if (dest_size < frame_total_size)
    {
        return QCM_XMODEM_ERR_NOMEM; /* Buffer too small */
    }

    /* Increment sequence number (wraps automatically at uint8 overflow) */
    ctx->packet_seq++;

    /* --- Construct Frame Directly in Destination Buffer --- */

    /* 1. Header Byte */
    dest_buf[0] = header_byte;

    /* 2. Sequence Number */
    dest_buf[1] = ctx->packet_seq;

    /* 3. Sequence Number Complement (1's complement) */
    dest_buf[2] = (qosa_uint8_t)(~ctx->packet_seq);

    /* 4. Payload Data */
    payload_ptr = &dest_buf[QCM_XMODEM_HEAD_SIZE + QCM_XMODEM_SEQ_SIZE];

    /* Copy actual data */
    if (src_len > 0)
    {
        qosa_memcpy(payload_ptr, src_data, src_len);
    }

    /* Pad remaining bytes with CTRLZ (0x1A) if data is shorter than packet size */
    if (src_len < payload_size)
    {
        qosa_memset(payload_ptr + src_len, QCM_XMODEM_CTRLZ, payload_size - src_len);
    }

    /* 5. CRC16 */
    /* Calculate CRC on the payload area (data + padding) */
    crc_value = qcm_xmodem_crc16(payload_ptr, payload_size);

    dest_buf[frame_total_size - 2] = (qosa_uint8_t)(crc_value >> 8);   /* High byte */
    dest_buf[frame_total_size - 1] = (qosa_uint8_t)(crc_value & 0xFF); /* Low byte */

    /* Set output length */
    *out_len = frame_total_size;

    return QCM_XMODEM_ERR_OK;
}
