/** @file         qurl_code.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-01 15:52:48
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#ifndef QURL_CODE_IN_H
#define QURL_CODE_IN_H

#include "qurl_config.h"
#include "qurl_code.h"

/**
 * @enum  qurl_ecode_e
 * @brief qurl global event code
 */
typedef enum
{
    QURL_EVENT_CODE_NETWORK = (1 | QURL_CFG_EVENT_CODE_BASIC_NUM | 0x80000000),
    QURL_EVENT_CODE_RESOLVE,
} qurl_event_code_e;

/**
 * @enum  qurl_state_e
 * @brief qurl core status codes
 */
typedef enum
{
    QURL_STATE_INIT = 0, /*!< Initial state*/
    //QURL_STATE_CONNECT_PEND,     /*!< Not connected, waiting for a connection */
    QURL_STATE_NETWORK,            /*!< Activate Data Link*/
    QURL_STATE_CONNECTION,         /*!< Apply for and bind a connection*/
    QURL_STATE_RESOLVE,            /*!< Parse operation*/
    QURL_STATE_CONNECT,            /*!< TCP connection*/
    QURL_STATE_WAIT_PROXY_CONNECT, /*!< Wait for SSL initialization of the HTTPS proxy or completion of the proxy connection*/
    QURL_STATE_SEND_PROT_CONNECT,  /*!< Start the protocol connection process*/
    QURL_STATE_PROT_CONNECT,       /*!< Complete the protocol-specific connection phase*/
    QURL_STATE_REQ,                /*!< Start sending request (first part)*/
    QURL_STATE_REQING,             /*!< Sending request (first part)*/
    QURL_STATE_REQ_MORE,           /*!< Continue sending the request (Part 2)*/
    QURL_STATE_REQ_DONE,           /*!< Request transmission completed*/
    QURL_STATE_TRANS_DATA,         /*!< Data transmission phase*/
    QURL_STATE_TRANS_TOOFAST,      /*!< Waiting due to speed limit*/
    QURL_STATE_TRANS_DONE,         /*!< Data transfer operation completed*/
    QURL_STATE_COMPLETED,          /*!< Operation completed*/
    QURL_STATE_MSGSENT,            /*!< Operation completion message has been sent*/
    QURL_STATE_LAST,               /*!< Not a real state, only marks the upper limit of state values.*/
} qurl_state_e;

/**
 * @enum  qurl_conn_state_e
 * @brief for the (SOCKS) connect state machine
 */
typedef enum
{
    QURL_CONN_STATE_INIT = 0,
    QURL_CONN_STATE_SOCKS_INIT,      /*!< 1 */
    QURL_CONN_STATE_SOCKS_SEND,      /*!< 2 waiting to send more first data */
    QURL_CONN_STATE_SOCKS_READ_INIT, /*!< 3 set up read */
    QURL_CONN_STATE_SOCKS_READ,      /*!< 4 read server response */
    QURL_CONN_STATE_GSSAPI_INIT,     /*!< 5 */
    QURL_CONN_STATE_AUTH_INIT,       /*!< 6 setup outgoing auth buffer */
    QURL_CONN_STATE_AUTH_SEND,       /*!< 7 send auth */
    QURL_CONN_STATE_AUTH_READ,       /*!< 8 read auth response */
    QURL_CONN_STATE_REQ_INIT,        /*!< 9 init SOCKS "request" */
    QURL_CONN_STATE_RESOLVING,       /*!< 10 */
    QURL_CONN_STATE_RESOLVED,        /*!< 11 */
    QURL_CONN_STATE_RESOLVE_REMOTE,  /*!< 12 */
    QURL_CONN_STATE_REQ_SEND,        /*!< 13 */
    QURL_CONN_STATE_REQ_SENDING,     /*!< 14 */
    QURL_CONN_STATE_REQ_READ,        /*!< 15 */
    QURL_CONN_STATE_REQ_READ_MORE,   /*!< 16 */
    QURL_CONN_STATE_DONE             /*!< 17 connected fine to the remote or the SOCKS proxy */
} qurl_conn_state_e;

/**
 * @enum  qurl_sec_state_e
 * @brief sec_state status code
 */
typedef enum
{
    QURL_SEC_STATE_IDLE = 0, /*!< Idle state*/
    QURL_SEC_STATE_BUSY,     /*!< Busy*/
    QURL_SEC_STATE_CLOSING,  /*!< Closing*/
} qurl_sec_state_e;

/**
 * @struct  qurl_conn_trans_type_e
 * @brief conn transport layer type
 */
typedef enum
{
    QURL_CONN_TRANS_TYPE_TCP = 3, /*!< TCP */
    QURL_CONN_TRANS_TYPE_UDP,     /*!< UDP */
    QURL_CONN_TRANS_TYPE_QUIC,    /*!< QUIC */
} qurl_conn_trans_type_e;

/**
 * @enum  qurl_socket_index_e
 * @brief Socket connection index for each connection
 */
typedef enum
{
    QURL_SOCKET_INDEX_0 = 0,
    QURL_SOCKET_INDEX_1,
    QURL_SOCKET_INDEX_LAST,
    QURL_SOCKET_SERVER_INDEX_0 = QURL_SOCKET_INDEX_LAST,
    QURL_SOCKET_SERVER_INDEX_LAST,
} qurl_socket_index_e;

/*Protocols supported by qurl*/
#define QURL_PROT_TYPE_HTTP             (1 << 0)
#define QURL_PROT_TYPE_HTTPS            (1 << 1)
#define QURL_PROT_TYPE_FTP              (1 << 2)
#define QURL_PROT_TYPE_FTPS             (1 << 3)
#define QURL_PROT_TYPE_SMTP             (1 << 4)
#define QURL_PROT_TYPE_SMTPS            (1 << 5)

#define QURL_PROT_TYPE_ALL              (~0) /* enable everything */

/*qurl protocol flag*/
#define QURL_PROT_FLAGS_NONE            (0)       /*!< No additional flags*/
#define QURL_PROT_FLAGS_SSL             (1 << 0)  /*!< Use SSL, i.e., Secure Sockets Layer protocol*/
#define QURL_PROT_FLAGS_DUAL            (1 << 1)  /*!< This protocol uses two connections*/
#define QURL_PROT_FLAGS_CLOSEACTION     (1 << 2)  /*!< Certain operations need to be performed before closing the socket*/
#define QURL_PROT_FLAGS_DIRLOCK         (1 << 3)  /*!< Some protocols require calling underlying functions in specific states*/
#define QURL_PROT_FLAGS_NONETWORK       (1 << 4)  /*!< This protocol does not use a network connection*/
#define QURL_PROT_FLAGS_NEEDSPWD        (1 << 5)  /*!< Password required; if no password is set, use the default password.*/
#define QURL_PROT_FLAGS_NOURLQUERY      (1 << 6)  /*!< The protocol cannot handle URL query strings (?foo=bar)*/
#define QURL_PROT_FLAGS_CREDSPERREQUEST (1 << 7)  /*!< Each request requires login credentials, not each connection.*/
#define QURL_PROT_FLAGS_ALPN_NPN        (1 << 8)  /*!< Set ALPN and/or NPN for this protocol*/
#define QURL_PROT_FLAGS_STREAM          (1 << 9)  /*!< This protocol supports independent logical flows*/
#define QURL_PROT_FLAGS_URLOPTIONS      (1 << 10) /*!< Allows inclusion of the option section in the user information field of the URL*/
#define QURL_PROT_FLAGS_PROXY_AS_HTTP   (1 << 11) /*!< Allows non-HTTP protocols to be transmitted as HTTP via an HTTP proxy*/
#define QURL_PROT_FLAGS_WILDCARD        (1 << 12) /*!< The protocol supports wildcard matching*/
#define QURL_PROT_FLAGS_USERPWDCTRL     (1 << 13) /*!< Allow control characters (ASCII codes less than 32) in usernames and passwords*/

/* URL */
/**
 * @enum  qurl_url_e
 * @brief qurl information options
 */
typedef enum
{
    QURL_URL_OPT_URL = 0x3000, /*!< Complete URL*/
    QURL_URL_OPT_SCHEME,       /*!< Protocol part (such as http, https)*/
    QURL_URL_OPT_USER,         /*!< Username*/
    QURL_URL_OPT_PASSWORD,     /*!< Password*/
    QURL_URL_OPT_OPTIONS,      /*!< Options section (such as query parameters)*/
    QURL_URL_OPT_HOST,         /*!< Hostname*/
    QURL_URL_OPT_PORT,         /*!< Port number*/
    QURL_URL_OPT_PATH,         /*!< Path section*/
    QURL_URL_OPT_QUERY,        /*!< Query string*/
    QURL_URL_OPT_FRAGMENT,     /*!< Fragment identifier (position within the document)*/
    QURL_URL_OPT_ZONEID,       /*!< Region identifier*/
} qurl_url_opt_e;

/*Protocol-related enumerations*/
/**
 * @struct  qurl_http_version_e
 * @brief Supported HTTP versions
 */
typedef enum
{
    QURL_HTTP_VERSION_NONE,              /*!< Do not specify any HTTP version, let the library automatically select the optimal version*/
    QURL_HTTP_VERSION_1_0,               /*!< Use HTTP 1.0 protocol version in the request*/
    QURL_HTTP_VERSION_1_1,               /*!< Use HTTP 1.1 protocol version in the request*/
    QURL_HTTP_VERSION_2_0,               /*!< Use HTTP 2.0 protocol version in the request*/
    QURL_HTTP_VERSION_2TLS,              /*!< Use HTTP 2.0 for HTTPS, and HTTP 1.1 for HTTP*/
    QURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE, /*!< Do not use HTTP/1.1 upgrade, directly use HTTP 2.0 protocol version*/
    QURL_HTTP_VERSION_3,                 /*!< Explicitly use the HTTP/3 protocol version; servers supporting HTTP/3 will respond using this version.*/
    QURL_HTTP_VERSION_LAST               /*!< Illegal HTTP version*/
} qurl_http_version_e;
/**
 * @struct  qurl_http_method_e
 * @brief Supported HTTP request methods
 */
typedef enum
{
    QURL_HTTP_METHOD_NONE,      /*!< Not specified*/
    QURL_HTTP_METHOD_GET,       /*!< A request sent via the GET method allows the client to retrieve data for a specified resource from the server.*/
    QURL_HTTP_METHOD_POST,      /*!< A request sent via the POST method allows the client to submit data to the server, commonly used for form submissions or resource creation.*/
    QURL_HTTP_METHOD_POST_FORM, /*!< A special form of the POST method used internally. This option is used when processing form submissions with multiple form fields.*/
    QURL_HTTP_METHOD_POST_MIME, /*!< An alternative special form of the POST method used internally. This option is used when handling form submissions with complex MIME type data.*/
    QURL_HTTP_METHOD_PUT,      /*!< Through the PUT method, the client can upload data to a specified resource location on the server.*/
    QURL_HTTP_METHOD_PUT_FORM, /*!< A special form of the PUT method used internally. This option is used when handling form submissions with multiple form fields.*/
    QURL_HTTP_METHOD_PUT_MIME, /*!< An alternative special form of the PUT method used internally. This option is employed when handling form submissions with complex MIME type data.*/
    QURL_HTTP_METHOD_PATCH,      /*!< The request sent via the PATCH method, which complements the PUT method, is used for partial updates to known resources.*/
    QURL_HTTP_METHOD_PATCH_FORM, /*!< A special form of the PATCH method used internally. This option is used when processing form submissions with multiple form fields.*/
    QURL_HTTP_METHOD_PATCH_MIME, /*!< An alternative special form of the PATCH method used internally. This option is used when handling form submissions with complex MIME type data.*/
    QURL_HTTP_METHOD_HEAD, /*!< The request sent via the HEAD method allows the client to obtain metadata information of the resource without retrieving the actual data content.*/
    QURL_HTTP_METHOD_LAST  /*!< Unsupported*/
} qurl_http_method_e;

#endif /* Head define end*/
