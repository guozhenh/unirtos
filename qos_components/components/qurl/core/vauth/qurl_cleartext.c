

#include "qurl_config.h"

#if QURL_CFG_ENABLE_SMTP
#include "qurl_log.h"
#include "qurl_vauth.h"
#include "qurl_default_basic.h"

/**
 * @brief Create SASL PLAIN authentication message
 * 
 * This function is used to construct the authentication message for the SASL PLAIN mechanism, following the RFC 4616 standard format:
 * authzid NUL authcid NUL passwd
 * and encode the constructed message in Base64
 * 
 * @param[in]  authzid  Authorization identity identifier (can be NULL)
 * @param[in]  authcid  Authentication identity identifier (username)
 * @param[in]  passwd   Password
 * @param[out] outptr   Pointer to the encoded message (caller must free memory!!!)
 * @param[out] outlen   Length of the encoded message
 * 
 * @return qurl_ecode_t error code
 * @retval QURL_ECODE_SUCCESS    Success
 * @retval QURL_ECODE_NO_MEMORY  Insufficient memory
 * 
 * @note The caller is responsible for freeing the memory pointed to by *outptr
 */
qurl_ecode_t qurl_auth_create_plain_message(const char *authzid, const char *authcid, const char *passwd, char **outptr, int *outlen)
{
    qurl_ecode_t   result;
    unsigned char *plainauth;
    int            zlen;
    int            clen;
    int            plen;
    int            plainlen;
    int            dst_len = 0;

    *outlen = 0;
    *outptr = QOSA_NULL;
    zlen = (authzid == QOSA_NULL ? 0 : qurl_strlen(authzid));
    clen = qurl_strlen(authcid);
    plen = qurl_strlen(passwd);

    plainlen = zlen + clen + plen + 2;

    plainauth = qurl_malloc(plainlen);
    if (!plainauth)
    {
        return QURL_ECODE_NO_MEMORY;
    }

    /* Calculate the reply */
    if (zlen != 0)
    {
        qurl_memcpy(plainauth, authzid, zlen);
    }

    plainauth[zlen] = '\0';
    qurl_memcpy(plainauth + zlen + 1, authcid, clen);
    plainauth[zlen + clen + 1] = '\0';
    qurl_memcpy(plainauth + zlen + clen + 2, passwd, plen);

    /* Base64 encode the reply */
    dst_len = plainlen * 4 / 3 + 4;
    //After external use -- note that it needs to be released
    outptr = qurl_malloc(dst_len);
    result = qurl_default_base64_encode(plainauth, plainlen, (unsigned char *)outptr, dst_len, (long *)outlen);
    qurl_free(plainauth);

    return result;
}

qurl_ecode_t qurl_auth_create_login_message(const char *valuep, char **outptr, int *outlen)
{
    qurl_logv("enter =%s", valuep);
    qurl_ecode_t ret = QURL_OK;
    char        *base64_str = QOSA_NULL;
    int          vlen = qurl_strlen(valuep);
    int          dst_len = 0;

    qurl_logv("enter =%d", vlen);
    if (!vlen)
    {
        /* Calculate an empty reply */
        *outptr = qurl_strdup("=");
        if (*outptr)
        {
            *outlen = 1;
            return QURL_OK;
        }

        *outlen = 0;
        return QURL_ECODE_NO_MEMORY;
    }

    /* Base64 encode the value */
    dst_len = vlen * 4 / 3 + 4;
    base64_str = qurl_malloc(dst_len);
    if (base64_str == QOSA_NULL)
    {
        return QURL_ECODE_NO_MEMORY;
    }
    qurl_memset(base64_str, 0, dst_len);
    ret = qurl_default_base64_encode((unsigned char *)valuep, vlen, (unsigned char *)base64_str, dst_len, (long *)outlen);
    *outptr = base64_str;
    qurl_logv("ret =%d,%s", ret, outptr);
    return ret;
}

qurl_ecode_t qurl_auth_create_external_message(const char *user, char **outptr, int *outlen)
{
    /* This is the same formatting as the login message */
    qurl_logv("enter");
    return qurl_auth_create_login_message(user, outptr, outlen);
}

#endif /* if no users */
