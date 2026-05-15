/*****************************************************************/ /**
* @file qcm_utils.h
* @brief
* @author larson.li@quectel.com
* @date 2023-10-14
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2023-10-14 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_Q_UTILS_H__
#define __QOSA_Q_UTILS_H__

#include "qosa_def.h"
#include "qosa_sys.h"

qosa_bool_t qcm_utils_is_hex_number(char c);

/**
 * @brief Convert hexadecimal to numeric value
 *
 * @param  char
 *           - character
 *
 * @return int
 *          - 1 (false)    other: converted decimal value
 */
extern qosa_int32_t qcm_utils_hex_to_num(char c);

/**
 * @brief This function is used to convert a hexadecimal array to hexadecimal number format
 *
 * @param qosa_uint8_t hex[] Input array to be converted (eg: {A0,56,B5})
 * @param qosa_uint32_t len Array length
 * @param char *retStr  Converted numeric value (eg: retstr = "A056B5")
 * @param qosa_bool_t isUpper  Whether the hexadecimal in the input array is uppercase letters
 * @return qosa_uint32_t Length of the converted string
 */
extern qosa_uint32_t qcm_utils_hex_to_str(qosa_uint8_t hex[], qosa_uint32_t hexLen, char *retStr, qosa_bool_t isUpper);

/**
 * @brief Convert hexadecimal data to string
 *
 * @param  char *hex_data
 *           - hexadecimal  string
 *
 * @param  qosa_int32_t length
 *           - hexadecimal  string   size
 *
 * @return qosa_bool_t
 *          - false: Invalid input ; true:  string
 */
extern qosa_bool_t qcm_utils_hex_data_parse(char *hex_data, qosa_int32_t length);

/**
 * @brief  ASCII hexadecimal translation character string
 *
 * @param  char *str_ptr   "313241"
 *           -   string
 *
 * @param  qosa_int32_t length
 *           -   string   size
 *
 * @param  char *hex
 *           -   hexadecimal string   "12A"
 *
 * @return qosa_bool_t
 *          - false: Invalid input ; true:  string
 */
qosa_int32_t qcm_utils_data_string_to_hex(const char *str_ptr, qosa_uint16_t length, qosa_uint8_t *hex);

/**
 * @brief Data conversion ASCII hexadecimal display
 *
 * @param  char *data    "12A"
 *           -   string
 *
 * @param  qosa_uint16_t length
 *           -   string   size
 *
 * @param  char *hexString
 *           -   hexadecimal string   "313241"
 *
 * @return qosa_int32_t
 *
 */
qosa_int32_t qcm_utils_data_to_hex_string(qosa_uint8_t *data, qosa_uint16_t length, char *hexString);

/**
 * @brief Parse attribute string, extract attribute values of specified keywords, and store them in the attribute list.
 *
 * eg:
 *  Suppose there is a string containing device attribute information, for example:
 *     const char *attrString = "Temperature=25.5 Humidity=60.0 Voltage=3.3";
 *
 *  At the same time, you have a keyword array key to specify the attributes you are interested in:
 *     const char *key[] = {"Temperature", "Humidity", "Voltage"};
 *
 *  Then, you can create a large enough attribute value list array to store the parsed attribute values:
 *     #define MAX_ATTR_NUM 3
 *     char *attrList[MAX_ATTR_NUM];
 *
 *  Finally, you can call the qcm_utils_parse_Attr function to parse the string and extract attribute values:
 *     qosa_uint32_t numAttrs = qcm_utils_parse_Attr(attrString, key, attrList, MAX_ATTR_NUM, " ");
 *
 *     // Check if parsing was successful
 *     if (numAttrs > 0) {
 *         // Traverse the attribute value list and process
 *         for (qosa_uint32_t i = 0; i < numAttrs; ++i) {
 *             printf("Attribute %s: %s\n", key[i], attrList[i]);
 *         }
 *     } else {
 *         printf("Failed to parse attributes.\n");
 *     }
 *
 * eg2:
 *    Suppose there is a string:
 *       const char *attrString = "1101;1102;1103"
 *    At the same time, the key keyword array is empty
 *    Then, you can create a large enough attribute value list array to store the parsed attribute values:
 *       #define MAX_ATTR_NUM 3
 *       char *attrList[MAX_ATTR_NUM];
 *    Finally, call the qcm_utils_parse_Attr function to parse the string and extract attribute values:
 *       qosa_uint32_t numAttrs = qcm_utils_parse_Attr(attrString, QOSA_NULL, attrList, MAX_ATTR_NUM, ";");
 *
 *    The result is to split according to ";" and the attrlist array contents are 1101 1102 1103
 *
 * Note: In the above, attrList points to the character address of attrString, there is no need to free attrList
 *
 * @param valueString String to be parsed
 * @param key Array containing keywords for matching attributes
 * @param attrList Array to store parsed attribute values
 * @param attrMaxNum Maximum number of attributes to avoid array out of bounds
 * @param delim Delimiter used to split the string
 *
 * @return Number of parsed attributes
 *
 */
qosa_uint32_t qcm_utils_parse_Attr(const char *valueString, const char *key[], char *attrList[], qosa_uint32_t attrMaxNum, const char *delim);

/**
 * @brief Converts a text buffer representing a decimal number to a 64-bit integer.
 *
 * @param buffer The buffer containing the text representation of the number.
 * @param length The length of the buffer.
 * @param dataP Pointer to the 64-bit integer where the result will be stored.
 * @return int Returns 1 if the conversion was successful, 0 otherwise.
 */
int qcm_utils_text_to_int64(qosa_uint8_t *buffer, int length, qosa_int64_t *dataP);

/**
 * @brief This function is used to convert a string type decimal number to a numeric value
 *
 * @param qosa_uint8_t *src Input string to be converted (eg: "567")
 * @param qosa_uint32_t len Length of the string
 * @param qosa_uint32_t *value  Converted numeric value (eg: value = 567)
 * @return qosa_bool_t
 */
qosa_bool_t qcm_utils_data_string_to_uint32(qosa_uint8_t *src, qosa_uint32_t len, qosa_uint32_t *value);

/**
 * @brief Converts a text buffer representing a decimal number to a 64-bit floating point number.
 *
 * @param buffer The buffer containing the text representation of the number.
 * @param length The length of the buffer.
 * @param dataP Pointer to the double where the result will be stored.
 * @return int Returns 1 if the conversion was successful, 0 otherwise.
 */
int qcm_utils_text_to_float64(qosa_uint8_t *buffer, int length, double *dataP);

/**
 * @brief This function is used to convert qosa_int64_t type to string
 *
 * @param qosa_int64_t v Input parameter to be converted
 * @param char* s Output converted string
 * @param  qosa_uint16_t r Input base to be converted to
 * @return
 */
void qcm_utils_int64_to_text(qosa_int64_t v, char *s, qosa_uint16_t r);

/**
 * @brief Convert the address, replace characters in the IP address 072.023.111.011==72.23.111.11
 *
 * @param[in] char * ip_string
 *          - Dotted decimal IP address data before conversion
 *
 * @param[in] char * dst_string
 *          - Dotted decimal IP address string after conversion
 *
* FUNCTION unir_tcpip_address_replace_zero
* DESCRIPTION
*
* replace ipv4 address more zero
* eg:010.012.122.122 -->10.12.122.122
*/

int qcm_utils_address_replace_zero(char *ip_string, char *dst_string);

#endif /* __QOSA_Q_UTILS_H__ */
