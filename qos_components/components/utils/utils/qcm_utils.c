/*****************************************************************/ /**
* @file utils.c
* @brief
* @author larson.li@quectel.com
* @date 2023-10-14
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description
* <tr><td>2023-10-14 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#include <float.h>
#include "qcm_utils.h"
#include "qosa_def.h"
#include "qosa_sys.h"

qosa_bool_t qcm_utils_is_hex_number(char c)
{
    qosa_bool_t result = QOSA_FALSE;

    if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
    {
        result = QOSA_TRUE;
    }
    return (result);
}

/**
 * @brief Convert hexadecimal to numeric value
 *
 * @param  char
 *           - character
 *
 * @return int
 *          - 1 (false)    other: converted decimal value
 */
qosa_int32_t qcm_utils_hex_to_num(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return 0xa + (c - 'A');
    if (c >= 'a' && c <= 'f')
        return 0xa + (c - 'a');
    return -1;
}

/**
 * @brief This function is used to convert a hexadecimal array into the form of a hexadecimal number.
 *
 * @param qosa_uint8_t hex[] Input the array to be converted (e.g., {A0,56,B5})
 * @param qosa_uint32_t len array length
 * @param char *retStr  The converted value (e.g., retstr = "A056B5")
 * @param qosa_bool_t isUpper  Whether the input array's hexadecimal is uppercase letters
 * @return qosa_uint32_t converted string length
 */
qosa_uint32_t qcm_utils_hex_to_str(qosa_uint8_t hex[], qosa_uint32_t hexLen, char *retStr, qosa_bool_t isUpper)
{
    char          Hex2strLBuf[] = "0123456789abcdef";
    char          Hex2strUBuf[] = "0123456789ABCDEF";
    qosa_uint32_t i;
    if (isUpper)
    {
        for (i = 0; i < hexLen; i++)
        {
            retStr[i << 1] = Hex2strUBuf[hex[i] >> 4];
            retStr[(i << 1) + 1] = Hex2strUBuf[hex[i] & 0x0F];
        }
    }
    else
    {
        for (i = 0; i < hexLen; i++)
        {
            retStr[i << 1] = Hex2strLBuf[hex[i] >> 4];
            retStr[(i << 1) + 1] = Hex2strLBuf[hex[i] & 0x0F];
        }
    }
    retStr[i << 1] = 0;
    return hexLen * 2;
}

/**
 * @brief Convert hexadecimal data to string
 *
 * @param[in/out]  char *hex_data     [in]"313241" [out]"12A"
 *           - hexadecimal  string
 * qosa_int32_t length
 *           - hexadecimal  string   size
 * @return bool
 *          - false: Invalid input ; true:  string
 */
qosa_bool_t qcm_utils_hex_data_parse(char *hex_data, qosa_int32_t length)
{
    if (hex_data == QOSA_NULL)
        return QOSA_FALSE;

    if (length % 2 != 0)
        return QOSA_FALSE;

    const char  *pv = hex_data;
    qosa_int32_t n = 0;
    for (n = 0; n < length; n++)
    {
        if (qcm_utils_hex_to_num(*pv++) < 0)
            return QOSA_FALSE;
    }

    char *s = hex_data;
    char *d = hex_data;
    length /= 2;
    for (n = 0; n < length; n++)
    {
        qosa_int32_t num0 = qcm_utils_hex_to_num(s[0]);
        qosa_int32_t num1 = qcm_utils_hex_to_num(s[1]);
        *d++ = (num0 << 4) | num1;
        s += 2;
    }
    *d = 0;
    return QOSA_TRUE;
}

/**
 * @brief  ASCII hexadecimal translation character string
 *
 * @param  char *str_ptr   "313241"
 *           -   string
 * qosa_int32_t length
 *           -   string   size
 * char *hex
 *           -   hexadecimal string   "12A"
 *  * @return bool
 *          - false: Invalid input ; true:  string
 */
qosa_int32_t qcm_utils_data_string_to_hex(const char *str_ptr, qosa_uint16_t length, qosa_uint8_t *hex)
{
    qosa_uint16_t i = 0, j = 0;
    qosa_uint8_t  hex_value = 0;

    if (QOSA_NULL == str_ptr || ((length & 0x01) != 0))
    {
        return QOSA_ERROR_GENERAL;
    }

    for (i = 0; i < length; i++)
    {
        if (str_ptr[i] >= '0' && str_ptr[i] <= '9')
        {
            hex_value += str_ptr[i] - '0';
        }
        else if (str_ptr[i] >= 'a' && str_ptr[i] <= 'f')
        {
            hex_value += str_ptr[i] - 'a' + 0x0a;
        }
        else if (str_ptr[i] >= 'A' && str_ptr[i] <= 'F')
        {
            hex_value += str_ptr[i] - 'A' + 0x0a;
        }
        else
        {
            return QOSA_ERROR_GENERAL;
        }

        if (i & 0x01)
        {
            hex[j++] = hex_value;
            hex_value = 0;
        }
        else
        {
            hex_value <<= 4;
        }
    }

    return QOSA_OK;
}

/**
 * @brief Data conversion ASCII hexadecimal display
 *
 * @param  char *data    "12A"
 *           -   string
 * qosa_uint16_t length
 *           -   string   size
 * char *hexString
 *           -   hexadecimal string   "313241"
 *  * @return qosa_int32_t
 *
 */
qosa_int32_t qcm_utils_data_to_hex_string(qosa_uint8_t *data, qosa_uint16_t length, char *hexString)
{
#if 0
    qosa_uint32_t i = 0;

    for (i = 0; i < length; i++)
    {
        qosa_sprintf(&hexString[i * 2], "%02X", data[i]);
    }

    return QOSA_OK;
#else
    qosa_uint32_t i = 0;
    const char    hexChars[] = "0123456789ABCDEF";

    for (i = 0; i < length; i++)
    {
        qosa_uint8_t byte = data[i];
        hexString[i * 2] = hexChars[(byte >> 4) & 0x0F];
        hexString[i * 2 + 1] = hexChars[byte & 0x0F];
    }

    hexString[i * 2] = 0;  //Add string terminator
    return QOSA_OK;
#endif
}

/**
 * @brief Parse the attribute string, extract the attribute values for specified keywords, and store them into the attribute list.
 *
 * eg:
 *  Suppose there is a string containing device attribute information, for example:
 *     const char *attrString = "Temperature=25.5 Humidity=60.0 Voltage=3.3";
 *
 *  Meanwhile, you have a keyword array named key, which is used to specify the properties you are interested in:
 *     const char *key[] = {"Temperature", "Humidity", "Voltage"};
 *
 *  Then, you can create a property value list array large enough to store the parsed property values:
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
 *    Assuming there is a string:
 *       const char *attrString = "1101;1102;1103"
 *    Meanwhile, the key keyword array is empty
 *    Then, you can create a property value list array large enough to store the parsed property values:
 *       #define MAX_ATTR_NUM 3
 *       char *attrList[MAX_ATTR_NUM];
 *    Finally, call the qcm_utils_parse_Attr function to parse the string and extract the attribute values:
 *       qosa_uint32_t numAttrs = qcm_utils_parse_Attr(attrString, QOSA_NULL, attrList, MAX_ATTR_NUM, ";");
 *
 *    The result is to split the attrlist array content by ";" into 1101 1102 1103
 *
 * Note: The attrList above points to the character address of attrString and does not need to be freed.
 *
 * @param valueString The string to be parsed
 * @param key is an array containing keywords, used for matching attributes
 * @param attrList stores the array of parsed attribute values
 * @param attrMaxNum Maximum number of attributes, to prevent array out-of-bounds
 * @param delim The delimiter used to split the string
 *
 * @return Number of attributes parsed
 *
 */
qosa_uint32_t qcm_utils_parse_Attr(const char *valueString, const char *key[], char *attrList[], qosa_uint32_t attrMaxNum, const char *delim)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    qosa_int32_t  i;
    qosa_uint32_t attrNum = 0;
    char         *buffer = QOSA_NULL;  //Used to store temporary fields during the parsing process
    char         *p = qosa_strtok((char *)valueString, delim);
    char         *field = QOSA_NULL;
    qosa_uint32_t fieldLen;
    char         *attrValue = QOSA_NULL;

    //Dynamically allocate buffer
    buffer = (char *)qosa_malloc(qosa_strlen(valueString) + 1);
    if (buffer == QOSA_NULL)
    {
        //Handle memory allocation failure cases
        return 0;
    }

    field = (char *)buffer;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (p != QOSA_NULL && *p != '\0' && attrNum < attrMaxNum)
    {
        qosa_strcpy(field, p);
        //Remove leading spaces
        while (*field == 0x20)
        {
            field++;
        }
        //Remove trailing spaces
        fieldLen = qosa_strlen(field);
        for (i = fieldLen - 1; i >= 0; i--)
        {
            if (*(field + i) == 0x20)
            {
                *(field + i) = '\0';
            }
            else
            {
                break;
            }
        }
        if (key != QOSA_NULL)
        {
            //Find '='
            attrValue = qosa_strchr(field, '=');
            if (attrValue == QOSA_NULL || qosa_strncmp(field, key[attrNum], qosa_strlen(key[attrNum])) != 0)
            {
                break;
            }
            attrValue++;
            //Remove leading spaces
            while (*attrValue == 0x20)
            {
                attrValue++;
            }
        }
        else
        {
            attrValue = field;
        }
        //Add to property list
        attrList[attrNum++] = attrValue;

        field = field + qosa_strlen(field) + 1;
        p = qosa_strtok(QOSA_NULL, (char *)delim);
    }

    //Other release operations
    qosa_free(buffer);

    return attrNum;
}

#include <stdint.h>
/**
 * @brief Converts a text buffer representing a decimal number to a 64-bit integer.
 *
 * @param buffer The buffer containing the text representation of the number.
 * @param length The length of the buffer.
 * @param dataP Pointer to the 64-bit integer where the result will be stored.
 * @return int Returns 1 if the conversion was successful, 0 otherwise.
 */
int qcm_utils_text_to_int64(qosa_uint8_t *buffer, int length, qosa_int64_t *dataP)
{
    qosa_uint64_t result = 0;
    int           sign = 1;
    int           i = 0;

    if (0 == length)
    {
        return 0;
    }

    if (buffer[0] == '-')
    {
        sign = -1;
        i = 1;
    }

    while (i < length)
    {
        if ('0' <= buffer[i] && buffer[i] <= '9')
        {
            if (result > (UINT64_MAX / 10))
            {
                return 0;
            }
            result *= 10;
            result += buffer[i] - '0';
        }
        else
        {
            return 0;
        }
        i++;
    }
    if (result > (sign == 1 ? (qosa_uint64_t)INT64_MAX : (qosa_uint64_t)((qosa_uint64_t)INT64_MAX + 1)))
    {
        return 0;
    }

    if (sign == -1)
    {
        *dataP = 0 - result;
    }
    else
    {
        *dataP = result;
    }

    return 1;
}

/**
 * @brief This function is used to convert a string-type decimal number into a numerical value.
 *
 * @param qosa_uint8_t *src Input string to be converted (e.g., "567")
 * @param qosa_uint32_t len The length of the string
 * @param qosa_uint32_t *value  The converted numerical value (e.g., value = 567)
 * @return qosa_bool_t
 */
qosa_bool_t qcm_utils_data_string_to_uint32(qosa_uint8_t *src, qosa_uint32_t len, qosa_uint32_t *value)
{
    qosa_uint32_t i = 0;

    if (QOSA_NULL == src || 0 == len || (len > 1 && '0' == src[0]))
    {
        return QOSA_FALSE;
    }

    if (value)
    {
        *value = 0;
    }

    for (i = 0; i < len; i++)
    {
        if (src[i] < '0' || src[i] > '9')
        {
            return QOSA_FALSE;
        }
        else if (value)
        {
            *value = (*value) * 10 + src[i] - '0';
        }
    }
    return QOSA_TRUE;
}

/**
 * @brief Converts a text buffer representing a decimal number to a 64-bit floating point number.
 *
 * @param buffer The buffer containing the text representation of the number.
 * @param length The length of the buffer.
 * @param dataP Pointer to the double where the result will be stored.
 * @return int Returns 1 if the conversion was successful, 0 otherwise.
 */
int qcm_utils_text_to_float64(qosa_uint8_t *buffer, int length, double *dataP)
{
    double result;
    int    sign;
    int    i;

    if (0 == length)
    {
        return 0;
    }

    if (buffer[0] == '-')
    {
        sign = -1;
        i = 1;
    }
    else
    {
        sign = 1;
        i = 0;
    }

    result = 0;
    while (i < length && buffer[i] != '.')
    {
        if ('0' <= buffer[i] && buffer[i] <= '9')
        {
            if (result > (DBL_MAX / 10))
            {
                return 0;
            }
            result *= 10;
            result += (buffer[i] - '0');
        }
        else
        {
            return 0;
        }
        i++;
    }
    if (buffer[i] == '.')
    {
        double dec;

        i++;
        if (i == length)
        {
            return 0;
        }

        dec = 0.1;
        while (i < length)
        {
            if ('0' <= buffer[i] && buffer[i] <= '9')
            {
                if (result > (DBL_MAX - 1))
                {
                    return 0;
                }
                result += (buffer[i] - '0') * dec;
                dec /= 10;
            }
            else
            {
                return 0;
            }
            i++;
        }
    }

    *dataP = result * sign;
    return 1;
}

/**
 * @brief This function is used to convert the qosa_int64_t type to a string.
 *
 * @param qosa_int64_t v Input parameter to be converted
 * @param char* s Output the converted string
 * @param  qosa_uint16_t r Input the base to convert to
 * @return
 */
void qcm_utils_int64_to_text(qosa_int64_t v, char *s, qosa_uint16_t r)
{
    char         buf[65] = {0};
    char         c;
    int          n;
    qosa_uint8_t negative = 0;

    n = sizeof(buf) - 1;
    buf[n] = '\0';

    if (v & 0x8000000000000000)
    {
        negative = 1;
        v *= (-1);
    }
    do {
        c = (char)(v % r);

        if (n <= 0)
        {
            break;
        }

        buf[--n] = (char)((c > 9) ? c + 'A' - 10 : c + '0');
    } while ((v /= r) > 0);

    if (negative == 1)
    {
        *s++ = '-';
    }
    while ((*s++ = buf[n++]) != 0)
        ;

    //return (s - 1);        //Cancel return returns char*
}

/**
 * @brief Convert the address, replace the characters in the IP address 072.023.111.011==72.23.111.11
 *
 * @param[in] char * ip_string
 *          - Pre-conversion dotted decimal IP address data
 *
 * @param[in] char * dst_string
 *          - The converted dotted decimal IP address string
 *
* FUNCTION unir_tcpip_address_replace_zero
* DESCRIPTION
*
* replace ipv4 address more zero
* eg:010.012.122.122 -->10.12.122.122
*/

int qcm_utils_address_replace_zero(char *ip_string, char *dst_string)
{
    int a, b, c, d;
    if ((qosa_sscanf(ip_string, "%d.%d.%d.%d", &a, &b, &c, &d) == 4) && a >= 0 && a <= 255 && b >= 0 && b <= 255 && c >= 0 && c <= 255 && d >= 0 && d <= 255)
    {
        qosa_sprintf(dst_string, "%d.%d.%d.%d", a, b, c, d);
        return 0;
    }
    return -1;
}
