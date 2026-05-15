/*****************************************************************/ /**
* @file qcm_tlv.h
* @brief
* @author lambert.zhao@quectel.com
* @date 2024-07-30
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-07-30 <td>1.0 <td>Lambert.Zhao <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_TLV_H__
#define __QOSA_TLV_H__

#include "qosa_def.h"
#include "qosa_sys.h"

/**
 * @enum qcm_asn_tag_class_e
 * @brief Class to which the TAG belongs
 */
typedef enum
{
    QCM_ASN_TAG_CLASS_UNIVERSAL = 0,   /*!< 0b00 */
    QCM_ASN_TAG_CLASS_APPLICATION = 1, /*!< 0b01 */
    QCM_ASN_TAG_CLASS_CONTEXT = 2,     /*!< 0b10 */
    QCM_ASN_TAG_CLASS_PRIVATE = 3      /*!< 0b11 */
} qcm_asn_tag_class_e;

/** BER TAG from Tag-Length-Value */
typedef unsigned qcm_ber_tlv_tag_t;
/** BER LEN from Tag-Length-Value */
typedef qosa_ssize_t qcm_ber_tlv_len_t;

/** Tag class is encoded together with tag value for optimization purposes */
#define QCM_BER_TAG_CLASS(tag)          ((tag)&0x3)
#define QCM_BER_TAG_VALUE(tag)          ((tag) >> 2)
#define QCM_BER_TLV_CONSTRUCTED(tagptr) (((*(const qosa_uint8_t *)tagptr) & 0x20) ? 1 : 0)

/**
 * @brief Get the tag from the input stream
 *
 * @param[in] const void *bufptr
 *          - Input data stream
 *
 * @param[in] qosa_size_t size
 *          - Size of the input data stream
 *
 * @param[in out] qcm_ber_tlv_tag_t *tag_r
 *          - T in TLV
 *
 * @return qosa_ssize_t
 *       - Returns 0 if the expected data is more than bufptr contains
 *       - Returns -1 if a fatal error occurs during tag decoding
 *       - Returns the number of bytes used in bufptr if successful. tag_r will contain the tag
 */
qosa_ssize_t qcm_ber_fetch_tag(const void *bufptr, qosa_size_t size, qcm_ber_tlv_tag_t *tag_r);

/**
 * @brief Serialize the tag in BER format (T in TLV)
 *
 * @param[in] qcm_ber_tlv_tag_t tag
 *          - T in TLV
 *
 * @param[in out] void *bufptr
 *          - Buffer for storing the serialized tag
 *
 * @param[in] qosa_size_t size
 *          - Size of the bufptr buffer
 *
 * @return qosa_size_t
 *       - Returns the number of bytes required for the tag, the caller needs to check the return value against the provided buffer size
 */
qosa_size_t qcm_ber_tlv_tag_serialize(qcm_ber_tlv_tag_t tag, void *bufptr, qosa_size_t size);

/**
 * @brief Serialize the length in DER format (L in TLV)
 *
 * @param[in] qcm_ber_tlv_len_t len
 *          - L in TLV
 *
 * @param[in out] void *bufp
 *          - Buffer for storing the serialized length
 *
 * @param[in] qosa_size_t size
 *          - Size of the bufp buffer
 *
 * @return qosa_size_t
 *       - Returns the number of bytes required for the length, the caller needs to check the return value against the provided buffer size
 */
qosa_size_t qcm_der_tlv_length_serialize(qcm_ber_tlv_len_t len, void *bufp, qosa_size_t size);

/**
 * @brief Get the length of BER TLV value
 *
 * @param[in] qosa_int32_t _is_constructed
 *          - Indicates whether it is a nested encoding format
 *
 * @param[in] const void *bufptr
 *          - Input data stream
 *
 * @param[in] qosa_size_t size
 *          - Size of the input data stream
 *
 * @param[in out] qcm_ber_tlv_len_t *len_r
 *          - L in TLV
 *
 * @return qosa_ssize_t
 *       - Returns 0 if the expected data is more than bufptr contains
 *       - Returns -1 if a fatal error occurs during tag decoding
 *       - Returns the number of bytes used in bufptr if successful. len_r will contain the length
 *       - A value of -1 for len_r indicates that V in TLV has an indefinite length
 */
qosa_ssize_t qcm_ber_fetch_length(qosa_int32_t _is_constructed, const void *bufptr, qosa_size_t size, qcm_ber_tlv_len_t *len_r);

#endif /* __QOSA_TLV_H__ */
