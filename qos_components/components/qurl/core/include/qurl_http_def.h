/** @file         qurl_http_def.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-21 16:53:48
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#ifndef QURL_HTTP_DEF_H
#define QURL_HTTP_DEF_H

#include "qurl_config.h"
#include "qurl_def_basic.h"
#include "qurl_http_chunked.h"
#include "qurl_http_multiform.h"

#if QURL_CFG_ENABLE_HTTP

/*Macro Definitions*/
/**
 * @enum  qurl_http_state_e
 * @brief Internal detailed status codes for HTTP
 */
typedef enum
{
    QURL_HTTP_STATE_IDLE = 0,     /*!< Idle state*/
    QURL_HTTP_STATE_WRITE_HEADER, /*!< Sending request header phase*/
    QURL_HTTP_STATE_WRITE_BODY,   /*!< Sending request body phase*/
    QURL_HTTP_STATE_READ_HEADER,  /*!< Reading response header phase*/
    QURL_HTTP_STATE_READ_BODY,    /*!< Reading response body phase*/
} qurl_http_state_e;

/*Core User Configuration*/
/**
 * @struct  qurl_core_usr_cfg_http_t
 * @brief Exclusive user configuration information for HTTP
 */
struct qurl_core_usr_cfg_http_s
{
    long                       http_version;       /*!< HTTP version, refer to qurl_http_version_e*/
    long                       http_method;        /*!< HTTP request method, refer to qurl_http_method_e*/
    long                       http_auth;          /*!< HTTP authentication scheme, refer to qurl_http_auth_e*/
    char                       keep_post;          /*!< Keep POST requests as POST after 30x requests; each bit represents a request, from 301 to 303*/
    qurl_http_multiform_ctrl_t multiform_ctrl;     /*!< User-defined multi-form, ordered, mounts qurl_http_form_ctrl_t*/
    qurl_slist_ctrl_t         *http_headers_slist; /*!< User-defined fields for the HTTP request header. These fields have the highest priority in the request header.*/
};
typedef struct qurl_core_usr_cfg_http_s qurl_core_usr_cfg_http_t;

/*Core Configuration*/
/**
 * @struct  qurl_core_cfg_http_t
 * @brief Exclusive configuration information for HTTP
 */
struct qurl_core_cfg_http_s
{
    long               http_method;             /*!< HTTP request method, refer to qurl_http_method_e*/
    long               http_auth;               /*!< HTTP authentication scheme, refer to qurl_http_auth_e*/
    long               http_auth_recv;          /*!< HTTP authentication scheme after receiving 401 or 407, refer to qurl_http_auth_e*/
    qurl_slist_ctrl_t *http_resp_headers_slist; /*!< Response header list*/
};
typedef struct qurl_core_cfg_http_s qurl_core_cfg_http_t;

/**
 * @brief HTTP parsing result
 */
struct qurl_http_parsed_s
{
    /*[lzm][todo]Remove unused items*/
    int   http_version;      /*!< HTTP version of server response: actual version *10, to avoid floating point*/
    int   http_code;         /*!< HTTP result code from server response*/
    long  content_length;    /*!< The length of the response body.*/
    char *content_type;      /*!< control_type, stores a string.*/
    char *transfer_encoding; /*!< Transfer-Encoding, stores a string.*/
    char *www_authenticate;  /*!< www_authenticate_ptr, field.*/
    char *location;          /*!< location, field.*/
};
typedef struct qurl_http_parsed_s qurl_http_parsed_t;

/*Data Structure*/
/**
 * @struct qurl_http_ctrl_t
 * @brief Data structure for HTTP protocol control
 */
struct qurl_http_ctrl_s
{
    /*Control Block*/
    qurl_http_state_e state; /*!< Marks the current state of HTTP processing*/

    /*Send Section*/
    unsigned char autoid_head_raw_cache[3]; /*!< Buffer for automatic recognition of "\r\n\r\n"*/

    /*Receive and Parse Section*/
    int                header_line; /*!< Number of successfully parsed HTTP response header lines*/
    qurl_http_parsed_t resp;        /*!< Response Result Parsing*/

    /*Embedded Transmission Related*/
    qurl_http_chunked_ctrl_t chunked; /*!< Dedicated for chunked transmission*/

    qurl_bit_t header;                /*!< Indicates that the incoming data contains HTTP headers.*/
    qurl_bit_t ignore_header;         /*!< Received the response header, but we ignore these bodies (not uploaded to the user side), such as redirection*/
    qurl_bit_t ignore_body;           /*!< Received the response body, but we ignore these bodies (not uploaded to the user side), such as redirection*/
    qurl_bit_t http_bodyless;         /*!< The HTTP response body should be zero: the HTTP response status code is between 100 and 199, 204, or 304.*/
    qurl_bit_t chunk;                 /*!< Indicates that the data uses chunked transfer encoding.*/
};
typedef struct qurl_http_ctrl_s qurl_http_ctrl_t;

#endif

#endif /* Head define end*/
