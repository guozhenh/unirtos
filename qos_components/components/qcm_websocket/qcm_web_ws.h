/*****************************************************************/ /**
* @file qcm_web_ws.h
* @brief
* @author harry.li@quectel.com
* @date 2024-04-25
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-04-25 <td>1.0 <td>harry.Li <td> Init
* </table>
**********************************************************************/

#ifndef __QCM_WEB_WS_H__
#define __QCM_WEB_WS_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qcm_websocket.h"
#include "qosa_queue_list.h"
#include "qosa_watermark.h"

/**
 * WebSocket Protocol Type Enumeration
 * Used to specify the protocol type for WebSocket connections, including unencrypted WebSocket (WS) and encrypted WebSocket (WSS).
 */
typedef enum
{
    WS_SCHEME_WS, /*!< Non-encrypted WebSocket protocol*/
    WS_SCHEME_WSS /*!< Encrypted WebSocket Protocol*/
} web_socket_scheme;

/**
 * Define WebSocket parsing type enumeration
 * Used to identify different stages during WebSocket data parsing
 */
typedef enum
{
    WS_PARSE_TYPE_HEADER = 0, /*!< Parse the frame header section*/
    WS_PARSE_TYPE_BODY,       /*!< Parse data section*/
} web_parse_type_e;

/**
 * Define the opcode enumeration for WebSocket frames.
 * Each opcode represents a frame type in WebSocket communication.
 */
typedef enum
{
    OPCODE_CONTINUATION = 0x0, /*!< Continuation frame opcode, used for identifying multi-frame messages*/
    OPCODE_TEXT = 0x1,         /*!< Text frame opcode, indicating the frame content is text data*/
    OPCODE_BINARY = 0x2,       /*!< Binary frame opcode, indicating the frame content is binary data*/
    OPCODE_CLOSE = 0x8,        /*!< Close frame opcode, used to terminate a WebSocket connection*/
    OPCODE_PING = 0x9,         /*!< Heartbeat frame opcode, used for WebSocket connection heartbeat detection*/
    OPCODE_PONG = 0xA,         /*!< Response frame opcode, used for pong messages in response to PING frames*/
} web_opcode_e;

/**
 * WebSocketUrl Structure Definition
 * For storing URL information of WebSocket connections
 */
typedef struct
{
    web_socket_scheme scheme; /*!< Protocol type, such as ws or wss*/
    char             *host;   /*!< Hostname or IP address*/
    int               port;   /*!< Port number*/
    char             *path;   /*!< Request path*/
} web_socket_url;

/**
 * Structure definition of WebSocket frame header
 * For parsing and constructing WebSocket protocol data frame headers
 */
typedef struct
{
    qosa_uint8_t FIN         : 1; /*!< Control bit, indicating this is the last segment of the data frame*/
    qosa_uint8_t RSV         : 3; /*!< Reserved bit, currently unused*/
    qosa_uint8_t opcode      : 4; /*!< Opcode, defines the data type of the frame*/
    qosa_uint8_t MASK        : 1; /*!< Flag indicating whether the data is masked*/
    qosa_uint8_t payload_len : 7; /*!< Payload length, indicating the effective payload length of the data frame*/
} web_socket_header_t;

/**
 * WebSocket packet structure definition
 * This structure is used to represent a WebSocket frame, containing all the necessary components of the frame.
 */
typedef struct
{
    //There are multiple frames in the data stream, always record the information of the last websocket frame among them.
    web_socket_header_t header;         /*!< WebSocket frame header, containing basic information of the frame, such as opcode, whether it is masked, etc.*/
    int                 header_len;     /*!< Frame header length, used to indicate the number of bytes occupied by the frame header.*/
    int                 masking_key[4]; /*!< Mask key, used for decoding the data part. This field is valid only when the MASK bit in the frame header is 1.*/
    int                 data_len;       /*!< Data length, indicating the number of bytes occupied by the data portion of the frame.*/
    int                 read_len;       /*!< The length of data already read, used to indicate the amount of data that has been read during the parsing process.*/
    int                 write_len;      /*!< The length of data that has been written, similar in purpose to read_len, but used during data transmission.*/
    web_parse_type_e    parse_type;     /*!< Parsing type, used to indicate which stage the current frame is in during parsing or what type of frame it is.*/

    //Record the data of the last frame header in the data stream.
    qosa_bool_t header_break; /*!< Record whether the websocket frame header data is complete (whether the frame header is interrupted; TRUE: frame header interrupted, FALSE: frame header complete)*/
    char *header_data[15]; /*!< If the WebSocket frame header data is incomplete, record the portion already read this time, and concatenate it with the next data stream to attempt to form a complete frame header. (The maximum length of the frame header is 14 bytes.)*/
    int header_data_len;   /*!< Record the length of the read websocket frame header*/

    /** Records of key control frames (ping, close, undefined frames) in multi-frame transmissions **/

    //There are multiple frames in the data stream, which include ping frames. Record the content of the first ping frame parsed in the data stream (refer to the libwebsocket implementation).
    qosa_bool_t ping;      /*!< Multiple frames exist ping frame flag*/
    char       *ping_data; /*!< Used to store the data portion of a PING or PONG frame.*/
    int         ping_len;  /*!< Stores the ping-pang data length, i.e., the data length of a PING or PONG frame.*/

    //There are multiple frames in the data stream, which include a close frame or other undefined control frame types.
    //or these control frames do not meet the requirements of RFC 6455
    //In these cases, refer to libwebsocket for handling the closure.
    qosa_bool_t close; /*!< Close tag*/
} web_socket_frame_t;

typedef struct
{
    qosa_q_link_type_t list;       /*!< Linked list node, used for linked list management*/
    web_opcode_e       opcode;     /*!< Frame operation type*/
    int                header_len; /*!< In ws_construct_frame, considered as the frame header length for a new frame;
In qcm_ws_get_send_payload_total_size, it is considered how much data of the current frame's header has not been sent out via send (the remaining data amount of the frame's header for this frame)*/
    int                data_len;   /*!< In ws_construct_frame, considered as the payload length of a new frame;
In qcm_ws_get_send_payload_total_size, it is considered how much data of the current frame's payload has not yet been sent out via send (the remaining data amount of this frame's payload)*/
} web_socket_s_frame_t;

typedef struct
{
    qosa_bool_t             rw_wm_ready;           /*!< Watermark initialization flag*/
    qosa_uint32_t           current_recv_cnt;      /*!< Data size after each recv*/
    qosa_bool_t             send_wm_high;          /*!< send water high status flag*/
    qosa_buffer_block_t    *current_send_data_ptr; /*!< Items already retrieved from the Watermark*/
    qosa_buffer_watermark_t send_wm_ptr;           /*!< send watermark*/
    qosa_q_type_t           send_q;                /*!< watermark internal management queue*/
    qosa_bool_t             recv_wm_high;          /*!< recv water high status flag*/
    qosa_buffer_block_t    *current_recv_data_ptr; /*!< Items already retrieved from the Watermark*/
    qosa_buffer_watermark_t recv_wm_ptr;           /*!< recv send watermark*/
    qosa_q_type_t           recv_q;                /*!< watermark internal management queue*/
} web_transfer_session;

/**
 * @brief Parse WebSocket URL.
 *
 * @param input_url The input WebSocket URL string.
 * @param parsed_url The parsed WebSocket URL structure.
 * @return Returns the parsing result, success is OSA_WS_SUCCESS, failure is other error codes.
 */
qcm_web_err_e qcm_ws_parse_url(const char *input_url, web_socket_url *parsed_url);

/**
 * @brief Parse WebSocket frames.
 *
 * @param client_id Client ID, used to identify a specific WebSocket connection.
 * @param src The original frame data input.
 * @param src_len The length of the original frame data input.
 * @param dst The output buffer for parsed frame data.
 * @param dst_len Pointer to the length of the parsed frame data; after the function execution completes, it will be updated to the actual output length.
 * @return Returns the processing result: a positive number indicates success, representing the length of the parsed frame data; a negative number indicates failure, representing an error code.
 */
int qcm_ws_parse_frame(int client_id, char *src, int src_len, char *dst, int *dst_len);

/**
 * @brief Construct a WebSocket frame.
 *
 * @param client_id Client ID, used to identify a specific WebSocket connection.
 * @param opcode, identifies the type of frame.
 * @param src The input data source.
 * @param src_len The length of the input data source.
 * @param dst The output buffer for the constructed frame data.
 * @param dst_len Pointer to the length of the constructed frame data, which will be updated to the actual output length after the function execution.
 * @return Returns the construction result; a positive value indicates success, representing the length of the constructed frame data; a negative value indicates failure, representing an error code.
 */
int qcm_ws_construct_frame(int client_id, web_opcode_e opcode, char *src, int src_len, char *dst, int *dst_len);

#endif /* __QCM_WEB_WS_H__ */
