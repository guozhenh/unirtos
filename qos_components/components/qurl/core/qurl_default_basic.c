/** @file         qurl_default_basic.c
 *  @brief        Brief description: Some basic functions provided internally, which can be rewritten.
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-11-17 10:40:31
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#include "qurl_config.h"
#include "qurl_platform_port.h"
#include "qurl_log.h"
#include "qurl_ctype.h"

int qurl_default_vasprintf(char **str, const char *format, va_list args)
{
    va_list args2;
    va_copy(args2, args);
    //Get the length of the formatted string
    int size = qurl_vsnprintf(QOSA_NULL, 0, format, args);
    if (size < 0)
    {
        va_end(args2);
        return -1;  //Formatting failed
    }

    //Allocate memory
    *str = (char *)qurl_malloc(size + 1);
    if (*str == QOSA_NULL)
    {
        va_end(args2);
        return -1;  //Memory allocation failed
    }

    qurl_vsnprintf(*str, size + 1, format, args2);  //Write the formatted string into the allocated memory
    va_end(args2);

    return size;                                    //Returns the length of the generated string.
}

#ifndef QURL_CFG_PORT_ASPRINTF_FUNC
int qurl_default_asprintf(char **str, const char *format, ...)
{
    int ret = 0;

    va_list args;
    va_start(args, format);
    ret = qurl_default_vasprintf(str, format, args);
    va_end(args);

    return ret;

#if 0
	va_list args;
	va_start(args, format);

	//Get the length of the formatted string
	int size = qosa_vsnprintf(QOSA_NULL, 0, format, args);
	va_end(args);

	if (size < 0)
	{
		return -1; //Formatting failed
	}

	//Allocate memory
	*str = (char *)qurl_malloc(size + 1);
	if (*str == QOSA_NULL)
	{
		return -1; //Memory allocation failed
	}

	va_start(args, format);
	qosa_vsnprintf(*str, size + 1, format, args); //Write the formatted string into the allocated memory
	va_end(args);

	return size; //Returns the length of the generated string.
#endif
}
#endif

#ifndef QURL_CFG_PORT_ASPRINTF_FUNC
int qurl_default_strtoofft(const char *str, char **endp, int base, long *num)
{
    char *end;
    long  number;
    //errno = 0;
    *num = 0; /* clear by default */

    while (*str && qurl_ctype_isblank(*str))
    {
        str++;
    }
    if (('-' == *str) || (qurl_ctype_isspace(*str)))
    {
        if (endp)
        {
            *endp = (char *)str;         /* didn't actually move */
        }
        return QURL_ECODE_PARAM_INVALID; /* nothing parsed */
    }
    number = qurl_strtol(str, &end, base);
    if (endp)
    {
        *endp = end;
    }
/** The definition of ERANGE varies across different platforms, resulting in inconsistent return values with ERANGE */
#if !defined(CONFIG_QOSA_ASR1903_PLATFORM_FUNC) && !defined(CONFIG_QOSA_ASR160X_PLATFORM_FUNC) && !defined(CONFIG_QOSA_ASR1805_PLATFORM_FUNC)
    if (qurl_get_errno() == ERANGE)
    {
        /* overflow/underflow */
        return QURL_ECODE_OVER_LIMIT;
    }
    else
#endif /* CONFIG_QOSA_ASR1903_PLATFORM_FUNC || CONFIG_QOSA_ASR160X_PLATFORM_FUNC || CONFIG_QOSA_ASR1805_PLATFORM_FUNC */
        if (str == end)
        {
            /* nothing parsed */
            return QURL_ECODE_PARAM_INVALID;
        }

    *num = number;
    return QURL_OK;
}
#endif

#if (!defined QURL_CFG_PORT_TLS_BASE64_ENCODE_FUNC || !defined QURL_CFG_PORT_TLS_BASE64_DECODE_FUNC)
/* ---- Base64 Encoding/Decoding Table --- */
/* Padding character string starts at offset 64. */
static const char          base64encdec[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
static const unsigned char decodetable[]
    = {62, 255, 255, 255, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 255, 255, 255, 255, 255, 255, 255, 0,   1,   2,   3,   4,
       5,  6,   7,   8,   9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,  21,  22,  23,  24,  25,  255, 255, 255, 255, 255, 255,
       26, 27,  28,  29,  30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51};
#endif

/**
 * @brief Base64 encoding.
 * Description: 8-bit characters are represented using 6 bits, i.e., a 3:4 ratio,
 * 		We take bytes as the unit, with the minimum granularity set at 3 bytes, rounding up to the nearest larger value,
 * 		If taking 6 bytes from 4 bytes, 8 bytes are needed to ensure 4-byte base64 encoding; of course, for 5 or 6 bytes, 8 bytes are also required to guarantee the output.
 * 		As above, the formula is as follows: out_len = 4 * ((in_len + 2) / 3)
 *
 * @param src_data
 * @param src_len
 * @param dst_buf
 * @param dst_len
 * @return qurl_ecode_t
 */
#ifndef QURL_CFG_PORT_TLS_BASE64_ENCODE_FUNC
qurl_ecode_t qurl_default_base64_encode(const unsigned char *src_data, long src_len, unsigned char *dst_buf, long dst_buf_len, long *dst_len)
{
    char                *output = (char *)dst_buf;
    const char          *table64 = base64encdec;
    const unsigned char *in = (unsigned char *)src_data;
    const char          *padstr = &table64[64]; /*Point to padding string: such as '=' in base64encdec*/

    if (src_data == QOSA_NULL || src_len <= 0 || dst_buf == QOSA_NULL || dst_len == QOSA_NULL)
    {
        if(dst_len != QOSA_NULL)
        {
            *dst_len = 0;
        }
        return QURL_ECODE_PARAM_INVALID;
    }
    *dst_len = 0;

    if (dst_buf_len < 4 * ((src_len + 2) / 3))
    {
        return QURL_ECODE_BUF_TOO_SMALL;
    }

    while (src_len >= 3)
    {
        *output++ = table64[in[0] >> 2];
        *output++ = table64[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        *output++ = table64[((in[1] & 0x0F) << 2) | ((in[2] & 0xC0) >> 6)];
        *output++ = table64[in[2] & 0x3F];
        src_len -= 3;
        in += 3;
    }
    if (src_len)
    {
        /* this is only one or two bytes now */
        *output++ = table64[in[0] >> 2];
        if (src_len == 1)
        {
            *output++ = table64[((in[0] & 0x03) << 4)];
            if (*padstr)
            {
                *output++ = *padstr;
                *output++ = *padstr;
            }
        }
        else
        {
            /* src_len == 2 */
            *output++ = table64[((in[0] & 0x03) << 4) | ((in[1] & 0xF0) >> 4)];
            *output++ = table64[((in[1] & 0x0F) << 2)];
            if (*padstr)
                *output++ = *padstr;
        }
    }

    *dst_len = output - (char *)dst_buf;

    return QURL_OK;
}
#endif

#ifndef QURL_CFG_PORT_TLS_BASE64_DECODE_FUNC
qurl_ecode_t qurl_default_base64_decode(const unsigned char *src_data, long src_len, unsigned char *dst_buf, long dst_buf_len, long *dst_len)
{
    int            i = 0;
    long           padding = 0;
    long           rawlen = 0;
    long           num_quantums = 0;
    long           full_quantums = 0;
    unsigned char *pos = dst_buf;
    unsigned char  lookup[256] = {0xff};

    if (src_data == QOSA_NULL || src_len <= 0 || (src_len % 4) || dst_buf == QOSA_NULL || dst_len == QOSA_NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    if ((src_len / 4) * 3 > dst_buf_len)
    {
        return QURL_ECODE_BUF_TOO_SMALL;
    }

    /*The src array must contain at least 4 elements.*/
    while (src_data[src_len - 1 - padding] == '=')
    {
        /*Count the number of padding characters*/
        padding++;
        /*A maximum of two padding characters is allowed.*/
        if (padding > 2)
        {
            return QURL_ECODE_TLS_BAD_CONTENT_ENCODING;
        }
    }

    /*Calculate the length of the output result*/
    num_quantums = src_len / 4;
    full_quantums = num_quantums - (padding ? 1 : 0);
    rawlen = (num_quantums * 3) - padding;

    qurl_memcpy(&lookup['+'], decodetable, sizeof(decodetable));

    for (i = 0; i < full_quantums; i++)
    {
        unsigned char val = 0;
        unsigned int  x = 0;
        int           j = 0;

        for (j = 0; j < 4; j++)
        {
            val = lookup[(unsigned char)*src_data++];
            if (val == 0xff) /* bad symbol */
            {
                return QURL_ECODE_TLS_BAD_CONTENT_ENCODING;
            }
            x = (x << 6) | val;
        }
        pos[2] = x & 0xff;
        pos[1] = (x >> 8) & 0xff;
        pos[0] = (x >> 16) & 0xff;
        pos += 3;
    }
    if (padding)
    {
        /*Rounded 1-byte or 2-byte*/
        unsigned char val = 0;
        unsigned int  x = 0;
        int           j = 0;
        long          padc = 0;
        for (j = 0; j < 4; j++)
        {
            if (*src_data == '=')
            {
                x <<= 6;
                src_data++;
                if (++padc > padding)
                /* this is a badly placed '=' symbol! */
                {
                    return QURL_ECODE_TLS_BAD_CONTENT_ENCODING;
                }
            }
            else
            {
                val = lookup[(unsigned char)*src_data++];
                if (val == 0xff) /* bad symbol */
                {
                    return QURL_ECODE_TLS_BAD_CONTENT_ENCODING;
                }
                x = (x << 6) | val;
            }
        }
        if (padding == 1)
            pos[1] = (x >> 8) & 0xff;
        pos[0] = (x >> 16) & 0xff;
        pos += 3 - padding;
    }

    *dst_len = rawlen;

    return QURL_OK;
}
#endif
