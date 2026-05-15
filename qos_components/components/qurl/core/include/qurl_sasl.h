/*
 * Portions of this file are derived from curl/libcurl's lib/curl_sasl.h.
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
#ifndef HEADER_CURL_SASL_H
#define HEADER_CURL_SASL_H

#include "qurl_config.h"
//#include "qurl_def_in.h"

/* Authentication mechanism flags */
#define SASL_MECH_LOGIN              (1 << 0)
#define SASL_MECH_PLAIN              (1 << 1)
#define SASL_MECH_CRAM_MD5           (1 << 2)
#define SASL_MECH_DIGEST_MD5         (1 << 3)
#define SASL_MECH_GSSAPI             (1 << 4)
#define SASL_MECH_EXTERNAL           (1 << 5)
#define SASL_MECH_NTLM               (1 << 6)
#define SASL_MECH_XOAUTH2            (1 << 7)
#define SASL_MECH_OAUTHBEARER        (1 << 8)

/* Authentication mechanism values */
#define SASL_AUTH_NONE               0
#define SASL_AUTH_ANY                ~0U
#define SASL_AUTH_DEFAULT            (SASL_AUTH_ANY & ~SASL_MECH_EXTERNAL)

/* Authentication mechanism strings */
#define SASL_MECH_STRING_LOGIN       "LOGIN"
#define SASL_MECH_STRING_PLAIN       "PLAIN"
#define SASL_MECH_STRING_CRAM_MD5    "CRAM-MD5"
#define SASL_MECH_STRING_DIGEST_MD5  "DIGEST-MD5"
#define SASL_MECH_STRING_GSSAPI      "GSSAPI"
#define SASL_MECH_STRING_EXTERNAL    "EXTERNAL"
#define SASL_MECH_STRING_NTLM        "NTLM"
#define SASL_MECH_STRING_XOAUTH2     "XOAUTH2"
#define SASL_MECH_STRING_OAUTHBEARER "OAUTHBEARER"

/* SASL machine states */
typedef enum
{
    SASL_STOP,
    SASL_PLAIN,
    SASL_LOGIN,
    SASL_LOGIN_PASSWD,
    SASL_EXTERNAL,
    SASL_CRAMMD5,
    SASL_DIGESTMD5,
    SASL_DIGESTMD5_RESP,
    SASL_NTLM,
    SASL_NTLM_TYPE2MSG,
    SASL_GSSAPI,
    SASL_GSSAPI_TOKEN,
    SASL_GSSAPI_NO_DATA,
    SASL_OAUTH2,
    SASL_OAUTH2_RESP,
    SASL_CANCEL,
    SASL_FINAL
} saslstate;

/* Progress indicator */
typedef enum
{
    SASL_IDLE,
    SASL_INPROGRESS,
    SASL_DONE
} saslprogress;

#if 0
/* Protocol dependent SASL parameters */
struct qurl_saslproto_s
{
    const char  *service;                                                          /* The service name */
    int          contcode;                                                         /* Code to receive when continuation is expected */
    int          finalcode;                                                        /* Code to receive upon authentication success */
    unsigned int maxirlen;                                                         /* Maximum initial response length */
    qurl_ecode_t (*sendauth)(qurl_conn_t *conn, const char *mech, const char *ir); /* Send authentication command */
    qurl_ecode_t (*sendcont)(qurl_conn_t *conn, const char *contauth);             /* Send authentication continuation */
    void (*getmessage)(char *buffer, char **outptr);                               /* Get SASL response message */
};
typedef struct qurl_saslproto_s qurl_saslproto_t;

/* Per-connection parameters */

struct qurl_sasl_s
{
    const qurl_saslproto_t *params;      /* Protocol dependent parameters */
    saslstate               state;       /* Current machine state */
    unsigned int            authmechs;   /* Accepted authentication mechanisms */
    unsigned int            prefmech;    /* Preferred authentication mechanism */
    unsigned int            authused;    /* Auth mechanism used for the connection */
    qosa_bool_t                    resetprefs;  /* For URL auth option parsing. */
    qosa_bool_t                    mutual_auth; /* Mutual authentication enabled (GSSAPI only) */
    qosa_bool_t                    force_ir;    /* Protocol always supports initial response */
};
typedef struct qurl_sasl_s qurl_sasl_t;
#else
/* Protocol dependent SASL parameters */
struct qurl_saslproto_s
{
    const char  *service;                                                   /* The service name */
    int          contcode;                                                  /* Code to receive when continuation is expected */
    int          finalcode;                                                 /* Code to receive upon authentication success */
    unsigned int maxirlen;                                                  /* Maximum initial response length */
    qurl_ecode_t (*sendauth)(void *conn, const char *mech, const char *ir); /* Send authentication command */
    qurl_ecode_t (*sendcont)(void *conn, const char *contauth);             /* Send authentication continuation */
    void (*getmessage)(char *buffer, char **outptr);                        /* Get SASL response message */
};
typedef struct qurl_saslproto_s qurl_saslproto_t;

/* Per-connection parameters */

struct qurl_sasl_s
{
    const qurl_saslproto_t *params;      /* Protocol dependent parameters */
    saslstate               state;       /* Current machine state */
    unsigned int            authmechs;   /* Accepted authentication mechanisms */
    unsigned int            prefmech;    /* Preferred authentication mechanism */
    unsigned int            authused;    /* Auth mechanism used for the connection */
    qosa_bool_t             resetprefs;  /* For URL auth option parsing. */
    qosa_bool_t             mutual_auth; /* Mutual authentication enabled (GSSAPI only) */
    qosa_bool_t             force_ir;    /* Protocol always supports initial response */
};
typedef struct qurl_sasl_s qurl_sasl_t;

#endif

/* This is used to test whether the line starts with the given mechanism */
#define sasl_mech_equal(line, wordlen, mech) (wordlen == (sizeof(mech) - 1) / sizeof(char) && !memcmp(line, mech, wordlen))

#if 0
/* Convert a mechanism name to a token */
unsigned int qurl_sasl_decode_mech(const char *ptr, int maxlen, int *len);

/* Parse the URL login options */
qurl_ecode_t qurl_sasl_parse_url_auth_option(qurl_sasl_t *sasl, const char *value, int len);

/* Initializes an SASL structure */
void qurl_sasl_init(qurl_sasl_t *sasl, const qurl_saslproto_t *params);

/* Check if we have enough auth data and capabilities to authenticate */
qosa_bool_t qurl_sasl_can_authenticate(qurl_sasl_t *sasl, qurl_conn_t *conn);

/* Calculate the required login details for SASL authentication  */
qurl_ecode_t qurl_sasl_start(qurl_sasl_t *sasl, qurl_conn_t *conn, qosa_bool_t force_ir, saslprogress *progress);

/* Continue an SASL authentication  */
qurl_ecode_t qurl_sasl_continue(qurl_sasl_t *sasl, qurl_conn_t *conn, int code, saslprogress *progress);
#endif

#endif /* HEADER_CURL_SASL_H */
