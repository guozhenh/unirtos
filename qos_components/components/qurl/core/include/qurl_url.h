/** @file         include/qurl_url.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-06 17:54:24
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#ifndef QURL_URL_H
#define QURL_URL_H

#include "qurl_def_in.h"
#include "qurl_log.h"
#include "qurl_code_in.h"
#include "qurl_ctype.h"

#define QURL_URL_DEFAULT            (0 << 0) /*!< No special requirements*/
#define QURL_URL_DEFAULT_PORT       (1 << 0) /*!< Returns the default port number*/
#define QURL_URL_NO_DEFAULT_PORT    (1 << 1) /*!< Do not use default port numbers: if the specified port number is the same as the default port number of the protocol stack, it is considered that the URL does not specify a port number.*/
#define QURL_URL_DEFAULT_SCHEME     (1 << 2)  /*!< If no scheme is present, allow returning the default scheme*/
#define QURL_URL_NON_SUPPORT_SCHEME (1 << 3)  /*!< Allow unsupported protocols.*/
#define QURL_URL_PATH_AS_IS         (1 << 4)  /*!< Keep the URL as is, do not process "." and ".."*/
#define QURL_URL_DISALLOW_USER      (1 << 5)  /*!< Do not use username and password*/
#define QURL_URL_URLDECODE          (1 << 6)  /*!< Perform URL decoding when fetching*/
#define QURL_URL_URLENCODE          (1 << 7)  /*!< URL encode when setting*/
#define QURL_URL_APPENDQUERY        (1 << 8)  /*!< Add a form-style section*/
#define QURL_URL_GUESS_SCHEME       (1 << 9)  /*!< Legacy conjecture*/
#define QURL_URL_NO_AUTHORITY       (1 << 10) /*!< Allow empty authority when the protocol is unknown*/
#define QURL_URL_ALLOW_SPACE        (1 << 11) /*!< Allow spaces in URLs*/
#define QURL_URL_PUNYCODE           (1 << 12) /*!< Get the hostname in punycode format*/
#define QURL_URL_PUNY2IDN           (1 << 13) /*!< Convert hostname from punycode format to IDN*/

/**
 * @enum  qurl_url_reject_e
 * @brief URL decode rejection code
 */
typedef enum
{
    QURL_URL_REJECT_NADA = 0, /*!< Accept everything, meaning do not reject any characters.*/
    QURL_URL_REJECT_CTRL,     /*!< Reject control characters, which refer to characters with byte codes less than 32.*/
    QURL_URL_REJECT_ZERO,     /*!< Reject strings containing null bytes after decoding.*/
} qurl_url_reject_e;

struct qurl_url_s
{
    char *scheme;    //Protocol Section
    char *user;      //Username
    char *password;  //Password
    char *options;   //Options (IMAP only?)
    char *host;      //Hostname or IP address
    char *zoneid;    //Zone ID for numeric IPv6 addresses
    char *port;      //Port number
    char *path;      //Path
    char *query;     //Query string
    char *fragment;  //Anchor Point

    char *scratch;   //Temporary buffer for ease of logic implementation
    char *temppath;  //Pointer to the temporary path
    long  portnum;   //The numerical form of the port number
};
typedef struct qurl_url_s qurl_url_t;

qurl_ecode_t qurl_url_create(qurl_url_t **uurt_pptr);

qurl_ecode_t qurl_url_delete(qurl_url_t *uurt_ptr);

qurl_ecode_t qurl_url_getopt(qurl_url_t *uurl_ptr, qurl_url_opt_e opt, char **part_pptr, unsigned int flags);

qurl_ecode_t qurl_url_setopt(qurl_url_t *uurl_ptr, qurl_url_opt_e opt, const char *part_ptr, unsigned int flags);

qurl_ecode_t qurl_url_decode(const char *str_ptr, qosa_size_t str_len, char **ostr_pptr, qosa_size_t *ostr_len_ptr, qurl_url_reject_e ctrl);

qurl_bit_t qurl_url_is_absolute(const char *url_ptr, char *buf_ptr, qosa_uint16_t buf_len);

#endif /* Head define end*/
