/*
 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *      An implementation of the Constrained Application Protocol (draft 12)
 * \author
 *      Matthias Kovatsch <kovatsch@inf.ethz.ch>
 * \contributors
 *    David Navarro, Intel Corporation - Adapt to usage in liblwm2m
 *    Tuve Nordius, Husqvarna Group - Please refer to git log
 */

#ifndef _COAP_13_H_
#define _COAP_13_H_

#include "qosa_def.h"
#include "qosa_sys.h"
#include <stdint.h>
#include <stddef.h> /* for qosa_size_t */

/*
 * Error code
 */

/** @brief No error, operation successful */
#define COAP_NO_ERROR                        (qosa_uint8_t)0x00
/** @brief Ignore current operation */
#define COAP_IGNORE                          (qosa_uint8_t)0x01
/** @brief Message retransmission required */
#define COAP_RETRANSMISSION                  (qosa_uint8_t)0x02

/**
 * @brief CoAP response codes
 */

#define COAP_CODE_EMPTY                      (qosa_uint8_t)0x0

#define COAP_201_CREATED                     (qosa_uint8_t)0x41
#define COAP_202_DELETED                     (qosa_uint8_t)0x42
#define COAP_204_CHANGED                     (qosa_uint8_t)0x44
#define COAP_205_CONTENT                     (qosa_uint8_t)0x45
#define COAP_231_CONTINUE                    (qosa_uint8_t)0x5F
#define COAP_400_BAD_REQUEST                 (qosa_uint8_t)0x80
#define COAP_401_UNAUTHORIZED                (qosa_uint8_t)0x81
#define COAP_402_BAD_OPTION                  (qosa_uint8_t)0x82
#define COAP_404_NOT_FOUND                   (qosa_uint8_t)0x84
#define COAP_405_METHOD_NOT_ALLOWED          (qosa_uint8_t)0x85
#define COAP_406_NOT_ACCEPTABLE              (qosa_uint8_t)0x86
#define COAP_408_REQ_ENTITY_INCOMPLETE       (qosa_uint8_t)0x88
#define COAP_412_PRECONDITION_FAILED         (qosa_uint8_t)0x8C
#define COAP_413_ENTITY_TOO_LARGE            (qosa_uint8_t)0x8D
#define COAP_415_UNSUPPORTED_CONTENT_FORMAT  (qosa_uint8_t)0x8F
#define COAP_500_INTERNAL_SERVER_ERROR       (qosa_uint8_t)0xA0
#define COAP_501_NOT_IMPLEMENTED             (qosa_uint8_t)0xA1
#define COAP_503_SERVICE_UNAVAILABLE         (qosa_uint8_t)0xA3

/** @brief Default Max-Age value */
#define COAP_DEFAULT_MAX_AGE                 60
/** @brief Response timeout (seconds) */
#define COAP_RESPONSE_TIMEOUT                2
/** @brief Maximum retransmission count */
#define COAP_MAX_RETRANSMIT                  4
/** @brief ACK random factor, used for backoff time calculation */
#define COAP_ACK_RANDOM_FACTOR               1.5
/** @brief Maximum network latency (milliseconds) */
#define COAP_MAX_LATENCY                     100
/** @brief Processing delay, equal to response timeout */
#define COAP_PROCESSING_DELAY                COAP_RESPONSE_TIMEOUT

/** @brief Maximum transmit wait time calculation formula */
#define COAP_MAX_TRANSMIT_WAIT               ((COAP_RESPONSE_TIMEOUT * ((1 << (COAP_MAX_RETRANSMIT + 1)) - 1) * COAP_ACK_RANDOM_FACTOR))
/** @brief Maximum transmit span time calculation formula */
#define COAP_MAX_TRANSMIT_SPAN               ((COAP_RESPONSE_TIMEOUT * ((1 << COAP_MAX_RETRANSMIT) - 1) * COAP_ACK_RANDOM_FACTOR))
/** @brief Exchange lifetime calculation formula */
#define COAP_EXCHANGE_LIFETIME               (COAP_MAX_TRANSMIT_SPAN + (2 * COAP_MAX_LATENCY) + COAP_PROCESSING_DELAY)

#define COAP_HEADER_LEN                      4 /* | version:0x03 type:0x0C tkl:0xF0 | code | mid:0x00FF | mid:0xFF00 | */
#define COAP_ETAG_LEN                        8 /* The maximum number of bytes for the ETag */
#define COAP_TOKEN_LEN                       8 /* The maximum number of bytes for the Token */
#define COAP_MAX_ACCEPT_NUM                  2 /* The maximum number of accept preferences to parse/store */

#define COAP_MAX_OPTION_HEADER_LEN           5

/** @brief CoAP header version mask */
#define COAP_HEADER_VERSION_MASK             0xC0
/** @brief CoAP header version position */
#define COAP_HEADER_VERSION_POSITION         6
/** @brief CoAP header message type mask */
#define COAP_HEADER_TYPE_MASK                0x30
/** @brief CoAP header message type position */
#define COAP_HEADER_TYPE_POSITION            4
/** @brief CoAP header token length mask */
#define COAP_HEADER_TOKEN_LEN_MASK           0x0F
/** @brief CoAP header token length position */
#define COAP_HEADER_TOKEN_LEN_POSITION       0

/** @brief CoAP option header delta mask */
#define COAP_HEADER_OPTION_DELTA_MASK        0xF0
/** @brief CoAP option header delta reserved value */
#define COAP_HEADER_OPTION_DELTA_RESERVED    0x0F
/** @brief CoAP option header short length mask */
#define COAP_HEADER_OPTION_SHORT_LENGTH_MASK 0x0F
/** @brief CoAP option header length reserved value */
#define COAP_HEADER_OPTION_LENGTH_RESERVED   0x0F

/* Bitmap for set options */
enum
{
    OPTION_MAP_SIZE = sizeof(qosa_uint8_t) * 8
};
#define SET_OPTION(packet, opt)                                                                                                                                \
    {                                                                                                                                                          \
        if (opt < sizeof((packet)->options) * OPTION_MAP_SIZE)                                                                                                 \
        {                                                                                                                                                      \
            (packet)->options[opt / OPTION_MAP_SIZE] |= 1 << (opt % OPTION_MAP_SIZE);                                                                          \
        }                                                                                                                                                      \
    }
#define IS_OPTION(packet, opt)                                                                                                                                 \
    ((opt < sizeof((packet)->options) * OPTION_MAP_SIZE) ? (packet)->options[opt / OPTION_MAP_SIZE] & (1 << (opt % OPTION_MAP_SIZE)) : 0)

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif /* MIN */

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif /* MAX */

/* CoAP message types */
typedef enum
{
    COAP_TYPE_CON, /*!< confirmables */
    COAP_TYPE_NON, /*!< non-confirmables */
    COAP_TYPE_ACK, /*!< acknowledgements */
    COAP_TYPE_RST  /*!< reset */
} coap_message_type_t;

/* CoAP request method codes */
typedef enum
{
    COAP_GET = 1, /*!< GET method, used to retrieve resources */
    COAP_POST,    /*!< POST method, used to create resources or submit data */
    COAP_PUT,     /*!< PUT method, used to update resources */
    COAP_DELETE   /*!< DELETE method, used to delete resources */
} coap_method_t;

#define COAP_EMPTY_MESSAGE_CODE 0x00

/* CoAP response codes */
typedef enum
{
    NO_ERROR = 0,

    CREATED_2_01 = 65,                   /*!< CREATED */
    DELETED_2_02 = 66,                   /*!< DELETED */
    VALID_2_03 = 67,                     /*!< NOT_MODIFIED */
    CHANGED_2_04 = 68,                   /*!< CHANGED */
    CONTENT_2_05 = 69,                   /*!< OK */

    BAD_REQUEST_4_00 = 128,              /*!< BAD_REQUEST */
    UNAUTHORIZED_4_01 = 129,             /*!< UNAUTHORIZED */
    BAD_OPTION_4_02 = 130,               /*!< BAD_OPTION */
    FORBIDDEN_4_03 = 131,                /*!< FORBIDDEN */
    NOT_FOUND_4_04 = 132,                /*!< NOT_FOUND */
    METHOD_NOT_ALLOWED_4_05 = 133,       /*!< METHOD_NOT_ALLOWED */
    NOT_ACCEPTABLE_4_06 = 134,           /*!< NOT_ACCEPTABLE */
    PRECONDITION_FAILED_4_12 = 140,      /*!< BAD_REQUEST */
    REQUEST_ENTITY_TOO_LARGE_4_13 = 141, /*!< REQUEST_ENTITY_TOO_LARGE */
    UNSUPPORTED_MEDIA_TYPE_4_15 = 143,   /*!< UNSUPPORTED_MEDIA_TYPE */

    INTERNAL_SERVER_ERROR_5_00 = 160,    /*!< INTERNAL_SERVER_ERROR */
    NOT_IMPLEMENTED_5_01 = 161,          /*!< NOT_IMPLEMENTED */
    BAD_GATEWAY_5_02 = 162,              /*!< BAD_GATEWAY */
    SERVICE_UNAVAILABLE_5_03 = 163,      /*!< SERVICE_UNAVAILABLE */
    GATEWAY_TIMEOUT_5_04 = 164,          /*!< GATEWAY_TIMEOUT */
    PROXYING_NOT_SUPPORTED_5_05 = 165,   /*!< PROXYING_NOT_SUPPORTED */

    /*!< Erbium errors */
    MEMORY_ALLOCATION_ERROR = 192, /*!< Memory allocation error */
    PACKET_SERIALIZATION_ERROR,    /*!< Packet serialization error */

    /*!< Erbium hooks */
    MANUAL_RESPONSE /*!< Manual response */

} coap_status_t;

/* CoAP header options */
typedef enum
{
    COAP_OPTION_IF_MATCH = 1,        /*!< 0-8 B */
    COAP_OPTION_URI_HOST = 3,        /*!< 1-255 B */
    COAP_OPTION_ETAG = 4,            /*!< 1-8 B */
    COAP_OPTION_IF_NONE_MATCH = 5,   /*!< 0 B */
    COAP_OPTION_OBSERVE = 6,         /*!< 0-3 B */
    COAP_OPTION_URI_PORT = 7,        /*!< 0-2 B */
    COAP_OPTION_LOCATION_PATH = 8,   /*!< 0-255 B */
    COAP_OPTION_URI_PATH = 11,       /*!< 0-255 B */
    COAP_OPTION_CONTENT_TYPE = 12,   /*!< 0-2 B */
    COAP_OPTION_MAX_AGE = 14,        /*!< 0-4 B */
    COAP_OPTION_URI_QUERY = 15,      /*!< 0-270 B */
    COAP_OPTION_ACCEPT = 17,         /*!< 0-2 B */
    COAP_OPTION_TOKEN = 19,          /*!< 1-8 B */
    COAP_OPTION_LOCATION_QUERY = 20, /*!< 1-270 B */
    COAP_OPTION_BLOCK2 = 23,         /*!< 1-3 B */
    COAP_OPTION_BLOCK1 = 27,         /*!< 1-3 B */
    COAP_OPTION_SIZE = 28,           /*!< 0-4 B */
    COAP_OPTION_PROXY_URI = 35,      /*!< 1-270 B */
    OPTION_MAX_VALUE = 0xFFFF
} coap_option_t;

/* CoAP Content-Types */
typedef enum
{
    TEXT_PLAIN = 0,
    TEXT_XML = 1, /*!< Indented types are not in the initial registry. */
    TEXT_CSV = 2,
    TEXT_HTML = 3,
    IMAGE_GIF = 21,
    IMAGE_JPEG = 22,
    IMAGE_PNG = 23,
    IMAGE_TIFF = 24,
    AUDIO_RAW = 25,
    VIDEO_RAW = 26,
    APPLICATION_LINK_FORMAT = 40,
    APPLICATION_XML = 41,
    APPLICATION_OCTET_STREAM = 42,
    APPLICATION_RDF_XML = 43,
    APPLICATION_SOAP_XML = 44,
    APPLICATION_ATOM_XML = 45,
    APPLICATION_XMPP_XML = 46,
    APPLICATION_EXI = 47,
    APPLICATION_FASTINFOSET = 48,
    APPLICATION_SOAP_FASTINFOSET = 49,
    APPLICATION_JSON = 50,
    APPLICATION_X_OBIX_BINARY = 51,
    CONTENT_MAX_VALUE = 0xFFFF
} coap_content_type_t;

/** @brief Multi-option linked list structure */
typedef struct _multi_option_t
{
    struct _multi_option_t *next;      /*!< Pointer to next multi-option */
    qosa_uint8_t            is_static; /*!< Flag indicating if option is static (1=static, 0=dynamic) */
    qosa_uint8_t            len;       /*!< Option data length */
    qosa_uint8_t           *data;      /*!< Option data pointer */
} multi_option_t;

/* Parsed message struct */
typedef struct
{
    qosa_uint8_t *buffer;        /*!< pointer to CoAP header / incoming packet buffer / memory to serialize packet */

    qosa_uint8_t        version; /*!<  CoAP version number*/
    coap_message_type_t type;    /*!<  Message type*/
    qosa_uint8_t        code;    /*!<  Request method or response code*/
    qosa_uint16_t       mid;     /*!<  Message ID*/

    /*!< Bitmap to check if option is set */
    qosa_uint8_t options[COAP_OPTION_PROXY_URI / OPTION_MAP_SIZE + 1];

    /*!< Parse options once and store; allows setting options in random order  */
    coap_content_type_t content_type;                /*!< Message entity data format */
    qosa_uint32_t       max_age;                     /*!< max_age option value */
    qosa_size_t         proxy_uri_len;               /*!< proxy_uri content length */
    qosa_uint8_t       *proxy_uri;                   /*!< Proxy server address */
    qosa_uint8_t        etag_len;                    /*!< etag content length */
    qosa_uint8_t        etag[COAP_ETAG_LEN];         /*!< etag option value */
    qosa_size_t         uri_host_len;                /*!< uri_host option length */
    qosa_uint8_t       *uri_host;                    /*!< Destination host address */
    multi_option_t     *location_path;               /*!< Resource path created by server */
    qosa_uint16_t       uri_port;                    /*!< Destination host port number */
    qosa_size_t         location_query_len;          /*!< location_query option length */
    qosa_uint8_t       *location_query;              /*!< Resource parameter value created by server */
    multi_option_t     *uri_path;                    /*!< Requested resource path */
    qosa_uint32_t       observe;                     /*!< Whether observe option is set */
    qosa_uint8_t        token_len;                   /*!< token option length */
    qosa_uint8_t        token[COAP_TOKEN_LEN];       /*!< Tag content, used for request and response confirmation */
    qosa_uint8_t        accept_num;                  /*!< Number of client accept types */
    qosa_uint16_t       accept[COAP_MAX_ACCEPT_NUM]; /*!< Client accept types */
    qosa_uint8_t        if_match_len;                /*!< if_match content length */
    qosa_uint8_t        if_match[COAP_ETAG_LEN];     /*!< if_match option value */
    qosa_uint32_t       block2_num;                  /*!< block2 block number */
    qosa_uint8_t        block2_more;                 /*!< Whether block2 has more blocks, 1=yes, 0=no */
    qosa_uint16_t       block2_size;                 /*!< block2 block size */
    qosa_uint32_t       block2_offset;               /*!< block2 block offset */
    qosa_uint32_t       block1_num;                  /*!< block1 block number */
    qosa_uint8_t        block1_more;                 /*!< Whether block1 has more blocks, 1=yes, 0=no */
    qosa_uint16_t       block1_size;                 /*!< block1 block size */
    qosa_uint32_t       block1_offset;               /*!< block1 block offset */
    qosa_uint32_t       size;                        /*!< Maximum request entity length that server can handle */
    multi_option_t     *uri_query;                   /*!< Requested resource parameters */
    qosa_uint8_t        if_none_match;               /*!< Whether if_none_match option is set */
    qosa_size_t         payload_len;                 /*!< Payload length */
    qosa_uint8_t       *payload;                     /*!< Payload */

} coap_packet_t;

// clang-format off

/* Option format serialization*/
#define COAP_SERIALIZE_INT_OPTION(number, field, text)                                                                                                         \
    if (IS_OPTION(coap_pkt, number))                                                                                                                           \
    {                                                                                                                                                          \
        PRINTF(text " [%u]\n", coap_pkt->field);                                                                                                               \
        option += coap_serialize_int_option(number, current_number, option, coap_pkt->field);                                                                  \
        current_number = number;                                                                                                                               \
    }
#define COAP_SERIALIZE_BYTE_OPTION(number, field, text)                                                                                                        \
    if (IS_OPTION(coap_pkt, number))                                                                                                                           \
    {                                                                                                                                                          \
        PRINTF(                                                                                                                                                \
            text " %u [0x%02X%02X%02X%02X%02X%02X%02X%02X]\n",                                                                                                 \
            coap_pkt->field##_len,                                                                                                                             \
            coap_pkt->field[0],                                                                                                                                \
            coap_pkt->field[1],                                                                                                                                \
            coap_pkt->field[2],                                                                                                                                \
            coap_pkt->field[3],                                                                                                                                \
            coap_pkt->field[4],                                                                                                                                \
            coap_pkt->field[5],                                                                                                                                \
            coap_pkt->field[6],                                                                                                                                \
            coap_pkt->field[7]                                                                                                                                 \
        ); /*FIXME always prints 8 bytes */                                                                                                                    \
        option += coap_serialize_array_option(number, current_number, option, coap_pkt->field, coap_pkt->field##_len, '\0');                                   \
        current_number = number;                                                                                                                               \
    }
#define COAP_SERIALIZE_STRING_OPTION(number, field, splitter, text)                                                                                            \
    if (IS_OPTION(coap_pkt, number))                                                                                                                           \
    {                                                                                                                                                          \
        PRINTF(text " [%.*s]\n", coap_pkt->field##_len, coap_pkt->field);                                                                                      \
        option += coap_serialize_array_option(number, current_number, option, (qosa_uint8_t *)coap_pkt->field, coap_pkt->field##_len, splitter);                \
        current_number = number;                                                                                                                               \
    }
#define COAP_SERIALIZE_MULTI_OPTION(number, field, text)                                                                                                       \
    if (IS_OPTION(coap_pkt, number))                                                                                                                           \
    {                                                                                                                                                          \
        PRINTF(text);                                                                                                                                          \
        option += coap_serialize_multi_option(number, current_number, option, coap_pkt->field);                                                                \
        current_number = number;                                                                                                                               \
    }
#define COAP_SERIALIZE_ACCEPT_OPTION(number, field, text)                                                                                                      \
    if (IS_OPTION(coap_pkt, number))                                                                                                                           \
    {                                                                                                                                                          \
        int i;                                                                                                                                                 \
        for (i = 0; i < coap_pkt->field##_num; ++i)                                                                                                            \
        {                                                                                                                                                      \
            PRINTF(text " [%u]\n", coap_pkt->field[i]);                                                                                                        \
            option += coap_serialize_int_option(number, current_number, option, coap_pkt->field[i]);                                                           \
            current_number = number;                                                                                                                           \
        }                                                                                                                                                      \
    }
#define COAP_SERIALIZE_BLOCK_OPTION(number, field, text)                                                                                                       \
    if (IS_OPTION(coap_pkt, number))                                                                                                                           \
    {                                                                                                                                                          \
        qosa_uint32_t block = coap_pkt->field##_num << 4;                                                                                                       \
        PRINTF(text " [%lu%s (%u B/blk)]\n", coap_pkt->field##_num, coap_pkt->field##_more ? "+" : "", coap_pkt->field##_size);                                \
        if (coap_pkt->field##_more)                                                                                                                            \
            block |= 0x8;                                                                                                                                      \
        block |= 0xF & qcm_coap_log_2(coap_pkt->field##_size / 16);                                                                                                \
        PRINTF(text " encoded: 0x%lX\n", block);                                                                                                               \
        option += coap_serialize_int_option(number, current_number, option, block);                                                                            \
        current_number = number;                                                                                                                               \
    }
// clang-format on

/* To store error code and human-readable payload */
extern const char *qcm_coap_error_message;

qosa_uint16_t qcm_coap_log_2(qosa_uint16_t value);

/**
 * @brief Get message sequence number value
 *
 * @return qosa_uint16_t
 *       - Returns mid value
 */
qosa_uint16_t qcm_coap_get_mid(void);

/**
 * @brief Message initialization
 *
 * @param[out] void * packet
 *          - Message data packet
 * @param[in] coap_message_type_t type
 *          - Message type
 * @param[in] qosa_uint8_t code
 *          - Request method
 * @param[in] qosa_uint16_t mid
 *          - Message sequence number
 */
void qcm_coap_init_message(void *packet, coap_message_type_t type, qosa_uint8_t code, qosa_uint16_t mid);

/**
 * @brief Calculate and return the byte length required for message serialization
 *
 * @param[in] void * packet
 *          - Message data packet
 * @return qosa_size_t
 *       - Returns calculated length
 */
qosa_size_t qcm_coap_serialize_get_size(void *packet);

/**
 * @brief Serialize message into byte stream
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[out] qosa_uint8_t * buffer
 *          - Byte stream data storage
 * 
 * @return qosa_size_t
 *       - Returns converted byte length
 */
qosa_size_t qcm_coap_serialize_message(void *packet, qosa_uint8_t *buffer);

/**
 * @brief Parse received message data
 *
 * @param[out] void * request
 *          - Message data packet
 * @param[in] qosa_uint8_t * data
 *          - Message data
 * @param[in] qosa_uint16_t data_len
 *          - Message data length
 * 
 * @return coap_status_t
 *       - Returns NO_ERROR on success
 *       - Returns error code on failure
 */
coap_status_t qcm_coap_parse_message(void *request, qosa_uint8_t *data, qosa_uint16_t data_len);

/**
 * @brief Free resources used by message
 *
 * @param[in] void * packet
 *          - Message data packet
 */
void qcm_coap_free_header(void *packet);

/**
 * @brief Extract path string
 *
 * @param[in] multi_option_t * option
 *          - Message option
 * 
 * @return char *
 *       - Returns obtained path string on success
 *       - Returns QOSA_NULL on failure
 */
char *qcm_coap_get_multi_option_as_path_string(multi_option_t *option);

/**
 * @brief Extract resource parameter string from uri-query option
 *
 * @param[in] multi_option_t * option
 *          - Message option
 * 
 * @return char *
 *       - Returns obtained resource string on success
 *       - Returns QOSA_NULL on failure
 */
char *qcm_coap_get_multi_option_as_query_string(multi_option_t *option);

/**
 * @brief Extract uri option string from packet
 *
 * @param[in] coap_packet_t * packet
 *          - Message data packet
 * 
 * @return char *
 *       - Returns obtained uri string on success
 *       - Returns QOSA_NULL on failure
 */
char *qcm_coap_get_packet_uri_as_string(coap_packet_t *packet);

/**
 * @brief Add an option
 *
 * @param[in,out] multi_option_t ** dst
 *          - Pointer variable to target option
 * @param[in] qosa_uint8_t * option
 *          - Data to add
 * @param[in] qosa_size_t option_len
 *          - Data length to add
 * @param[in] qosa_uint8_t is_static
 *          - Flag indicating if option is static
 */
void qcm_coap_add_multi_option(multi_option_t **dst, qosa_uint8_t *option, qosa_size_t option_len, qosa_uint8_t is_static);

/**
 * @brief Free option resources
 *
 * @param[in] multi_option_t * dst
 *          - Option to free
 */
void qcm_free_multi_option(multi_option_t *dst);

/**
 * @brief Get query variable value (currently not implemented)
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] const char * name
 *          - Variable name
 * @param[out] const char ** output
 *          - Pointer to store variable value
 * 
 * @return int
 *       - Returns 0 (currently not implemented)
 */
int qcm_coap_get_query_variable(void *packet, const char *name, const char **output);

/**
 * @brief Get variable value with specified name from POST request body
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] const char * name
 *          - Variable name to get
 * @param[out] const char **output
 *          - Pointer to save variable value
 * @return char *
 *       - Returns string length on success
 *       - Returns 0 on failure
 */
int qcm_coap_get_post_variable(void *packet, const char *name, const char **output);

/*-----------------------------------------------------------------------------------*/

/**
 * @brief Set status code
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] unsigned int code
 *          - Status code
 * 
 * @return int
 *       - Returns 1 on success
 *       - Returns 0 on failure
 */
int qcm_coap_set_status_code(void *packet, unsigned int code);

/**
 * @brief Get payload data format
 *
 * @param[in] void * packet
 *          - Message data packet
 * 
 * @return int
 *       - Returns payload data type on success
 *       - Returns -1 on failure
 */
int qcm_coap_get_header_content_type(void *packet);

/**
 * @brief Set payload data format
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] unsigned int content_type
 *          - Data format
 * @return int
 *       - Returns 1
 */
int qcm_coap_set_header_content_type(void *packet, unsigned int content_type);

/**
 * @brief Get data formats that client can accept
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[out] const qosa_uint16_t ** accept
 *          - Store obtained data formats
 * 
 * @return int
 *       - Returns number of saved data formats on success
 *       - Returns 0 on failure
 */
int qcm_coap_get_header_accept(void *packet, const qosa_uint16_t **accept);

/**
 * @brief Set data formats that client can accept
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] const qosa_uint16_t accept
 *          - Data format value
 * 
 * @return int
 *       - Returns number of saved data formats
 */
int qcm_coap_set_header_accept(void *packet, qosa_uint16_t accept);

/**
 * @brief Get Max-Age value
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[out] qosa_uint32_t * age
 *          - Store obtained value
 * 
 * @return int
 *       - Returns 1
 */
int qcm_coap_get_header_max_age(void *packet, qosa_uint32_t *age);

/**
 * @brief Set Max-Age value
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] qosa_uint32_t age
 *          - Max-Age value
 * 
 * @return int
 *       - Returns 1
 */
int qcm_coap_set_header_max_age(void *packet, qosa_uint32_t age);

/**
 * @brief Get ETag value
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[out] const qosa_uint8_t **etag
 *          - Store obtained value
 * 
 * @return int
 *       - Returns 1
 */
int qcm_coap_get_header_etag(void *packet, const qosa_uint8_t **etag);

/**
 * @brief Set ETag value
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] const qosa_uint8_t *etag
 *          - ETag value
 * @param[in] qosa_size_t etag_len
 *          - ETag value length
 * 
 * @return int
 *      - Returns 1
 */
int qcm_coap_set_header_etag(void *packet, const qosa_uint8_t *etag, qosa_size_t etag_len);

/**
 * @brief Get If-Match value
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[out] const qosa_uint8_t **etag
 *          - Store obtained value
 * 
 * @return int
 *       - Returns If-Match value length on success
 *       - Returns 0 on failure
 */
int qcm_coap_get_header_if_match(void *packet, const qosa_uint8_t **etag);

/**
 * @brief Set If-Match value
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] const qosa_uint8_t *etag
 *          - If-Match value
 * @param[in] qosa_size_t etag_len
 *          - If-Match value length
 * 
 * @return int
 *       - Returns If-Match value length
 */
int qcm_coap_set_header_if_match(void *packet, const qosa_uint8_t *etag, qosa_size_t etag_len);

/**
 * @brief Check if If-None-Match option is set
 *
 * @param[in] void * packet
 *          - Message data packet
 * 
 * @return int
 *       - Returns 1 if If-None-Match option is set
 *       - Returns 0 if If-None-Match option is not set
 */
int qcm_coap_get_header_if_none_match(void *packet);

/**
 * @brief Set If-None-Match
 *
 * @param[in] void * packet
 *          - Message data packet
 * 
 * @return int
 *       - Returns 1
 */
int qcm_coap_set_header_if_none_match(void *packet);

/**
 * @brief Get token value
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[out] qosa_uint8_t ** token
 *          - Store obtained value
 * 
 * @return int
 *       - Returns token length
 */
int qcm_coap_get_header_token(void *packet, qosa_uint8_t **token);

/**
 * @brief Set token value
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] const qosa_uint8_t *token
 *          - token value
 * @param[in] qosa_size_t token_len
 *          - token value length
 * 
 * @return int
 *       - Returns token length
 */
int qcm_coap_set_header_token(void *packet, const qosa_uint8_t *token, qosa_size_t token_len);

/**
 * @brief Get proxy URI value
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[out] const char **uri
 *          - Store obtained value
 * 
 * @return int
 *       - Returns proxy URI length on success
 *       - Returns 0 on failure
 */
int qcm_coap_get_header_proxy_uri(void *packet, const char **uri); /* In-place string might not be 0-terminated. */

/**
 * @brief Set proxy URI value
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] const char * uri
 *          - Proxy URI value
 * 
 * @return int
 *       - Returns proxy URI length
 */
int qcm_coap_set_header_proxy_uri(void *packet, const char *uri);

/**
 * @brief Get destination host address
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[out] const char ** host
 *          - Store obtained value
 * 
 * @return int
 *       - Returns host address length on success
 *       - Returns 0 on failure
 */
int qcm_coap_get_header_uri_host(void *packet, const char **host); /* In-place string might not be 0-terminated. */

/**
 * @brief Set destination host address
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] const char * host
 *          - Destination host address
 * 
 * @return int
 *       - Returns host address length
 */
int qcm_coap_set_header_uri_host(void *packet, const char *host);

/** Not implemented
 * @brief Get absolute resource path
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[out] const char ** path
 *          - Store obtained value
 * 
 * @return int
 *       - Returns resource path length on success
 *       - Returns 0 on failure
 */
int qcm_coap_get_header_uri_path(void *packet, char **path); /* In-place string might not be 0-terminated. */

/**
 * @brief Set absolute resource path, only need to call once to set a complete path option
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] const char * path
 *          - Resource path value
 * 
 * @return int
 *       - Returns resource path length
 */
int qcm_coap_set_header_uri_path(void *packet, const char *path);

/**
 * @brief Set absolute resource path, call this interface multiple times, each time setting a path segment, will exist as multiple uri_path options
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] const char * path
 *          - Resource path value
 * 
 * @return int
 *       - Returns resource path length on success
 *       - Returns 0 on failure
 */
int qcm_coap_set_header_uri_path_segment(void *packet, const char *path);

/** 
 * @brief Get destination host port number
 *
 * @param[in] void * packet
 *          - Message data packet
 * 
 * @return int
 *       - Returns port number on success
 *       - Returns 0 on failure
 */
qosa_uint16_t qcm_coap_get_header_uri_port(void *packet);

/**
 * @brief Set destination host port number
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] qosa_uint16_t port
 *          - Port number
 */
void qcm_coap_set_header_uri_port(void *packet, qosa_uint16_t port);

/** Not implemented
 * @brief Get resource parameters
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[out] const char ** query
 *          - Store obtained value
 * 
 * @return int
 *       - Returns resource parameter length on success
 *       - Returns 0 on failure
 */
int qcm_coap_get_header_uri_query(void *packet, char **query); /* In-place string might not be 0-terminated. */

/**
 * @brief Set resource parameters
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] const char * query
 *          - Resource parameters
 * 
 * @return int
 *       - Returns resource parameter length
 */
int qcm_coap_set_header_uri_query(void *packet, const char *query);

/**
 * @brief Append a new resource parameter
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] const char * segment
 *          - Resource path value
 * 
 * @return int
 *       - Returns resource parameter length
 */
int qcm_coap_set_header_uri_query_segment(void *packet, const char *segment);

/** Not implemented
 * @brief Get location-path value
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[out] const char ** path
 *          - Store obtained value
 * 
 * @return int
 *       - Returns location-path length on success
 *       - Returns 0 on failure
 */
int qcm_coap_get_header_location_path(void *packet, const char **path); /* In-place string might not be 0-terminated. */

/**
 * @brief Set location-path value
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] const char * path
 *          - location-path value
 * 
 * @return int
 *       - Returns location-path length
 */
int qcm_coap_set_header_location_path(void *packet, const char *path); /* Also splits optional query into Location-Query option. */

/**
 * @brief Get location-query value
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[out] const char ** query
 *          - Store obtained value
 * 
 * @return int
 *       - Returns location-query length on success
 *       - Returns 0 on failure
 */
int qcm_coap_get_header_location_query(void *packet, const char **query); /* In-place string might not be 0-terminated. */

/**
 * @brief Set location-query value
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] const char * query
 *          - location-query value
 * 
 * @return int
 *       - Returns location-query length
 */
int qcm_coap_set_header_location_query(void *packet, char *query);

/**
 * @brief Get observe value
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[out] qosa_uint32_t * observe
 *          - Store obtained value
 * 
 * @return int
 *       - Returns 1 on success
 *       - Returns 0 on failure
 */
int qcm_coap_get_header_observe(void *packet, qosa_uint32_t *observe);

/**
 * @brief Set observe value
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] qosa_uint32_t observe
 *          - observe value
 * 
 * @return int
 *       - Returns 1 on success
 *       - Returns 0 on failure
 */
int qcm_coap_set_header_observe(void *packet, qosa_uint32_t observe);

/**
 * @brief Get block2 value, used for block transfer of response data
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[out] qosa_uint32_t * num
 *          - Current block number
 * @param[out] qosa_uint8_t *more
 *          - Indicates whether there are more blocks. 1=yes, 0=no.
 * @param[out] qosa_uint16_t *size
 *          - Current block size
 * @param[out] qosa_uint32_t * offset
 *          - Offset of current block within the entire data block
 * 
 * @return int
 *       - Returns 1 on success
 *       - Returns 0 on failure
 */
int qcm_coap_get_header_block2(void *packet, qosa_uint32_t *num, qosa_uint8_t *more, qosa_uint16_t *size, qosa_uint32_t *offset);

/**
 * @brief Set block2 value, used for block transfer of response data
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] qosa_uint32_t num
 *          - Set current block number
 * @param[in] qosa_uint8_t more
 *          - Set whether there are more blocks. 1=yes, 0=no.
 * @param[in] qosa_uint16_t size
 *          - Set current block size
 * 
 * @return int
 *       - Returns 1 on success
 *       - Returns 0 on failure
 */
int qcm_coap_set_header_block2(void *packet, qosa_uint32_t num, qosa_uint8_t more, qosa_uint16_t size);

/**
 * @brief Get block1 value, used for block transfer of request data
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[out] qosa_uint32_t * num
 *          - Current block number
 * @param[out] qosa_uint8_t *more
 *          - Indicates whether there are more blocks. 1=yes, 0=no.
 * @param[out] qosa_uint16_t *size
 *          - Current block size
 * @param[out] qosa_uint32_t * offset
 *          - Offset of current block within the entire data block
 * 
 * @return int
 *       - Returns 1 on success
 *       - Returns 0 on failure
 */
int qcm_coap_get_header_block1(void *packet, qosa_uint32_t *num, qosa_uint8_t *more, qosa_uint16_t *size, qosa_uint32_t *offset);

/**
 * @brief Set block1 value, used for block transfer of request data
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] qosa_uint32_t num
 *          - Set current block number
 * @param[in] qosa_uint8_t more
 *          - Set whether there are more blocks. 1=yes, 0=no.
 * @param[in] qosa_uint16_t size
 *          - Set current block size
 * 
 * @return int
 *       - Returns 1 on success
 *       - Returns 0 on failure
 */
int qcm_coap_set_header_block1(void *packet, qosa_uint32_t num, qosa_uint8_t more, qosa_uint16_t size);

/**
 * @brief Get block value, used for block transfer of request data
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[out] qosa_uint32_t * num
 *          - Current block number
 * @param[out] qosa_uint8_t *more
 *          - Indicates whether there are more blocks. 1=yes, 0=no.
 * @param[out] qosa_uint16_t *size
 *          - Current block size
 * @param[out] qosa_uint32_t * offset
 *          - Offset of current block within the entire data block
 * 
 * @return int
 *       - Returns 1 on success
 *       - Returns 0 on failure
 */
int qcm_coap_get_header_block(void *packet, qosa_uint32_t *num, qosa_uint8_t *more, qosa_uint16_t *size, qosa_uint32_t *offset);

/**
 * @brief Get maximum request entity length that server can handle
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] qosa_uint16_t * size
 *          - Length value
 * 
 * @return int
 *       - Returns 1 on success
 *       - Returns 0 on failure
 */
int qcm_coap_get_header_size(void *packet, qosa_uint32_t *size);

/**
 * @brief Set maximum request entity length that server can handle
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] qosa_uint16_t size
 *          - Length value
 * 
 * @return int
 *       - Returns 1
 */
int qcm_coap_set_header_size(void *packet, qosa_uint32_t size);

/**
 * @brief Get payload content
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[out] const qosa_uint8_t **payload
 *          - Store payload content
 * 
 * @return int
 *       - Returns payload length on success
 *       - Returns 0 on failure
 */
qosa_size_t qcm_coap_get_payload(void *packet, const qosa_uint8_t **payload);

/**
 * @brief Set payload content
 *
 * @param[in] void * packet
 *          - Message data packet
 * @param[in] const void *payload
 *          - Payload content
 * @param[in] qosa_size_t length
 *          - Payload length
 * 
 * @return int
 *       - Returns payload length
 */
qosa_size_t qcm_coap_set_payload(void *packet, const void *payload, qosa_size_t length);

#endif /* _COAP_13_H_ */
