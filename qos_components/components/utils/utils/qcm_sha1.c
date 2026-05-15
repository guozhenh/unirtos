/**
 *@file qcm_sha1.c
 * @author lawrence.liu (lawrence.liu@quectel.com)
 *  @brief
 * @version 0.1
 * @date 2023-11-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "qcm_proj_config.h"
#include "qcm_sha1.h"

#define QL_SOCKET_LOG_LEVEL QL_LOG_LEVEL_INFO

#include "qosa_system_config.h"
#if defined(CONFIG_QCM_MBEDTLS_LIBRARY_FUNC) || (defined(CONFIG_QOSA_MBEDTLS_LIBRARY_FUNC) && !defined(UNIRTOS_OPEN_MODE_SUPPORT))

#ifdef CONFIG_QCM_MBEDTLS_4_LIBRARY_FUNC
#include "mbedtls/private/sha1.h"
#else /* CONFIG_QCM_MBEDTLS_4_LIBRARY_FUNC */
#include "mbedtls/sha1.h"
#endif /* CONFIG_QCM_MBEDTLS_4_LIBRARY_FUNC */

#else /* CONFIG_QCM_MBEDTLS_LIBRARY_FUNC */
static qosa_size_t ft(int t, qosa_size_t x, qosa_size_t y, qosa_size_t z)
{
    qosa_size_t a, b, c = 0;

    if (t < 20)
    {
        a = x & y;
        b = (~x) & z;
        c = a ^ b;
    }
    else if (t < 40)
    {
        c = x ^ y ^ z;
    }
    else if (t < 60)
    {
        a = x & y;
        b = a ^ (x & z);
        c = b ^ (y & z);
    }
    else if (t < 80)
    {
        c = (x ^ y) ^ z;
    }

    return c;
}

static qosa_size_t k(int t)
{
    qosa_size_t c = 0;

    if (t < 20)
    {
        c = 0x5a827999;
    }
    else if (t < 40)
    {
        c = 0x6ed9eba1;
    }
    else if (t < 60)
    {
        c = 0x8f1bbcdc;
    }
    else if (t < 80)
    {
        c = 0xca62c1d6;
    }

    return c;
}

static qosa_size_t rotl(int bits, qosa_size_t a)
{
    qosa_size_t c, d, e, f, g = 0;

    if (bits <= 31)
    {
        c = (0x0001 << (32 - bits)) - 1;
        d = ~c;

        e = (a & c) << bits;
        f = (a & d) >> (32 - bits);

        g = e | f;
    }

    return (g & 0xffffffff);
}

static void qcm_sha1(qosa_uint8_t *message, int message_length, qosa_uint8_t *digest)
{
    int i;
    int num_blocks;
    int block_remainder;
    int padded_length;

    qosa_size_t l;
    qosa_size_t t;
    qosa_size_t h[5];
    qosa_size_t a, b, c, d, e;
    qosa_size_t w[80];
    qosa_size_t temp;

    /* Calculate the number of 512 bit blocks */

    padded_length = message_length + 8; /* Add length for l */
    padded_length = padded_length + 1;  /* Add the 0x01 bit postfix */

    l = message_length * 8;

    num_blocks = padded_length / 64;
    block_remainder = padded_length % 64;

    if (block_remainder > 0)
    {
        num_blocks++;
    }

    padded_length = padded_length + (64 - block_remainder);

    /* clear the padding field */
    for (i = message_length; i < (num_blocks * 64); i++)
    {
        message[i] = 0x00;
    }

    /* insert b1 padding bit */
    message[message_length] = 0x80;

    /* Insert l */
    message[(num_blocks * 64) - 1] = (qosa_uint8_t)(l & 0xff);
    message[(num_blocks * 64) - 2] = (qosa_uint8_t)((l >> 8) & 0xff);
    message[(num_blocks * 64) - 3] = (qosa_uint8_t)((l >> 16) & 0xff);
    message[(num_blocks * 64) - 4] = (qosa_uint8_t)((l >> 24) & 0xff);

    /* Set initial hash state */
    h[0] = 0x67452301;
    h[1] = 0xefcdab89;
    h[2] = 0x98badcfe;
    h[3] = 0x10325476;
    h[4] = 0xc3d2e1f0;

    for (i = 0; i < num_blocks; i++)
    {
        /* Prepare the message schedule */
        for (t = 0; t < 80; t++)
        {
            if (t < 16)
            {
                w[t] = (256 * 256 * 256) * message[(i * 64) + (t * 4)];
                w[t] += (256 * 256) * message[(i * 64) + (t * 4) + 1];
                w[t] += (256) * message[(i * 64) + (t * 4) + 2];
                w[t] += message[(i * 64) + (t * 4) + 3];
            }
            else if (t < 80)
            {
                w[t] = rotl(1, (w[t - 3] ^ w[t - 8] ^ w[t - 14] ^ w[t - 16]));
            }
        }

        /* Initialize the five working variables */
        a = h[0];
        b = h[1];
        c = h[2];
        d = h[3];
        e = h[4];

        /* iterate a-e 80 times */

        for (t = 0; t < 80; t++)
        {
            temp = (rotl(5, a) + ft(t, b, c, d)) & 0xffffffff;
            temp = (temp + e) & 0xffffffff;
            temp = (temp + k(t)) & 0xffffffff;
            temp = (temp + w[t]) & 0xffffffff;
            e = d;
            d = c;
            c = rotl(30, b);
            b = a;
            a = temp;
        }

        /* compute the ith intermediate hash value */

        h[0] = (a + h[0]) & 0xffffffff;
        h[1] = (b + h[1]) & 0xffffffff;
        h[2] = (c + h[2]) & 0xffffffff;
        h[3] = (d + h[3]) & 0xffffffff;
        h[4] = (e + h[4]) & 0xffffffff;
    }

    digest[3] = (qosa_uint8_t)(h[0] & 0xff);
    digest[2] = (qosa_uint8_t)((h[0] >> 8) & 0xff);
    digest[1] = (qosa_uint8_t)((h[0] >> 16) & 0xff);
    digest[0] = (qosa_uint8_t)((h[0] >> 24) & 0xff);

    digest[7] = (qosa_uint8_t)(h[1] & 0xff);
    digest[6] = (qosa_uint8_t)((h[1] >> 8) & 0xff);
    digest[5] = (qosa_uint8_t)((h[1] >> 16) & 0xff);
    digest[4] = (qosa_uint8_t)((h[1] >> 24) & 0xff);

    digest[11] = (qosa_uint8_t)(h[2] & 0xff);
    digest[10] = (qosa_uint8_t)((h[2] >> 8) & 0xff);
    digest[9] = (qosa_uint8_t)((h[2] >> 16) & 0xff);
    digest[8] = (qosa_uint8_t)((h[2] >> 24) & 0xff);

    digest[15] = (qosa_uint8_t)(h[3] & 0xff);
    digest[14] = (qosa_uint8_t)((h[3] >> 8) & 0xff);
    digest[13] = (qosa_uint8_t)((h[3] >> 16) & 0xff);
    digest[12] = (qosa_uint8_t)((h[3] >> 24) & 0xff);

    digest[19] = (qosa_uint8_t)(h[4] & 0xff);
    digest[18] = (qosa_uint8_t)((h[4] >> 8) & 0xff);
    digest[17] = (qosa_uint8_t)((h[4] >> 16) & 0xff);
    digest[16] = (qosa_uint8_t)((h[4] >> 24) & 0xff);
}
#endif /* CONFIG_QCM_MBEDTLS_LIBRARY_FUNC */
/******************************************************/
/* hmac-sha1()                                        */
/* Performs the hmac-sha1 keyed secure hash algorithm */
/******************************************************/
#define MAX_MESSAGE_LENGTH 2048
void qcm_hmac_sha1(qosa_uint8_t *key, int key_length, qosa_uint8_t *data, int data_length, qosa_uint8_t *digest)
{
#if defined(CONFIG_QCM_MBEDTLS_LIBRARY_FUNC) || (defined(CONFIG_QOSA_MBEDTLS_LIBRARY_FUNC) && !defined(UNIRTOS_OPEN_MODE_SUPPORT))
    mbedtls_sha1_context context;
    qosa_uint8_t         k_ipad[64];
    qosa_uint8_t         k_opad[64];
    int                  i;

    if ((QOSA_NULL == data) || (QOSA_NULL == digest) || (QOSA_NULL == key))
    {
        return;
    }

    qosa_memset(k_ipad, 0, sizeof(k_ipad));
    qosa_memset(k_opad, 0, sizeof(k_opad));
    if (key_length > 64)
    {
        mbedtls_sha1_init(&context);
        mbedtls_sha1_starts(&context);
        mbedtls_sha1_update(&context, (qosa_uint8_t *)key, key_length);
        mbedtls_sha1_finish(&context, digest);

        for (i = 0; i < 20; i++)
        {
            k_ipad[i] = digest[i];
            k_opad[i] = digest[i];
        }
    }
    else
    {
        qosa_memcpy(k_ipad, key, key_length);
        qosa_memcpy(k_opad, key, key_length);
    }

    for (i = 0; i < 64; i++)
    {
        k_ipad[i] ^= 0x36;
        k_opad[i] ^= 0x5c;
    }
    mbedtls_sha1_init(&context);
    mbedtls_sha1_starts(&context);
    mbedtls_sha1_update(&context, k_ipad, 64);
    mbedtls_sha1_update(&context, (qosa_uint8_t *)data, data_length);
    mbedtls_sha1_finish(&context, digest);

    mbedtls_sha1_init(&context);
    mbedtls_sha1_starts(&context);
    mbedtls_sha1_update(&context, k_opad, 64);
    mbedtls_sha1_update(&context, digest, 20);
    mbedtls_sha1_finish(&context, digest);
#else
    int          b = 64; /* blocksize */
    qosa_uint8_t ipad = 0x36;

    qosa_uint8_t opad = 0x5c;

    qosa_uint8_t k0[64];
    qosa_uint8_t k0xorIpad[64];
    qosa_uint8_t step7data[64];
    qosa_uint8_t step5data[MAX_MESSAGE_LENGTH + 128];
    qosa_uint8_t step8data[64 + 20];
    int          i;

    for (i = 0; i < 64; i++)
    {
        k0[i] = 0x00;
    }

    if (key_length != b) /* Step 1 */
    {
        /* Step 2 */
        if (key_length > b)
        {
            qcm_sha1(key, key_length, digest);
            for (i = 0; i < 20; i++)
            {
                k0[i] = digest[i];
            }
        }
        else if (key_length < b) /* Step 3 */
        {
            for (i = 0; i < key_length; i++)
            {
                k0[i] = key[i];
            }
        }
    }
    else
    {
        for (i = 0; i < b; i++)
        {
            k0[i] = key[i];
        }
    }
    /* Step 4 */
    for (i = 0; i < 64; i++)
    {
        k0xorIpad[i] = k0[i] ^ ipad;
    }
    /* Step 5 */
    for (i = 0; i < 64; i++)
    {
        step5data[i] = k0xorIpad[i];
    }
    for (i = 0; i < data_length; i++)
    {
        step5data[i + 64] = data[i];
    }

    /* Step 6 */
    qcm_sha1(step5data, data_length + b, digest);

    /* Step 7 */
    for (i = 0; i < 64; i++)
    {
        step7data[i] = k0[i] ^ opad;
    }

    /* Step 8 */
    for (i = 0; i < 64; i++)
    {
        step8data[i] = step7data[i];
    }
    for (i = 0; i < 20; i++)
    {
        step8data[i + 64] = digest[i];
    }

    /* Step 9 */
    qcm_sha1(step8data, b + 20, digest);
#endif
}
