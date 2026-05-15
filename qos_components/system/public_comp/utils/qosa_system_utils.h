/*****************************************************************/ /**
* @file qosa_system_utils.h
* @brief
* @author larson.li@quectel.com
* @date 2025-01-24
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2025-01-24 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_SYSTEM_UTILS_H__
#define __QOSA_SYSTEM_UTILS_H__

#include "qosa_def.h"
#include "qosa_sys.h"

/**
 * helper data structure for integer string map
 */
typedef struct
{
    qosa_uint32_t value; /*!< integer value */
    const char   *str;   /*!< string value */
} qosa_value_str_map_t;

/**
 * @brief Convert qosa_int64_t type data to string and store in qosa_uint8_t array
 *
 * @param[in] data The qosa_int64_t type data to convert
 *
 * @param[out] string Array to store the converted string
 *
 * @param[in] length Length of the array
 *
 * @return qosa_size_t Length of the converted string, returns 0 if conversion fails
 */
qosa_size_t qosa_utils_int_to_str(qosa_int64_t data, qosa_uint8_t *string, qosa_size_t length);

/**
 * @brief Convert qosa_uint64_t type data to string and store in qosa_uint8_t array
 *
 * @param[in] data The qosa_uint64_t type data to convert
 *
 * @param[out] string Array to store the converted string
 *
 * @param[in] length Length of the array
 *
 * @return qosa_size_t Length of the converted string, returns 0 if conversion fails
 */
qosa_size_t qosa_utils_uint_to_str(qosa_uint64_t data, qosa_uint8_t *string, qosa_size_t length);

/**
 * @brief Converts a floating-point number to a string.
 *
 * @param n The floating-point number to be converted.
 * @param res The buffer where the resulting string will be stored.
 * @param precision The number of digits to be printed after the decimal point.
 */
qosa_size_t qosa_utils_double_to_str(double data, qosa_uint8_t *string, qosa_size_t length);

/**
 * @brief Convert a double value to string with specified precision
 *
 * This function converts a double floating-point number to its string representation
 * with a specified number of decimal places. It handles rounding, trailing zero removal,
 * and proper formatting of negative numbers.
 *
 * @param[in] data The double value to convert
 * @param[out] string Output buffer to store the converted string
 * @param[in] length Size of the output buffer (must be at least 2 bytes)
 * @param[in] precision Number of decimal places to preserve (0-15, defaults to 15 if out of range)
 *
 * @return qosa_size_t Length of the resulting string, or 0 on error
 *
*/
qosa_size_t qosa_utils_double_to_str_with_precision(double data, qosa_uint8_t *string, qosa_size_t length, qosa_uint8_t precision);

/**
 * @brief Converts a byte to a bit string.
 *
 * @param[in] byte
 *          - The byte to be converted.
 * @param[out] bit_string
 *          - The buffer where the resulting bit string will be stored, should be at least 9 bytes long.
 */
void qosa_utils_byte_to_bit_string(unsigned char byte, char *bit_string);

/**
 * @brief Finds a value-string map entry by case insensitive string comparison.
 *
 * @param[in] const qosa_value_str_map_t * vsmap
 *          - A pointer to the value-string map.
 *
 * @param[in] const char * str
 *          - The string to compare.
 *
 * @return const qosa_value_str_map_t *
 *       - A pointer to the found entry, or QOSA_NULL if not found.
 */
const qosa_value_str_map_t *qosa_Vs_map_find_by_istr(const qosa_value_str_map_t *vsmap, const char *str);

/**
 * @brief Finds a value-string map entry by value.
 *
 * @param[in] const qosa_value_str_map_t * vsmap
 *          - A pointer to the value-string map.
 *
 * @param[in] qosa_uint32_t value
 *          - The value to compare.
 *
 * @return const qosa_value_str_map_t *
 *       - A pointer to the found entry, or QOSA_NULL if not found.
 */
const qosa_value_str_map_t *qosa_Vs_map_find_by_value(const qosa_value_str_map_t *vsmap, qosa_uint32_t value);

#endif /* __QOSA_SYSTEM_UTILS_H__ */
