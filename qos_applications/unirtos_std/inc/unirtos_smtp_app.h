/**  @file
  unirtos_smtp_app.h

  @brief
  TODO

*/

/*================================================================
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/

#ifndef __UNIRTOS_SMTP_APP_H
#define __UNIRTOS_SMTP_APP_H

#define QL_SMTP_PARAM_LEN_MAX      (51)   // Contains the end of string flag '\0'
#define QL_SMTP_SUBJECT_LEN_MAX    (101)  // Contains the end of string flag '\0'
#define QL_SMTP_RECIPIENT_NUM_MAX  (20)
#define QL_SMTP_ATTACHMENT_NUM_MAX (10)
#define QL_SMTP_FILE_NAME_LEN_MAX  (140)
#define QL_SMTP_MAIL_BODY_LEN_MAX  (10240)  // Don't contain the end of string flag '\0'

//typedef int ql_smtp_client_t;
typedef long ql_smtp_client_t;

typedef enum
{
    QL_SMTP_OK = 0,

    QL_SMTP_ERROR_UNKNOWN = 651,          /* Unknown error */
    QL_SMTP_ERROR_SERVER_BUSY = 652,      /* The SMTP server is busy, such as uploading the body or sending an email. */
    QL_SMTP_ERROR_FAILED_DNS = 653,       /* Failed to get IP address according to the domain name. */
    QL_SMTP_ERROR_NETWORK = 654,          /* Network error, such as failed to activate GPRS/CSD context, failed to establish the
																						TCP connection with the SMTP server or failed to send an email to the SMTP
																						server, etc. */
    QL_SMTP_ERROR_UNSUPPORT_AUTH = 655,   /* Unsupported authentication type */
    QL_SMTP_ERROR_CONN_CLOSED = 656,      /* The connection for the SMTP server is closed by peer. */
    QL_SMTP_ERROR_PDP_DEACT = 657,        /* GPRS/CSD context is deactivated. */
    QL_SMTP_ERROR_TIMEOUT = 658,          /* Timeout */
    QL_SMTP_ERROR_NO_RECIPIENT = 659,     /* No recipient for the SMTP server */
    QL_SMTP_ERROR_FAILED_SEND = 660,      /* Failed to send an email */
    QL_SMTP_ERROR_FAILED_OPEN_FILE = 661, /* Failed to open a file */
    QL_SMTP_ERROR_NO_ENOUGH_MEMORY = 662, /* No enough memory */
    QL_SMTP_ERROR_FAILED_SAVE_ATTA = 663, /* Failed to save the attachment */
    QL_SMTP_ERROR_INVALID_PARAM = 664,    /* The input parameter is wrong */
    QL_SMTP_ERROR_FAILED_SSL_AUTH = 665,  /* SSL authentication failed */

    QL_SMTP_ERROR_NOT_AVAILABLE = 666,    /* Service not available, closing transmission channel */
    QL_SMTP_ERROR_MAILBOX_UNAVAIL = 667,  /* Requested mail action not taken: mailbox unavailable */
    QL_SMTP_ERROR_LOCAL_PROCE = 668,      /* Requested action aborted: local error in processing */
    QL_SMTP_ERROR_SPACE_NOT_ENOUGH = 669, /* Requested action not taken: insufficient system storage */
    QL_SMTP_ERROR_SYNTAX_CMD = 670,       /* Syntax error, command unrecognized */
    QL_SMTP_ERROR_SYNTAX_PARAM = 671,     /* Syntax error in parameters or arguments */
    QL_SMTP_ERROR_CMD_NOT_EXCU = 672,     /* Command not implemented */
    QL_SMTP_ERROR_CMD_BAD_SEQUENCE = 673, /* Bad sequence of commands */
    QL_SMTP_ERROR_CMD_PARAM_NORUN = 674,  /* Command parameter not implemented */
    QL_SMTP_ERROR_DOMAIN_NOT_SMTP = 675,  /* <domain> does not accept mail (see RFC1846) */
    QL_SMTP_ERROR_ACCESS_DENIED = 676,    /* Access denied */
    QL_SMTP_ERROR_FAILED_AUTH = 677,      /* Authentication failed */
    QL_SMTP_ERROR_MAILBOX_UNAVAIL2 = 678, /* Requested action not taken: mailbox unavailable */
    QL_SMTP_ERROR_USER_NOT_LOCAL = 679,   /* User not local; please try <forward-path> */
    QL_SMTP_ERROR_STORAGE_EXCEED = 680,   /* Requested mail action aborted: exceeded storage allocation */
    QL_SMTP_ERROR_MAILBOX_NAME = 681,     /* Requested action not taken: mailbox name not allowed */
    QL_SMTP_ERROR_FAILED_TRANSACT = 682,  /* Transaction failed */
} ql_smtp_errcode_e;

typedef enum
{
    /*
    * Function implemented according to RFC2821 (https://www.rfc-editor.org/rfc/pdfrfc/rfc2821.txt.pdf).
    * This document details the protocol specifications for email transmission.
    */
    QL_SMTP_PRO_SUCCESS = 200,          /* SUCCESS */
    QL_SMTP_PRO_SYS_STATUS = 211,       /* System status, or system help reply */
    QL_SMTP_PRO_HELP_MSG = 214,         /* Help message (Information on how to use the receiver or the meaning of a
														particular non-standard command; this reply is useful only to the human user) */
    QL_SMTP_PRO_SERVICE_READY = 220,    /* <domain> Service ready */
    QL_SMTP_PRO_SERVICE_CLOSE = 221,    /* <domain> Service closing transmission channel */
    QL_SMTP_PRO_AUTH_SUCCESS = 235,     /* Authentication successful */
    QL_SMTP_PRO_MAIL_COMPLETE = 250,    /* Requested mail action okay, completed */
    QL_SMTP_PRO_NOT_LOCAL = 251,        /* User not local; will forward to <forward-path> */
    QL_SMTP_PRO_CANNOT_VRFY_USER = 252, /* Cannot VRFY user, but will accept message and attempt delivery */
    QL_SMTP_PRO_START_AUTH = 334,       /* Start Authentication */
    QL_SMTP_PRO_START_MAIL = 354,       /* Start mail input; end with <CRLF>.<CRLF> */

    QL_SMTP_PRO_NOT_AVAILABLE = 421,    /* <domain> Service not available, closing transmission channel
														(This may be a reply to any command if the service knows it must shut down) */
    QL_SMTP_PRO_MAILBOX_UNAVAIL = 450,  /* Requested mail action not taken: mailbox unavailable (e.g., mailbox busy) */
    QL_SMTP_PRO_LOCAL_PROCE = 451,      /* Requested action aborted: error in processing */
    QL_SMTP_PRO_SPACE_NOT_ENOUGH = 452, /* Requested action not taken: insufficient system storage */
    QL_SMTP_PRO_SYNTAX_CMD = 500,       /* Syntax error, command unrecognized (This may include errors such as command line too long) */
    QL_SMTP_PRO_SYNTAX_PARAM = 501,     /* Syntax error in parameters or arguments */
    QL_SMTP_PRO_CMD_NOT_EXCU = 502,     /* Command not implemented */
    QL_SMTP_PRO_CMD_BAD_SEQUENCE = 503, /* Bad sequence of commands */
    QL_SMTP_PRO_CMD_PARAM_NORUN = 504,  /* Command parameter not implemented */
    QL_SMTP_PRO_DOMAIN_NOT_SMTP = 521,  /* <domain> does not accept mail (see RFC1846) */
    QL_SMTP_PRO_ACCESS_DENIED = 530,    /* Access denied */
    QL_SMTP_PRO_FAILED_AUTH = 535,      /* Authentication failed */
    QL_SMTP_PRO_MAILBOX_UNAVAIL2 = 550, /* Requested action not taken: mailbox unavailable
														(e.g., mailbox not found, no access, or command rejected for policy reasons) */
    QL_SMTP_PRO_USER_NOT_LOCAL = 551,   /* User not local; please try <forward-path> */
    QL_SMTP_PRO_STORAGE_EXCEED = 552,   /* Requested mail action aborted: exceeded storage allocation */
    QL_SMTP_PRO_MAILBOX_NAME = 553,     /* Requested action not taken: mailbox name not allowed (e.g., mailbox syntax incorrect) */
    QL_SMTP_PRO_FAILED_TRANSACT = 554,  /* Transaction failed (Or, in the case of a connection-opening response, "No SMTP service here") */
} ql_smtp_pro_code_e;

typedef enum
{
    QL_SMTP_OPT_USERNAME = 1, /* 1 */  /* Sender email account */
    QL_SMTP_OPT_PASSWORD,              /* Sender email password */
    QL_SMTP_OPT_NICKNAME,              /* Sender display nickname */
    QL_SMTP_OPT_EMAIL_ADDR,            /* Sender email address */
    QL_SMTP_OPT_SUBJECT, /* 5 */       /* Email Subject */
    QL_SMTP_OPT_SUBJECT_CHARSET,       /* Email Subject Encoding */
    QL_SMTP_OPT_RECIPIENT,             /* Email recipient */
    QL_SMTP_OPT_ATTACHMENT,            /* Email Attachment */
    QL_SMTP_OPT_BODY,                  /* Email body */
    QL_SMTP_OPT_BODY_CHARSET, /* 10 */ /* Email body encoding type */
    QL_SMTP_OPT_SERVER_ADDR,           /* SMTP server address */
    QL_SMTP_OPT_SERVER_PORT,           /* SMTP server port */
    QL_SMTP_OPT_SIMCID,                /* SMTP connection uses nSim and cid, refer to ql_bind_sim_and_profile() */
    QL_SMTP_OPT_SSL_CTXID,             /* SMTP encrypted connection configuration ctx ID */
    QL_SMTP_OPT_SSL_TYPE, /* 15 */     /* SMTP encryption connection method */
    QL_SMTP_OPT_BODY_ORIGINAL,         /* The email body is not encoded, currently invalid */
    QL_SMTP_OPT_PROTOCOL_ERR,          /* SMTP protocol interaction error codes */
    QL_SMTP_OPT_ADDR_SEPARATOR,        /* SMTP multiple address separator */
} ql_smtp_option_e;                    /* Client configuration item ID */

typedef enum
{
    QL_SMTP_SSL_TYPE_NO = 0,
    QL_SMTP_SSL_TYPE_SSL = 1,      /* smtps */
    QL_SMTP_SSL_TYPE_STARTTLS = 2, /* smtp + tls */
} ql_smtp_ssl_type_e;              /* SMTP encryption connection method */

typedef enum
{
    QL_SMTP_DST_ALL = 0,    /* All, only for deletion */
    QL_SMTP_DST_RECP = 1,   /* Recipient */
    QL_SMTP_DST_COPY = 2,   /* Copy to */
    QL_SMTP_DST_SECRET = 3, /* Blind carbon copy */
} ql_smtp_dst_e;            /* Recipient type */

typedef enum
{
    QL_SMTP_CHARSET_ASCII,  /* ASCII code */
    QL_SMTP_CHARSET_UTF8,   /* UTF-8 Encoding */
    QL_SMTP_CHARSET_GB2312, /* GB2312 encoding */
    QL_SMTP_CHARSET_BIG5,   /* BIG5 encoding */
} ql_smtp_charset_e;        /* Theme and body encoding format */

/*****************************************************************
* Type: ql_smtp_recipient_t
*
* Description:
*	Information related to the recipient. Can be directly used for set/del operations.
*
* Parameters:
*	type				[in/out]	Recipient type. For del, 0 means delete all.
*										For set, only QL_SMTP_DST_RECP, QL_SMTP_DST_COPY, QL_SMTP_DST_SECRET are valid.
*	email				[in/out]	Recipient email address.
*
*****************************************************************/
typedef struct
{
    ql_smtp_dst_e type;
    char          email[QL_SMTP_PARAM_LEN_MAX];
} ql_smtp_recipient_t;

/*****************************************************************
* Type: ql_smtp_attachment_t
*
* Description:
*	Information related to an attachment. Can be directly used for set/del operations.
*
* Parameters:
*	id					[in/out]	Attachment ID. For del, 0 indicates deleting all.
*	file_name			[in/out]	Attachment file name (path). [Refer to the file system API documentation for the format]
*
*****************************************************************/
typedef struct
{
    int  id;
    char file_name[QL_SMTP_FILE_NAME_LEN_MAX];
} ql_smtp_attachment_t;

/*****************************************************************
* Type: ql_smtp_recipient_read_t
*
* Description:
*	Used to retrieve recipient-related information from the client.
*
* Parameters:
*	count				[out]	The number of valid recipients retrieved.
*	data				[out]	The retrieved valid recipient information. See: ql_smtp_recipient_t
*
*****************************************************************/
typedef struct
{
    int                 count;
    ql_smtp_recipient_t data[QL_SMTP_RECIPIENT_NUM_MAX];
} ql_smtp_recipient_read_t;

/*****************************************************************
* Type: ql_smtp_attachment_read_t
*
* Description:
*   Used to obtain the client's attachment-related information.
*
* Parameters:
*   count               [out]   The number of valid attachments obtained.
*   data                [out]   The obtained valid attachment information. See: ql_smtp_attachment_t
*
*****************************************************************/
typedef struct
{
    int                  count;
    ql_smtp_attachment_t data[QL_SMTP_ATTACHMENT_NUM_MAX];
} ql_smtp_attachment_read_t;

/*****************************************************************
* Function: ql_smtp_client_new
*
* Description:
*	Creates a new SMTP client for sending emails.
*
* Parameters:
*	client				[out]	The client entity.
*
* Return:
*	0			Success.
*	other		Error code.
*
*****************************************************************/
int ql_smtp_client_new(ql_smtp_client_t *client);

/*****************************************************************
* Function: ql_smtp_setopt
*
* Description:
*	Sets client-related configurations.
*
* Parameters:
*	client				[in]	Client entity.
*	opt_tag				[in]	ID of the configuration item to set. QL_SMTP_OPT_PROTOCOL_ERR is not supported for setting.
*	value				[in]	Specific parameter value to set.
*	len					[in]	Data length of the specific parameter, e.g., strlen(value) for a string, sizeof(int) for an integer, sizeof(struct) for a structure.
*
* Return:
*	0			Success.
*	other		Error code.
*
*****************************************************************/
int ql_smtp_setopt(ql_smtp_client_t client, int opt_tag, void *value, int len);

/*****************************************************************
* Function: ql_smtp_setopt
*
* Description:
*   Retrieves client-related configuration.
*
* Parameters:
*   client              [in]    Client entity.
*   opt_tag             [in]    ID of the configuration item to retrieve. QL_SMTP_OPT_BODY is not supported for retrieval.
*   value               [out]   Used to store the specific parameter to be retrieved.
*   len                 [in]    Size of the space available to store the retrieved parameter, e.g., sizeof(buffer) for strings, sizeof(int) for integers, sizeof(struct) for structures.
*
* Return:
*   0               Success.
*   other           Error code.
*
*****************************************************************/
int ql_smtp_getopt(ql_smtp_client_t client, int opt_tag, void *value, int len);

/*****************************************************************
* Function: ql_smtp_setopt
*
* Description:
*	Deletes client-related configurations.
*
* Parameters:
*	client				[in]	Client entity.
*	opt_tag				[in]	The ID of the configuration item to be retrieved.
*								Only QL_SMTP_OPT_SUBJECT, QL_SMTP_OPT_BODY, QL_SMTP_OPT_RECIPIENT, QL_SMTP_OPT_ATTACHMENT are supported.
*	value				[in]	The relevant option for the specific parameter to be deleted.
*	len					[in]	The size of the space available to store the retrieved parameter, e.g., sizeof(buffer) for strings, sizeof(int) for integers, sizeof(struct) for structures.
*
* Return:
*	0			Success.
*	other		Error code.
*
*****************************************************************/
int ql_smtp_delopt(ql_smtp_client_t client, int opt_tag, void *value, int len);

/*****************************************************************
* Function: ql_smtp_setopt
*
* Description:
*	Deletes client-related configurations.
*
* Parameters:
*	client				[in]	Client entity.
*	timeout				[in]	Timeout for sending emails, in seconds.
*
* Return:
*	0			Success.
*	other		Error code.
*
*****************************************************************/
int ql_smtp_put(ql_smtp_client_t client, int timeout);

#endif /* __QUEUECOS_SMTP_APP_H__ */
