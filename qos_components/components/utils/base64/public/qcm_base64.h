/*****************************************************************/ /**
* @file qcm_base64.h
* @brief
* @author larson.li@quectel.com
* @date 2024-03-07
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-03-07 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_BASE64_H__
#define __QOSA_BASE64_H__

#include "qosa_def.h"
#include "qosa_sys.h"

/**
 * @brief base64 decode
 *
 * @param[in] const char * src_data
 *          - Input base64 string to be decoded
 *
 * @param[in] int src_len
 *          - Length of input base64 string to be decoded
 *
 * @param[out] char * dst_buf
 *           - Output decoded string buffer
 *
 * @param[in/out] int * dst_len
 *              - Output decoded string length, when passed to function it's the length to be decoded, after function execution it's the actual decoded length
 *
 * @return int
 *       - Returns actual decoded length greater than 0 on success
 *       - Returns -2 on parameter error
 *       - Returns -1 if dst_buf space is insufficient
 */
int qcm_base64_decode(const char *src_data, int src_len, char *dst_buf, int *dst_len);

/**
 * @brief Encode given binary data using Base64 and store the result in target buffer.
 *
 * @param[in] const char * src_data
 *          - Pointer to binary data to be encoded.
 *
 * @param[in] int src_len
 *          - Length of binary data to be encoded (in bytes).
 *
 * @param[in] char * dst_buf
 *          - Pointer to target buffer for storing encoded data.
 *
 * @param[in] int * dst_len
 *          - Pointer to variable specifying target buffer size (in bytes).
 *
 * @param[in] qosa_bool_t line_wrap
 *          - Boolean value indicating whether to add line breaks every 76 bytes. For objective reality, OSA_FALSE means disabled
 *
 * @return int
 *       - Returns actual encoded length greater than 0 on success
 *       - Returns -2 on parameter error
 *       - Returns -1 if dst_buf space is insufficient
 */
int qcm_base64_encode(const char *src_data, int src_len, char *dst_buf, int *dst_len, qosa_bool_t line_wrap);

#endif /* __QOSA_BASE64_H__ */
