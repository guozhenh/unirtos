/** @file         qurl_http_chunked.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-10-27 19:59:24
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#ifndef QURL_HTTP_CHUNKED_H
#define QURL_HTTP_CHUNKED_H

#include "qurl_config.h"
#include "qurl_code_in.h"
#include "qurl_log.h"
#include "qurl_def_in.h"

#if QURL_CFG_ENABLE_HTTP

/** Declare some data structures*/
struct qurl_conn_s;
typedef struct qurl_conn_s qurl_conn_t;

/**
 * The packet format is:
 * Data Length
 * Data
 */

/**
 * RFC2616 and subsequent HTTP specifications do not define a maximum chunk size for chunked transfer encoding.
 * However, resources are limited, so we use this macro to restrict the number of characters in the chunked transfer length field, thereby limiting the maximum length of each chunk.
 * As defined as 8, it means supporting 8 characters. The chunked length is in hexadecimal characters, so the maximum value can be: 0xFFFFFFFF (maximum for 32-bit systems)
 */
#define QURL_HTTP_CHUNKED_NUMLEN_MAX (8)

/**
 * @struct  qurl_http_chunked_state_e
 * @brief Chunked Transfer State Machine
 */
typedef enum
{
    /*!< Wait and buffer all hexadecimal digits until encountering a character that is not a hexadecimal digit. After completion, transition to the CHUNK_LF state.*/
    QURL_HTTP_CHUNKED_STATE_HEX,
    /*!< Wait for LF (line feed \n), ignore other characters.*/
    QURL_HTTP_CHUNKED_STATE_HEX_LF,
    /*!< Read the specified amount of data. After completion, enter the CHUNK_POSTCR state.*/
    QURL_HTTP_CHUNKED_STATE_DATA,
    /*!< Wait for the CR (carriage return \r) and LF (line feed \n) of the data field, then no other characters, then return to the CHUNK_HEX state. Missing CR is not a big issue, can still continue.*/
    QURL_HTTP_CHUNKED_STATE_DATA_LF,
    /*!< In this state, optional trailer header fields can be found, unless the next line is CRLF (carriage return + line feed \r\n).*/
    QURL_HTTP_CHUNKED_STATE_TRAILER,
    /*!< Indicates the end state. Note that there is a field named data_size in the structure, which will tell us the number of bytes in the last buffer that were not passed to the client.*/
    QURL_HTTP_CHUNKED_STATE_STOP,
} qurl_http_chunked_state_e;

/**
 * @struct  qurl_http_chunked_ctrl_t
 * @brief Data structure for HTTP chunked control
 */
struct qurl_http_chunked_ctrl_s
{
    qurl_http_chunked_state_e state;                                     /*!< Status*/
    long                      data_size;                                 /*!< Length of the parsed chunked block data*/
    unsigned char             hex_index;                                 /*!< Number of characters in the chunked block length field*/
    char                      hex_buf[QURL_HTTP_CHUNKED_NUMLEN_MAX + 1]; /*!< Save the chunked transfer length field*/
};
typedef struct qurl_http_chunked_ctrl_s qurl_http_chunked_ctrl_t;

void qurl_http_chunked_init(qurl_conn_t *conn_ptr);

qurl_ecode_t qurl_http_chunked_read_body(qurl_conn_t *conn_ptr);

#endif

#endif /* Head define end*/
