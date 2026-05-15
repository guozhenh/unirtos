/*
 * Portions of this file are derived from curl/libcurl's lib/smtp.h.
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
#ifndef QURL_SMTP_DEF_H
#define QURL_SMTP_DEF_H

#include "qurl_config.h"
#include "qurl_code_in.h"

#if QURL_CFG_ENABLE_SMTP
#include "qurl_sasl.h"
#include "qurl_pingpong.h"

/****************************************************************************
 * SMTP unique setup
 ***************************************************************************/
typedef enum
{
    SMTP_STOP = 0,    /* do nothing state, stops the state machine */
    SMTP_SERVERGREET, /* waiting for the initial greeting immediately after a connect */
    SMTP_EHLO,
    SMTP_HELO,
    SMTP_STARTTLS,
    SMTP_UPGRADETLS = 5, /* asynchronously upgrade the connection to SSL/TLS (multi mode only) */
    SMTP_AUTH,
    SMTP_COMMAND,        /* VRFY, EXPN, NOOP, RSET and HELP */
    SMTP_MAIL,           /* MAIL FROM */
    SMTP_RCPT,           /* RCPT TO */
    SMTP_DATA = 10,
    SMTP_POSTDATA,
    SMTP_QUIT,
    SMTP_LAST /* never used */
} smtpstate;

/**
 * @struct  qurl_core_usr_cfg_smtp_t
 * @brief smtp exclusive user configuration information
 */
struct qurl_core_usr_cfg_smtp_s
{
    char              *mail_from_ptr;    /*!< Mail sender*/
    char              *mail_rtcp_ptr;    /*!< Mail recipient*/
    char              *mail_auth_ptr;
    char              *mail_login_ptr;
    char              *mail_xoauth2_ptr;
    char              *sasl_authzid_ptr;
    qosa_bool_t        sasl_ir;
    qurl_slist_ctrl_t *rtcp_slist;
};
typedef struct qurl_core_usr_cfg_smtp_s qurl_core_usr_cfg_smtp_t;

/* This SMTP struct is used in the Curl_easy. All SMTP data that is
   connection-oriented must be in smtp_conn to properly deal with the fact that
   perhaps the Curl_easy is changed between the times the connection is
   used. */

struct SMTP
{
    qurl_pp_transfer_e transfer;
    char              *custom;          /* Custom Request */
    qurl_slist_ctrl_t *rcpt;            /* Recipient list */
    qurl_bit_t         rcpt_had_ok;     /* Whether any of RCPT TO commands (depends on total number of recipients) succeeded so far */
    int                rcpt_last_error; /* The last error received for RCPT TO command */
    int                eob;             /* Number of bytes of the EOB (End Of Body) that have been received so far */
    qurl_bit_t         trailing_crlf;   /* Specifies if the tailing CRLF is present */
    char              *path;
};

/* smtp_conn is used for struct connection-oriented data in the connectdata
   struct */

struct hostname
{
    char       *rawalloc;
    char       *encalloc;
    char       *name;
    const char *dispname;
};

struct qurl_smtp_ctrl_s
{
    qurl_pp_ctrl_t pp;
    smtpstate      state;          /* Always use smtp.c:state() to change state! */
    qurl_bit_t     ssldone;        /* Is connect() over SSL done? */
    char          *domain;         /* Client address/name to send in the EHLO */
    qurl_bit_t     tls_supported;  /* StartTLS capability supported by server */
    qurl_bit_t     size_supported; /* If server supports SIZE extension according to RFC 1870 */
    qurl_bit_t     utf8_supported; /* If server supports SMTPUTF8 extension according to RFC 6531 */
    qurl_bit_t     auth_supported; /* AUTH capability supported by server */
    qurl_sasl_t    sasl;           /* SASL-related storage */
};
typedef struct qurl_smtp_ctrl_s qurl_smtp_ctrl_t;

/* this is the 5-bytes End-Of-Body marker for SMTP */
#define SMTP_EOB          "\x0d\x0a\x2e\x0d\x0a"
#define SMTP_EOB_LEN      5
#define SMTP_EOB_FIND_LEN 3

/* if found in data, replace it with this string instead */
#define SMTP_EOB_REPL     "\x0d\x0a\x2e\x2e"
#define SMTP_EOB_REPL_LEN 4

//qurl_ecode_t qurl_smtp_escape_eob(qurl_conn_t *conn, const int nread);

#endif
#endif /* HEADER_CURL_SMTP_H */
