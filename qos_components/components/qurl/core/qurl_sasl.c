/*
 * Portions of this file are derived from curl/libcurl's lib/curl_sasl.c.
 *
 * Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * should be included in the curl distribution. The terms are also
 * available at https://curl.se/docs/copyright.html.
 *
 * For qurl redistribution, see:
 * - qos_components/components/qurl/THIRD_PARTY_NOTICES.md
 * - qos_components/components/qurl/licenses/curl.txt
 */
#include "qurl_config.h"

#if QURL_CFG_ENABLE_SMTP
#include "qurl_sasl.h"
#include "qurl_ctype.h"
#include "qurl_log.h"
#include "qurl_def_in.h"

extern qurl_ecode_t qurl_auth_create_oauth_bearer_message(const char *user, const char *host, const long port, const char *bearer, char **outptr, int *outlen);
extern qurl_ecode_t qurl_auth_create_xoauth_bearer_message(const char *user, const char *bearer, char **outptr, int *outlen);
extern qurl_ecode_t qurl_auth_create_external_message(const char *user, char **outptr, int *outlen);
extern qurl_ecode_t qurl_auth_create_plain_message(const char *authzid, const char *authcid, const char *passwd, char **outptr, int *outlen);
extern qurl_ecode_t qurl_auth_create_login_message(const char *valuep, char **outptr, int *outlen);

/* Supported mechanisms */
static const struct
{
    const char  *name; /* Name */
    int          len;  /* Name length */
    unsigned int bit;  /* Flag bit */
} mechtable[]
    = {{"LOGIN", 5, SASL_MECH_LOGIN},
       {"PLAIN", 5, SASL_MECH_PLAIN},
       {"CRAM-MD5", 8, SASL_MECH_CRAM_MD5},
       {"DIGEST-MD5", 10, SASL_MECH_DIGEST_MD5},
       {"GSSAPI", 6, SASL_MECH_GSSAPI},
       {"EXTERNAL", 8, SASL_MECH_EXTERNAL},
       {"NTLM", 4, SASL_MECH_NTLM},
       {"XOAUTH2", 7, SASL_MECH_XOAUTH2},
       {"OAUTHBEARER", 11, SASL_MECH_OAUTHBEARER},
       {QOSA_NULL, 0, 0}};

unsigned int qurl_sasl_decode_mech(const char *ptr, int maxlen, int *len)
{
    unsigned int i;
    char         c;

    for (i = 0; mechtable[i].name; i++)
    {
        if (maxlen >= mechtable[i].len && !qurl_memcmp(ptr, mechtable[i].name, mechtable[i].len))
        {
            if (len)
            {
                *len = mechtable[i].len;
            }

            if (maxlen == mechtable[i].len)
            {
                return mechtable[i].bit;
            }

            c = ptr[mechtable[i].len];
            if (!qurl_ctype_isupper(c) && !qurl_ctype_isdigit(c) && c != '-' && c != '_')
            {
                return mechtable[i].bit;
            }
        }
    }

    return 0;
}

/*
 * Curl_sasl_parse_url_auth_option()
 *
 * Parse the URL login options.
 */
qurl_ecode_t qurl_sasl_parse_url_auth_option(qurl_sasl_t *sasl, const char *value, int len)
{
    qurl_ecode_t result = QURL_OK;
    int          mechlen;

    if (!len)
    {
        return QURL_ECODE_SMTP_URL_MALFORMAT;
    }

    if (sasl->resetprefs)
    {
        sasl->resetprefs = QOSA_FALSE;
        sasl->prefmech = SASL_AUTH_NONE;
    }

    if (!qurl_strncmp(value, "*", len))
    {
        sasl->prefmech = SASL_AUTH_DEFAULT;
    }
    else
    {
        unsigned int mechbit = qurl_sasl_decode_mech(value, len, &mechlen);
        if (mechbit && mechlen == len)
        {
            sasl->prefmech |= mechbit;
        }
        else
        {
            result = QURL_ECODE_SMTP_URL_MALFORMAT;
        }
    }

    return result;
}

/*
 * Curl_sasl_init()
 *
 * Initializes the SASL structure.
 */
void qurl_sasl_init(qurl_sasl_t *sasl, const qurl_saslproto_t *params)
{
    sasl->params = params;              /* Set protocol dependent parameters */
    sasl->state = SASL_STOP;            /* Not yet running */
    sasl->authmechs = SASL_AUTH_NONE;   /* No known authentication mechanism yet */
    sasl->prefmech = SASL_AUTH_DEFAULT; /* Prefer all mechanisms */
    sasl->authused = SASL_AUTH_NONE;    /* No the authentication mechanism used */
    sasl->resetprefs = QOSA_TRUE;       /* Reset prefmech upon AUTH parsing. */
    sasl->mutual_auth = QOSA_FALSE;     /* No mutual authentication (GSSAPI only) */
    sasl->force_ir = QOSA_FALSE;        /* Respect external option */
}

/*
 * state()
 *
 * This is the ONLY way to change SASL state!
 */
static void state(qurl_sasl_t *sasl, saslstate newstate)
{
    static const char *const names[] = {
        "STOP",
        "PLAIN",
        "LOGIN",
        "LOGIN_PASSWD",
        "EXTERNAL",
        "CRAMMD5",
        "DIGESTMD5",
        "DIGESTMD5_RESP",
        "NTLM",
        "NTLM_TYPE2MSG",
        "GSSAPI",
        "GSSAPI_TOKEN",
        "GSSAPI_NO_DATA",
        "OAUTH2",
        "OAUTH2_RESP",
        "CANCEL",
        "FINAL",
        /* LAST */
    };

    if (sasl->state != newstate)
    {
        QURL_UNUSED(names);
        qurl_loge("SASL %p state change from %s to %s\n", (void *)sasl, names[sasl->state], names[newstate]);
    }

    sasl->state = newstate;
}

/*
 * Curl_sasl_can_authenticate()
 *
 * Check if we have enough auth data and capabilities to authenticate.
 */
qosa_bool_t qurl_sasl_can_authenticate(qurl_sasl_t *sasl, void *conn_ptr)
{
    qurl_conn_t *conn = (qurl_conn_t *)conn_ptr;
    /* Have credentials been provided? */
    if (conn->bits.user_passwd)
    {
        return QOSA_TRUE;
    }

    /* EXTERNAL can authenticate without a user name and/or password */
    if (sasl->authmechs & sasl->prefmech & SASL_MECH_EXTERNAL)
    {
        return QOSA_TRUE;
    }

    return QOSA_FALSE;
}

/*
 * Curl_sasl_start()
 *
 * Calculate the required login details for SASL authentication.
 */

qurl_ecode_t qurl_sasl_start(qurl_sasl_t *sasl, void *conn_ptr, qosa_bool_t force_ir, saslprogress *progress)
{
    qurl_ecode_t result = QURL_OK;
    unsigned int enabledmechs;
    const char  *mech = QOSA_NULL;
    char        *resp = QOSA_NULL;
    int          len = 0;
    saslstate    state1 = SASL_STOP;
    saslstate    state2 = SASL_FINAL;

    qurl_conn_t      *conn = (qurl_conn_t *)conn_ptr;
    const char *const hostname = conn->host_name;
    const long int    port = conn->conn_conn[QURL_SOCKET_INDEX_0].remote_port;
    const char       *oauth_bearer = conn->core_ptr->usr_cfg.smtp.mail_xoauth2_ptr;

    sasl->force_ir = force_ir; /* Latch for future use */
    sasl->authused = 0;        /* No mechanism used yet */
    enabledmechs = sasl->authmechs & sasl->prefmech;
    *progress = SASL_IDLE;

    /* Calculate the supported authentication mechanism, by decreasing order of
     security, as well as the initial response where appropriate */

    qurl_logd("enter enabledmechs=%d,%d,%x", enabledmechs, conn->bits.user_passwd, oauth_bearer);
    if ((enabledmechs & SASL_MECH_EXTERNAL) && !conn->passwd_ptr[0])
    {
        mech = SASL_MECH_STRING_EXTERNAL;
        state1 = SASL_EXTERNAL;
        sasl->authused = SASL_MECH_EXTERNAL;

        if (force_ir)
        {
            result = qurl_auth_create_external_message(conn->username_ptr, &resp, &len);
        }
    }
    else if (conn->bits.user_passwd)
    {
        if ((enabledmechs & SASL_MECH_OAUTHBEARER) && oauth_bearer != QOSA_NULL)
        {
            mech = SASL_MECH_STRING_OAUTHBEARER;
            state1 = SASL_OAUTH2;
            state2 = SASL_OAUTH2_RESP;
            sasl->authused = SASL_MECH_OAUTHBEARER;

            if (force_ir)
            {
                result = qurl_auth_create_oauth_bearer_message(conn->username_ptr, hostname, port, oauth_bearer, &resp, &len);
            }
        }
        else if ((enabledmechs & SASL_MECH_XOAUTH2) && oauth_bearer != QOSA_NULL)
        {
            mech = SASL_MECH_STRING_XOAUTH2;
            state1 = SASL_OAUTH2;
            sasl->authused = SASL_MECH_XOAUTH2;

            if (force_ir || conn->core_ptr->usr_cfg.smtp.sasl_ir)
            {
                result = qurl_auth_create_xoauth_bearer_message(conn->username_ptr, oauth_bearer, &resp, &len);
            }
        }
        else if (enabledmechs & SASL_MECH_LOGIN)
        {
            //Curl_debug(data, CURLINFO_TEXT, "AUTH LOGIN", qosa_strlen("AUTH LOGIN"));
            mech = SASL_MECH_STRING_LOGIN;
            state1 = SASL_LOGIN;
            state2 = SASL_LOGIN_PASSWD;
            sasl->authused = SASL_MECH_LOGIN;

            if (force_ir || conn->core_ptr->usr_cfg.smtp.sasl_ir)
            {
                result = qurl_auth_create_login_message(conn->username_ptr, &resp, &len);
            }
        }
        else if (enabledmechs & SASL_MECH_PLAIN)
        {
            mech = SASL_MECH_STRING_PLAIN;
            state1 = SASL_PLAIN;
            sasl->authused = SASL_MECH_PLAIN;

            if (force_ir || conn->core_ptr->usr_cfg.smtp.sasl_ir)
            {
                result = qurl_auth_create_plain_message(conn->core_ptr->usr_cfg.smtp.sasl_authzid_ptr, conn->username_ptr, conn->passwd_ptr, &resp, &len);
            }
        }
    }

    if (!result && mech)
    {
        if (resp && (sasl->params->maxirlen > 0) && ((qurl_strlen(mech) + len) > sasl->params->maxirlen))
        {
            qurl_free(resp);
            resp = QOSA_NULL;
        }

        result = sasl->params->sendauth(conn, mech, resp);
        if (!result)
        {
            *progress = SASL_INPROGRESS;
            state(sasl, resp ? state2 : state1);
        }
    }

    qurl_free(resp);
    return result;
}

/*
 * Curl_sasl_continue()
 *
 * Continue the authentication.
 */
qurl_ecode_t qurl_sasl_continue(qurl_sasl_t *sasl, void *conn_ptr, int code, saslprogress *progress)
{
    qurl_ecode_t result = QURL_OK;
    saslstate    newstate = SASL_FINAL;
    char        *resp = QOSA_NULL;

    qurl_conn_t      *conn = (qurl_conn_t *)conn_ptr;
    const char *const hostname = conn->host_name;
    const long int    port = conn->conn_conn[QURL_SOCKET_INDEX_0].remote_port;

    int         len = 0;
    const char *oauth_bearer = conn->core_ptr->usr_cfg.smtp.mail_xoauth2_ptr;

    *progress = SASL_INPROGRESS;

    qurl_logd("sasl->state=%d,%d,%d", sasl->state, sasl->params->contcode, sasl->params->finalcode);
    if (sasl->state == SASL_FINAL)
    {
        if (code != sasl->params->finalcode)
        {
            result = QURL_ECODE_SMTP_AUTH_FAILED;
        }

        *progress = SASL_DONE;
        state(sasl, SASL_STOP);
        return result;
    }

    if (sasl->state != SASL_CANCEL && sasl->state != SASL_OAUTH2_RESP && code != sasl->params->contcode)
    {
        *progress = SASL_DONE;
        state(sasl, SASL_STOP);
        return QURL_ECODE_SMTP_AUTH_FAILED;
    }

    qurl_logd("sasl->state=%d", sasl->state);
    switch (sasl->state)
    {
        case SASL_STOP:
            *progress = SASL_DONE;
            return result;
        case SASL_PLAIN:
            result = qurl_auth_create_plain_message(
                conn->core_ptr->usr_cfg.smtp.sasl_authzid_ptr,
                conn->core_ptr->usr_cfg.username_ptr,
                conn->core_ptr->usr_cfg.password_ptr,
                &resp,
                &len
            );
            break;
        case SASL_LOGIN:
            qurl_logd("SASL_LOGIN =%s", conn->core_ptr->usr_cfg.username_ptr);
            result = qurl_auth_create_login_message(conn->core_ptr->usr_cfg.username_ptr, &resp, &len);
            newstate = SASL_LOGIN_PASSWD;
            break;
        case SASL_LOGIN_PASSWD:
            result = qurl_auth_create_login_message(conn->core_ptr->usr_cfg.password_ptr, &resp, &len);
            break;
        case SASL_EXTERNAL:
            result = qurl_auth_create_external_message(conn->core_ptr->usr_cfg.username_ptr, &resp, &len);
            break;
        case SASL_OAUTH2:
            /* Create the authorisation message */
            if (sasl->authused == SASL_MECH_OAUTHBEARER)
            {
                result = qurl_auth_create_oauth_bearer_message(conn->core_ptr->usr_cfg.username_ptr, hostname, port, oauth_bearer, &resp, &len);

                /* Failures maybe sent by the server as continuations for OAUTHBEARER */
                newstate = SASL_OAUTH2_RESP;
            }
            else
            {
                result = qurl_auth_create_xoauth_bearer_message(conn->core_ptr->usr_cfg.username_ptr, oauth_bearer, &resp, &len);
            }

            break;
        case SASL_OAUTH2_RESP:
            /* The continuation is optional so check the response code */
            if (code == sasl->params->finalcode)
            {
                /* Final response was received so we are done */
                *progress = SASL_DONE;
                state(sasl, SASL_STOP);
                return result;
            }
            else if (code == sasl->params->contcode)
            {
                /* Acknowledge the continuation by sending a 0x01 response base64 encoded */
                resp = qurl_strdup("AQ==");
                if (!resp)
                {
                    result = QURL_ECODE_NO_MEMORY;
                }
                break;
            }
            else
            {
                *progress = SASL_DONE;
                state(sasl, SASL_STOP);
                return QURL_ECODE_SMTP_AUTH_FAILED;
            }
            //break;
        case SASL_CANCEL:
            /* Remove the offending mechanism from the supported list */
            sasl->authmechs ^= sasl->authused;

            /* Start an alternative SASL authentication */
            result = qurl_sasl_start(sasl, conn, sasl->force_ir, progress);
            newstate = sasl->state; /* Use state from Curl_sasl_start() */
            break;
        default:
            qurl_loge("Unsupported SASL authentication mechanism");
            result = QURL_ECODE_SMTP_UNSPPORTED_PROTOCOL; /* Should not happen */
            break;
    }

    qurl_logd("result=%d", result);
    switch (result)
    {
        case QURL_ECODE_PARAM_INVALID:
        case QURL_ECODE_BUF_TOO_SMALL:
            /* Cancel dialog */
            qurl_logd("QURL_ECODE_BUF_TOO_SMALL");
            result = sasl->params->sendcont(conn, "*");
            newstate = SASL_CANCEL;
            break;
        case QURL_OK:
            if (resp)
            {
                qurl_logd("sendcont=%s", resp);
                result = sasl->params->sendcont(conn, resp);
            }
            break;
        default:
            qurl_logd("default");
            newstate = SASL_STOP; /* Stop on error */
            *progress = SASL_DONE;
            break;
    }

    if (resp)
    {
        qurl_free(resp);
    }

    state(sasl, newstate);

    return result;
}
#endif /* protocols are enabled that use SASL */
