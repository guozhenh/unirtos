
#include "qurl_config.h"

#if QURL_CFG_ENABLE_SMTP

#include "qurl_vauth.h"
#include "qurl_default_basic.h"

qurl_ecode_t qurl_auth_create_oauth_bearer_message(const char *user, const char *host, const long port, const char *bearer, char **outptr, int *outlen)
{
    qurl_ecode_t result = QURL_OK;
    char        *oauth = QOSA_NULL;
    int          dst_len = 0;

    /* Generate the message */
    if (port == 0 || port == 80)
    {
        qurl_asprintf(&oauth, "n,a=%s,\1host=%s\1auth=Bearer %s\1\1", user, host, bearer);
    }
    else
    {
        qurl_asprintf(&oauth, "n,a=%s,\1host=%s\1port=%ld\1auth=Bearer %s\1\1", user, host, port, bearer);
    }

    if (!oauth)
    {
        return QURL_ECODE_NO_MEMORY;
    }

    /* Base64 encode the reply */
    dst_len = qurl_strlen(oauth) * 4 / 3 + 4;
    outptr = qurl_malloc(dst_len);
    result = qurl_default_base64_encode((unsigned char *)oauth, qurl_strlen(oauth), (unsigned char *)outptr, dst_len, (long *)outlen);

    qurl_free(oauth);

    return result;
}

qurl_ecode_t qurl_auth_create_xoauth_bearer_message(const char *user, const char *bearer, char **outptr, int *outlen)
{
    qurl_ecode_t result = QURL_OK;
    int          dst_len = 0;
    char        *xoauth = QOSA_NULL;

    /* Generate the message */
    qurl_asprintf(&xoauth, "user=%s\1auth=Bearer %s\1\1", user, bearer);
    if (!xoauth)
    {
        return QURL_ECODE_NO_MEMORY;
    }

    /* Base64 encode the reply */
    dst_len = qurl_strlen(xoauth) * 4 / 3 + 4;
    outptr = qurl_malloc(dst_len);
    result = qurl_default_base64_encode((unsigned char *)xoauth, qurl_strlen(xoauth), (unsigned char *)outptr, dst_len, (long *)outlen);
    if (result != QURL_OK)
    {
        qosa_free(outptr);
    }

    qurl_free(xoauth);

    return result;
}
#endif
