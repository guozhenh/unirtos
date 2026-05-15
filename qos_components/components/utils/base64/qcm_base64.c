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
#include "qcm_base64.h"
#include "qosa_def.h"
#include "qosa_sys.h"

#define XX                     127
#define BASE64_LINE_WRAP_COUNT 76
#define OSA_PRV_B64_PADDING    '='

/*
 * Table for decoding base64
 */
static const char index_64[128] = {
    XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX,
    XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, 62, XX, XX, XX, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, XX, XX, XX, XX, XX, XX,
    XX, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, XX, XX, XX, XX, XX,
    XX, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, XX, XX, XX, XX, XX,
};
#define CHAR64(c) (((c) >= sizeof(index_64)) ? XX : (index_64[(c)]))
static const char basis_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static int qcm_base64_get_decoded_size(const char *dataP, int dataLen)
{
    int result;

    result = 3 * (dataLen / 4);
    switch (dataLen % 4)
    {
        case 0:
            if (result > 0)
            {
                /* Account for any padding */
                if (dataP[dataLen - 2] == OSA_PRV_B64_PADDING)
                    result -= 2;
                else if (dataP[dataLen - 1] == OSA_PRV_B64_PADDING)
                    result -= 1;
            }
            break;
        case 2:
            result += 1;
            break;
        case 3:
            result += 2;
            break;
        default:
            /* Should never happen */
            break;
    }

    return result;
}

/**
 * @brief base64 decoding
 *
 * @param[in] const char * src_data
 *          - The input base64 string to be decoded
 *
 * @param[in] int src_len
 *          - Length of the input base64 string to be decoded
 *
 * @param[out] char * dst_buf
 *           - The output buffer for the decoded string
 *
 * @param[in/out] int * dst_len
 *              - The length of the output decoded string; when passed into the function, it is the length of the string to be decoded; after the function execution, it becomes the actual length of the decoded string.
 *
 * @return int
 *       - Returns the actual decoded length greater than 0 on success
 *       - Returns -2 for parameter errors
 *       - Return -1 if the dst_buff space is insufficient
 */
int qcm_base64_decode(const char *src_data, int src_len, char *dst_buf, int *dst_len)
{
    const char *pSrc = src_data;
    char       *pDest = dst_buf;
    int         decodeSize = qcm_base64_get_decoded_size(src_data, src_len);

    if (src_data == QOSA_NULL || src_len <= 0 || dst_buf == QOSA_NULL || dst_len == QOSA_NULL)
    {
        return -2;
    }

    if (decodeSize > *dst_len)
    {
        return -1;
    }
    *dst_len = 0;

    for (;;)
    {
        unsigned char c1, c2, c3, c4;

        c1 = *pSrc++;

        if (c1 == '\0')
        {
            return pDest - dst_buf;
        }
        if (c1 == 0x0D)
        {  // newline?
            c1 = *pSrc++;
            if (c1 == 0x0A)
                c1 = *pSrc++;
        }

        if (CHAR64(c1) == XX)
        {
            return -1;
        }

        c2 = *pSrc++;

        if (CHAR64(c2) == XX)
        {
            return -1;
        }

        c3 = *pSrc++;

        if ((c3 != OSA_PRV_B64_PADDING) && (CHAR64(c3) == XX))
        {
            return -1;
        }

        c4 = *pSrc++;

        if ((c4 != OSA_PRV_B64_PADDING) && (CHAR64(c4) == XX))
        {
            return -1;
        }

        *pDest++ = (char)((CHAR64(c1) << 2) | ((CHAR64(c2) & 0x30) >> 4));

        if (c3 == OSA_PRV_B64_PADDING)
        {
            c1 = *pSrc++;

            if (c1 != '\0')
            {
                return -1;
            }

            if (c4 != OSA_PRV_B64_PADDING)
            {
                return -1;
            }

            *dst_len = pDest - dst_buf;
            return *dst_len;
        }

        *pDest++ = (char)(((CHAR64(c2) & 0xF) << 4) | ((CHAR64(c3) & 0x3C) >> 2));

        if (c4 == OSA_PRV_B64_PADDING)
        {
            c1 = *pSrc++;

            if (c1 != '\0')
            {
                return -1;
            }

            *dst_len = pDest - dst_buf;
            return *dst_len;
        }

        *pDest++ = (char)(((CHAR64(c3) & 0x3) << 6) | CHAR64(c4));
    }
}

/**
 * qcm_base64_get_size: Calculate the size after Base64 encoding for a given data length.
 *
 * @param dataLen: The length of the data to be encoded (in bytes).
 *
 * @return Returns the encoded size in bytes.
 */
static int qcm_base64_get_size(int dataLen)
{
    int result_len;

    result_len = 4 * (dataLen / 3);
    if (dataLen % 3)
        result_len += 4;

    return result_len;
}

/**
 * @brief Encode the given binary data using Base64 and store the result in the destination buffer.
 *
 * @param[in] const char * src_data
 *          - Pointer to the binary data to be encoded.
 *
 * @param[in] int src_len
 *          - Length of the binary data to be encoded (in bytes).
 *
 * @param[in] char * dst_buf
 *          - Pointer to the destination buffer for storing the encoded data.
 *
 * @param[in] int * dst_len
 *          - A pointer to a variable that specifies the size of the destination buffer in bytes.
 *
 * @param[in] qosa_bool_t line_wrap
 *          - A boolean value indicating whether to add a line break after every 76 bytes. Used for objective reality, OSA_FALSE means not enabled.
 *
 * @return int
 *       - Returns the actual encoded length greater than 0 upon success
 *       - Returns -2 for parameter errors
 *       - Return -1 if the dst_buff space is insufficient
 */
int qcm_base64_encode(const char *src_data, int src_len, char *dst_buf, int *dst_len, qosa_bool_t line_wrap)
{
    char       *pDest;
    const char *pSrc;
    int         out_len = 0;
    int         result_len = 0;

    if (src_data == QOSA_NULL || src_len <= 0 || dst_buf == QOSA_NULL || dst_len == QOSA_NULL)
    {
        return -2;
    }

    result_len = qcm_base64_get_size(src_len);
    if (result_len > *dst_len)
    {
        return -1;
    }
    *dst_len = 0;

    pSrc = src_data;
    pDest = dst_buf;

    while (src_len)
    {
        unsigned char c1, c2, c3;
        c1 = (unsigned char)*pSrc++;
        *pDest++ = basis_64[c1 >> 2];
        src_len--;
        out_len++;

        if (!src_len)
        {
            c2 = 0;
        }
        else
        {
            c2 = (unsigned char)*pSrc++;
        }

        *pDest++ = basis_64[((c1 & 0x3) << 4) | ((c2 & 0xF0) >> 4)];
        out_len++;
        if (!src_len)
        {
            *pDest++ = OSA_PRV_B64_PADDING;
            *pDest++ = OSA_PRV_B64_PADDING;
            out_len++;
            out_len++;
            break;
        }

        src_len--;
        if (!src_len)
        {
            c3 = 0;
        }
        else
        {
            c3 = (unsigned char)*pSrc++;
        }

        *pDest++ = basis_64[((c2 & 0xF) << 2) | ((c3 & 0xC0) >> 6)];
        out_len++;
        if (!src_len)
        {
            *pDest++ = OSA_PRV_B64_PADDING;
            out_len++;
            break;
        }
        --src_len;

        *pDest++ = basis_64[c3 & 0x3F];
        out_len++;
        if (out_len % BASE64_LINE_WRAP_COUNT == 0)
        {
            if (line_wrap == QOSA_TRUE)
            {
                *pDest++ = '\r';
                *pDest++ = '\n';
            }
            out_len = 0;
        }
    }

    *pDest = '\0';
    *dst_len = (pDest - dst_buf);

    return *dst_len;
}
