/*****************************************************************/ /**
* @file qcm_utf8.c
* @brief
* @author larson.li@quectel.com
* @date 2023-06-11
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2023-06-11 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#include "qcm_utf8.h"
#include "qosa_def.h"

#ifndef ARR_SIZE
#define ARR_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

struct
{
    qosa_uint32_t len; /**< number of elements in the following array (1 to 4) */
    struct
    {
        char lower; /**< lower limit of valid range */
        char upper; /**< upper limit of valid range */
    } bytes[4];     /**< up to 4 bytes can be used per character */
} qcm_utf8_valid_ranges[] = {
    {1, {{00, 0x7F}}},
    {2, {{0xC2, 0xDF}, {0x80, 0xBF}}},
    {3, {{0xE0, 0xE0}, {0xA0, 0xBF}, {0x80, 0xBF}}},
    {3, {{0xE1, 0xEC}, {0x80, 0xBF}, {0x80, 0xBF}}},
    {3, {{0xED, 0xED}, {0x80, 0x9F}, {0x80, 0xBF}}},
    {3, {{0xEE, 0xEF}, {0x80, 0xBF}, {0x80, 0xBF}}},
    {4, {{0xF0, 0xF0}, {0x90, 0xBF}, {0x80, 0xBF}, {0x80, 0xBF}}},
    {4, {{0xF1, 0xF3}, {0x80, 0xBF}, {0x80, 0xBF}, {0x80, 0xBF}}},
    {4, {{0xF4, 0xF4}, {0x80, 0x8F}, {0x80, 0xBF}, {0x80, 0xBF}}},
};

/**
 * @brief Validate a single UTF-8 character
 * @param int len
 *           - the length of the string in "data"
 *
 * @param const char *data
 *           - the bytes to check for a valid UTF-8 char
 *
 * @return const char *
 *          -  pointer to the start of the next UTF-8 character in "data"
 */
static const char *qcm_utf8_char_validate(qosa_uint32_t len, const char *data)
{
    int           good = 0;
    qosa_uint32_t charlen = 2;
    qosa_uint32_t i, j;
    const char   *rc = QOSA_NULL;

    /* first work out how many bytes this char is encoded in */
    if ((data[0] & 128) == 0)
        charlen = 1;
    else if ((data[0] & 0xF0) == 0xF0)
        charlen = 4;
    else if ((data[0] & 0xE0) == 0xE0)
        charlen = 3;

    if (charlen > len)
        goto exit; /* not enough characters in the string we were given */

    for (i = 0; i < ARR_SIZE(qcm_utf8_valid_ranges); ++i)
    { /* just has to match one of these rows */
        if (qcm_utf8_valid_ranges[i].len == charlen)
        {
            good = 1;
            for (j = 0; j < charlen; ++j)
            {
                if (data[j] < qcm_utf8_valid_ranges[i].bytes[j].lower || data[j] > qcm_utf8_valid_ranges[i].bytes[j].upper)
                {
                    good = 0; /* failed the check */
                    break;
                }
            }
            if (good)
                break;
        }
    }

    if (good)
        rc = data + charlen;
exit:
    return rc;
}

/**
 * @brief Validate a length-delimited string has only UTF-8 characters
 *
 * @param int len
 *          - the length of the string in "data"
 *
 * @param const char *data
 *          - the bytes to check for valid UTF-8 characters
 *
 * @return int
 *          - 1 (true) if the string has only UTF-8 characters, 0 (false) otherwise
 */
static int qcm_utf8_validate(qosa_uint32_t len, const char *data)
{
    const char *curdata = QOSA_NULL;
    int         rc = 0;

    if (len == 0)
    {
        rc = 1;
        goto exit;
    }
    curdata = qcm_utf8_char_validate(len, data);
    while (curdata && (curdata < data + len)) curdata = qcm_utf8_char_validate(len, curdata);

    rc = curdata != QOSA_NULL;
exit:
    return rc;
}

/**
 * @brief Validate a null-terminated string has only UTF-8 characters
 *
 * @param const char *string
 *           - the string to check for valid UTF-8 characters
 *
 * @return int
 *          - 1 (true) if the string has only UTF-8 characters, 0 (false) otherwise
 */
int qcm_utf8_validate_string(const char *string)
{
    int rc = 0;

    rc = qcm_utf8_validate(qosa_strlen(string), string);

    return rc;
}
