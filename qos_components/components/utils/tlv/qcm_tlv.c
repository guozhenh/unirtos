/*****************************************************************/ /**
* @file qcm_tlv.c
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
#include "qcm_tlv.h"
#include "qosa_def.h"

/**
 * @brief The tag field in the serialized TLV structure is in BER encoding format.
 * 
 * This function encodes the tag value into BER (Basic Encoding Rules) format. In the BER encoding rules,
 * The first byte of the tag contains the class and tag value information. If the tag value
 * If less than or equal to 30, it is directly encoded in the first byte; if the tag value is greater than 30, the first byte
 * The tag value portion is set to 0x1F, and multi-byte encoding is used for the tag value in subsequent bytes.
 * 
 * @param[in] tag The tag value to be encoded, containing category and tag value information
 * @param[out] bufp Pointer to the buffer storing the encoding result; if NULL, only calculates the required space size
 * @param[in] size Buffer size; if 0, only calculates the required space size
 * 
 * @return Returns the total number of encoded bytes
 * @retval 1 When the tag value is <=30, only 1 byte is required to store the tag.
 * @retval When the tag value is greater than 30, n bytes are required (1 header byte + (n-1) tag value bytes)
 * 
 * @note BER Tag Encoding Rules:
 *       1. The high 2 bits of the first byte indicate the class: 00=UNIVERSAL, 01=APPLICATION,
 *          10=CONTEXT_SPECIFIC, 11=PRIVATE
 *       2. The lower 6 bits or lower 5 bits of the first byte represent the tag value:
 *          - When the tag value is <=30, the lower 6 bits directly represent the tag value
 *          - When the tag value > 30, the lower 5 bits are 0x1F (31), and the tag value is encoded in subsequent bytes
 *       3. In the subsequent bytes, except for the last byte, the highest bit of all other bytes is 1, indicating that there are more bytes to follow.
 *       4. The highest bit of the last byte is 0, indicating the end.
 * 
 * @see QCM_BER_TAG_CLASS() is used to extract the tag class
 * @see QCM_BER_TAG_VALUE() is used to extract tag values
 */
qosa_size_t qcm_ber_tlv_tag_serialize(qcm_ber_tlv_tag_t tag, void *bufp, qosa_size_t size)
{
    qosa_int32_t      tclass = QCM_BER_TAG_CLASS(tag);
    qcm_ber_tlv_tag_t tval = QCM_BER_TAG_VALUE(tag);
    qosa_uint8_t     *buf = (qosa_uint8_t *)bufp;
    qosa_uint8_t     *end;
    qosa_size_t       required_size;
    qosa_size_t       i;

    if (tval <= 30)
    {
        /* Encoded in 1 octet */
        if (size)
        {
            buf[0] = (tclass << 6) | tval;
        }
        return 1;
    }
    else if (size)
    {
        *buf++ = (tclass << 6) | 0x1F;
        size--;
    }

    /*
	 * Compute the size of the subsequent bytes.
	 */
    for (required_size = 1, i = 7; i < 8 * sizeof(tval); i += 7)
    {
        if (tval >> i)
        {
            required_size++;
        }
        else
        {
            break;
        }
    }

    if (size < required_size)
    {
        return required_size + 1;
    }

    /*
	 * Fill in the buffer, space permitting.
	 */
    end = buf + required_size - 1;
    for (i -= 7; buf < end; i -= 7, buf++)
    {
        *buf = 0x80 | ((tval >> i) & 0x7F);
    }
    *buf = (tval & 0x7F); /* Last octet without high bit */

    return required_size + 1;
}

qosa_ssize_t qcm_ber_fetch_tag(const void *ptr, qosa_size_t size, qcm_ber_tlv_tag_t *tag_r)
{
    qcm_ber_tlv_tag_t val;
    qcm_ber_tlv_tag_t tclass;
    qosa_size_t       skipped;

    if (size == 0)
    {
        return 0;
    }

    val = *(const qosa_uint8_t *)ptr;
    tclass = (val >> 6);
    if ((val &= 0x1F) != 0x1F)
    {
        /*
		 * Simple form: everything encoded in a single octet.
		 * Tag Class is encoded using two least significant bits.
		 */
        *tag_r = (val << 2) | tclass;
        return 1;
    }

    /*
	 * Each octet contains 7 bits of useful information.
	 * The MSB is 0 if it is the last octet of the tag.
	 */
    for (val = 0, ptr = ((const char *)ptr) + 1, skipped = 2; skipped <= size; ptr = ((const char *)ptr) + 1, skipped++)
    {
        qosa_uint32_t oct = *(const qosa_uint8_t *)ptr;
        if (oct & 0x80)
        {
            val = (val << 7) | (oct & 0x7F);
            /*
			 * Make sure there are at least 9 bits spare
			 * at the MS side of a value.
			 */
            if (val >> ((8 * sizeof(val)) - 9))
            {
                /*
				 * We would not be able to accomodate
				 * any more tag bits.
				 */
                return -1;
            }
        }
        else
        {
            val = (val << 7) | oct;
            *tag_r = (val << 2) | tclass;
            return skipped;
        }
    }

    return 0; /* Want more */
}

/**
 * @brief The length field in the serialized TLV structure is in DER encoding format.
 * 
 * This function encodes the length value into DER (Distinguished Encoding Rules) format, which is a subset of BER (Basic Encoding Rules)
 * a specific implementation. For length values less than or equal to 127, a single byte is used directly; for length values greater than 127,
 * Using a multi-byte encoding format, the highest bit of the first byte is 1, and the lower 7 bits indicate the number of subsequent bytes, which store the actual length value.
 * 
 * @param[in] len The length value to be encoded
 * @param[out] bufp Pointer to the buffer storing the encoding result; if NULL, only calculates the required space size
 * @param[in] size Buffer size; if 0, only calculates the required space size
 * 
 * @return Returns the total number of bytes after encoding (including the identifier byte)
 * @retval 1 When len <= 127, only 1 byte is needed to store the length
 * @retval When len > 127, n bytes are required (1 identifier byte + (n-1) length bytes)
 * 
 * @note If bufp is NULL or the size is insufficient, the function will not write data, but will still return the required minimum buffer size.
 */
qosa_size_t qcm_der_tlv_length_serialize(qcm_ber_tlv_len_t len, void *bufp, qosa_size_t size)
{
    qosa_size_t   required_size; /* Size of len encoding */
    qosa_uint8_t *buf = (qosa_uint8_t *)bufp;
    qosa_uint8_t *end;
    qosa_size_t   i;

    if (len <= 127)
    {
        /* Encoded in 1 octet */
        if (size)
            *buf = (qosa_uint8_t)len;
        return 1;
    }

    /*
	 * Compute the size of the subsequent bytes.
	 */
    for (required_size = 1, i = 8; i < 8 * sizeof(len); i += 8)
    {
        if (len >> i)
        {
            required_size++;
        }
        else
        {
            break;
        }
    }

    if (size <= required_size)
    {
        return required_size + 1;
    }

    *buf++ = (qosa_uint8_t)(0x80 | required_size); /* Length of the encoding */

    /*
	 * Produce the len encoding, space permitting.
	 */
    end = buf + required_size;
    for (i -= 8; buf < end; i -= 8, buf++)
    {
        *buf = (qosa_uint8_t)(len >> i);
    }

    return required_size + 1;
}

qosa_ssize_t qcm_ber_fetch_length(qosa_int32_t _is_constructed, const void *bufptr, qosa_size_t size, qcm_ber_tlv_len_t *len_r)
{
    const qosa_uint8_t *buf = (const qosa_uint8_t *)bufptr;
    qosa_uint32_t       oct;

    if (size == 0)
    {
        return 0; /* Want more */
    }

    oct = *(const qosa_uint8_t *)buf;
    if ((oct & 0x80) == 0)
    {
        /*
		 * Short definite length.
		 */
        *len_r = oct; /* & 0x7F */
        return 1;
    }
    else
    {
        qcm_ber_tlv_len_t len;
        qosa_size_t       skipped;

        if (_is_constructed && oct == 0x80)
        {
            *len_r = -1; /* Indefinite length */
            return 1;
        }

        if (oct == 0xff)
        {
            /* Reserved in standard for future use. */
            return -1;
        }

        oct &= 0x7F; /* Leave only the 7 LS bits */
        for (len = 0, buf++, skipped = 1; oct && (++skipped <= size); buf++, oct--)
        {
            len = (len << 8) | *buf;
            if (len < 0 || (len >> ((8 * sizeof(len)) - 8) && oct > 1))
            {
                /*
				 * Too large length value.
				 */
                return -1;
            }
        }

        if (oct == 0)
        {
            qcm_ber_tlv_len_t lenplusepsilon = (qosa_size_t)len + 1024;
            /*
			 * Here length may be very close or equal to 2G.
			 * However, the arithmetics used in some decoders
			 * may add some (small) quantities to the length,
			 * to check the resulting value against some limits.
			 * This may result in integer wrap-around, which
			 * we try to avoid by checking it earlier here.
			 */
            if (lenplusepsilon < 0)
            {
                /* Too large length value */
                return -1;
            }

            *len_r = len;
            return skipped;
        }

        return 0; /* Want more */
    }
}
