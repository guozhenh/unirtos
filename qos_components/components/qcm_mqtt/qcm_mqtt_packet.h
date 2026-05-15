/*******************************************************************************
 *
 * THIRD-PARTY NOTICE
 *
 * Portions of this file are derived from Eclipse Paho MQTT Embedded C
 * (MQTTPacket code line, package version 1.1.0).
 *
 * Original work:
 *   Copyright (c) 2014 IBM Corp.
 *
 * The Paho-derived portions are redistributed by Quectel under the
 * Eclipse Distribution License v1.0.
 *
 * See THIRD_PARTY_NOTICES.md and licenses/ for details.
 *
 ******************************************************************************/
/*****************************************************************/ /**
* @file qcm_mqtt_packet.h
* @brief
* @author larson.li@quectel.com
* @date 2023-12-07
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2023-12-07 <td>1.0 <td>Larson.Li <td> Init
* <tr><td>2025-06-30 <td>1.1 <td>Lawrence.liu <td> Add MQTT5 properties support
* </table>
**********************************************************************/

#ifndef __QCM_MQTT_PACKET_H__
#define __QCM_MQTT_PACKET_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_queue_list.h"
#include "qcm_mqtt_config.h"
#include "qcm_mqtt_private.h"
#include "qcm_mqtt.h"

/** Length of the MQTT packet header.*/
#define MQTT_PACKET_HEAD_LEN          2
/** Length of MQTT packet ID.*/
#define MQTT_PACKET_ID_LEN            2
/** Length of the optional field length in an MQTT packet.*/
#define MQTT_PACKET_LENGTH_HEAD_LEN   2
/** The length of the QOS field in MQTT SUBSCRIBE and SUBSCRIBE packets.*/
#define MQTT_SUBSCRIBE_PACKET_QOS_LEN 1

typedef enum
{
    MQTTPACKET_BUFFER_TOO_SHORT = -2,
    MQTTPACKET_READ_ERROR = -1, /*!< MQTT unpacking failed*/
    MQTTPACKET_READ_COMPLETE,   /*!< MQTT unpacking successful*/
} mqtt_packet_errors;

/**
 * @enum mqtt_msg_types.
 * @brief Control type field in MQTT protocol packet assembly.
 */
typedef enum
{
    MQTT_CONNECT = 1,      /** The control type in the MQTT protocol packet assembly is CONNECT.*/
    MQTT_CONNACK = 2,      /** The control type in the MQTT protocol packet assembly is CONNACK.*/
    MQTT_PUBLISH = 3,      /** The control type in the MQTT protocol packet assembly is PUBLISH.*/
    MQTT_PUBACK = 4,       /** The control type in the MQTT protocol packet assembly is PUBACK.*/
    MQTT_PUBREC = 5,       /** In the MQTT protocol packet assembly, the control type is PUBREC.*/
    MQTT_PUBREL = 6,       /** The control packet type in the MQTT protocol packaging is PUBREL.*/
    MQTT_PUBCOMP = 7,      /** In the MQTT protocol packet assembly, the control type is PUBCOMP.*/
    MQTT_SUBSCRIBE = 8,    /** The control type in the MQTT protocol packet assembly is SUBSCRIBE.*/
    MQTT_SUBACK = 9,       /** In the MQTT protocol packet assembly, the control type is SUBACK.*/
    MQTT_UNSUBSCRIBE = 10, /** The control type in the MQTT protocol packet assembly is UNSUBSCRIBE.*/
    MQTT_UNSUBACK = 11,    /** The control type in the MQTT protocol packet assembly is UNSUBACK.*/
    MQTT_PINGREQ = 12,     /** In the MQTT protocol packet assembly, the control type is PINGREQ.*/
    MQTT_PINGRESP = 13,    /** The control type in the MQTT protocol packet assembly is PINGRESP.*/
    MQTT_DISCONNECT = 14,  /** The control type in the MQTT protocol packet assembly is DISCONNECT.*/
#ifdef CONFIG_QCM_MQTT5_FUNC
    MQTT_AUTH = 15,        /** The control type in the MQTT protocol packet assembly is AUTH.*/
#endif
} mqtt_msg_types;

typedef enum
{
    MQTT_CONNECTION_ACCEPTED = 0,
    MQTT_UNNACCEPTABLE_PROTOCOL = 1,
    MQTT_CLIENTID_REJECTED = 2,
    MQTT_SERVER_UNAVAILABLE = 3,
    MQTT_BAD_USERNAME_OR_PASSWORD = 4,
    MQTT_NOT_AUTHORIZED = 5,
} mqtt_connack_return_codes;

/**
 * Data for a publish packet.
 */
typedef struct
{
    qcm_mqtt_support_version_e version; /**< MQTT version number */
    qcm_mqtt_header            header;  /**< MQTT header byte */
    qcm_mqtt_packet_buffer    *topic;   /**< topic string */
    qosa_uint16_t              msg_id;  /**< MQTT message id */
    qcm_mqtt_packet_buffer    *payload; /**< binary payload, length delimited */
#ifdef CONFIG_QCM_MQTT5_FUNC
    qcm_mqtt_pub_properties_t *options; /**< MQTT 5.0 properties */
#endif                                  /* CONFIG_QCM_MQTT5_FUNC */
} mqtt_input_publish_packet;

typedef struct
{
    qcm_mqtt_support_version_e version; /**< MQTT version number */
    qcm_mqtt_header            header;  /**< MQTT header byte */
    union
    {
        unsigned char all; /**< all connect flags */
        struct
        {
            int                     : 1; /**< unused */
            qosa_bool_t  cleanstart : 1; /**< cleansession flag */
            qosa_uint8_t will       : 1; /**< will flag */
            unsigned int willQos    : 2; /**< will QoS value */
            qosa_bool_t  willRetain : 1; /**< will retain setting */
            qosa_bool_t  password   : 1; /**< 3.1 password */
            qosa_bool_t  username   : 1; /**< 3.1 user name */
        } bits;
    } flags;                             /**< connect flags byte */

    char *protocol;                      /**< MQTT protocol name */
    char *client_id;                     /**< string client id */
    char *will_topic;                    /**< will topic */
    char *will_msg;                      /**< will payload */
    int   keep_alive_interval;           /**< keepalive timeout value in seconds */
    char *username;
    char *password;
#ifdef CONFIG_QCM_MQTT5_FUNC
    qcm_mqtt_properties_t conn_props; /**< MQTT 5.0 connect properties */
    qcm_mqtt_properties_t will_props; /**< MQTT 5.0 will properties */
#endif                                /* CONFIG_QCM_MQTT5_FUNC */
} mqtt_connect_packet;

typedef struct
{
    qcm_mqtt_support_version_e version; /**< MQTT version number */
    qcm_mqtt_header            header;
    union
    {
        unsigned char all; /**< all connack flags */
        struct
        {
            unsigned int                : 7; /**< unused */
            unsigned int sessionpresent : 1; /**< session present flag */
        } bits;
    } flags;

    char return_code;

#ifdef CONFIG_QCM_MQTT5_FUNC
    qcm_mqtt_connack_properties_t *options;
#endif                 /* CONFIG_QCM_MQTT5_FUNC */
} mqtt_connack_packet; /**< connack flags byte */

/**
 * Data for a packet with header only.
 */
typedef struct
{
    qcm_mqtt_support_version_e version; /**< MQTT version number */
    qcm_mqtt_header            header;  /**< MQTT header byte */
#ifdef CONFIG_QCM_MQTT5_FUNC
    qcm_mqtt_properties_t props;        /**< MQTT 5.0 properties */
#endif                                  /* CONFIG_QCM_MQTT5_FUNC */
} mqtt_packet;

/**
 * Data for a subscribe packet.
 */

typedef struct
{
    qcm_mqtt_support_version_e version; /**< MQTT version number */
    qcm_mqtt_header            header;  /**< MQTT header byte */
    qosa_uint16_t              msg_id;  /**< MQTT message id */
    qosa_q_type_t             *topic_list;
    qosa_uint32_t              packet_len;
#ifdef CONFIG_QCM_MQTT5_FUNC
    qcm_mqtt_properties_t *props; /**< MQTT 5.0 properties */
#endif                            /* CONFIG_QCM_MQTT5_FUNC */
} mqtt_subscribe_packet;

/**
 * Data for a suback packet.
 */
typedef struct
{
    qcm_mqtt_support_version_e version; /**< MQTT version number */
    qcm_mqtt_header            header;  /**< MQTT header byte */
    qosa_uint16_t              msg_id;  /**< MQTT message id */
    qosa_uint8_t              *qoss;    /**< list of granted QoSs */
    int                        qos_cnt;
#ifdef CONFIG_QCM_MQTT5_FUNC
    qcm_mqtt_suback_properties_t *options;
#endif /* CONFIG_QCM_MQTT5_FUNC */
} mqtt_suback_packet;

typedef struct
{
    qcm_mqtt_support_version_e version; /**< MQTT version number */
    qcm_mqtt_header            header;  /**< MQTT header byte */
    qcm_mqtt_data_t            topic;   /**< topic string */
    qosa_uint16_t              msg_id;  /**< MQTT message id */
    qcm_mqtt_data_t            payload; /**< binary payload, length delimited */
#ifdef CONFIG_QCM_MQTT5_FUNC
    qcm_mqtt_pub_properties_t *options; /**< MQTT 5.0 properties */
    qcm_mqtt_properties_t     *props;   /**< MQTT 5.0 properties */
#endif                                  /* CONFIG_QCM_MQTT5_FUNC */
} mqtt_publish_packet;

/**
 * @struct  qcm_mqtt_request_t
 * @brief Used to record a single MQTT data packet to be sent.
 */
typedef struct
{
    qosa_q_link_type_t     link;        /*!< Linked list head node*/
    qcm_mqtt_header        header;      /*!< Corresponds to the MQTT protocol header field information of the sent data packet, primarily recording the type of the sent data packet.*/
    qosa_uint16_t          msg_id;      /*!< Corresponds to the MQTT packet message ID*/
    qosa_time_t            expire_time; /*!< Record the future timeout time of the current data packet*/
    int                    retry_cnt;   /*!< Record the number of MQTT packet retransmissions*/
    qcm_mqtt_packet_buffer buffer;      /*!< Packet information to be sent*/
} qcm_mqtt_request_t;

/**
 * Data for one of the ack packets.
 */
typedef struct
{
    qcm_mqtt_support_version_e version;     /*!< MQTT version number */
    qcm_mqtt_header            header;      /*!< MQTT header byte */
    qosa_uint16_t              msg_id;      /*!< MQTT message id */
#ifdef CONFIG_QCM_MQTT5_FUNC
    qosa_uint8_t               reason_code; /*!< MQTT 5.0 reason codes */
    qcm_mqtt_ack_properties_t *options;     /**< MQTT 5.0 properties */
    qcm_mqtt_properties_t     *props;       /**< MQTT 5.0 properties */
#endif                                      /* CONFIG_QCM_MQTT5_FUNC */
} mqtt_ack;

typedef mqtt_ack mqtt_puback_packet;
typedef mqtt_ack mqtt_pubrec_packet;
typedef mqtt_ack mqtt_pubrel_packet;
typedef mqtt_ack mqtt_pubcomp_packet;
typedef mqtt_ack mqtt_disconnect_packet;
#ifdef CONFIG_QCM_MQTT5_FUNC
typedef mqtt_ack mqtt_auth_packet;

/**
 * Data for one of the ack packets.
 */
typedef struct
{
    qcm_mqtt_support_version_e version;                             /*!< MQTT version number */
    qcm_mqtt_header            header;                              /*!< MQTT header byte */
    qosa_uint16_t              msg_id;                              /*!< MQTT message id */
    qosa_uint8_t               reason_code[QCM_MQTT_MAX_TOPIC_NUM]; /*!< MQTT 5.0 reason codes */
    qcm_mqtt_ack_properties_t *options;                             /**< MQTT 5.0 properties */
} mqtt_unsuback_packet;
#else
typedef mqtt_ack mqtt_unsuback_packet;
#endif /* CONFIG_QCM_MQTT5_FUNC */

/**
 * @brief Used to record key characteristics of MQTT message packets
 */
typedef struct
{
    qosa_uint16_t  msg_id;
    mqtt_msg_types msg_type;
} qcm_mqtt_packet_compare_t;

typedef struct
{
    qosa_q_link_type_t  link;
    int                 store_id;
    mqtt_publish_packet new_msg;
} qcm_mqtt_new_message_t;

/**
 *  @brief Calculate the length of the MQTT protocol's remaining length field.
 *
 * @param[in] qosa_uint32_t rem_len
 *           - The actual length of the remaining length field.
 * @return qosa_uint8_t
 *       - Returns the length of the remaining length field.
 */
qosa_uint8_t qcm_mqtt_packet_vbi_len(qosa_uint32_t rem_len);

/**
 * @brief Encode the remaining length field of the MQTT protocol.
 *
 * @param[out] char * buf
 *            - Used to store the encoded result.
 * @param[in] int length
 *           - The actual value of the remaining length field.
 * @return int
 *        - Returns the number of bytes used in the buffer.
 */
int qcm_mqtt_remaining_length_encode(char *buf, int length);

/**
 * @brief Write a char type data into the buf of buffer_block_type.
 *
 * @param[out] qcm_mqtt_packet_buffer * buf
 *            - The buffer of type mqtt_packet_buffer to be written.
 * @param[in] char c
 *           - The char type data to be written.
 */
void qcm_mqtt_write_char(qcm_mqtt_packet_buffer *rb, char c);

/**
 * @brief Write an int type data into the buf of mqtt_packet_buffer.
 *
 * @param[out] qcm_mqtt_packet_buffer * rb
 *           - The buff of type mqtt_packet_buffer to be written.
 * @param[in] int anInt
 *           - The int type data to be written.
 */
void qcm_mqtt_write_int(qcm_mqtt_packet_buffer *rb, qosa_int32_t anInt);

/**
 * @brief Write a 4-byte osa_uint32_t type data into the buf of mqtt_packet_buffer.
 *
 * @param[out] qcm_mqtt_packet_buffer * rb
 *           - The buffer of type mqtt_packet_buffer to be written.
 * @param[in] qosa_uint32_t anInt
 *           - Data of type osa_uint32_t to be written.
 */
void qcm_mqtt_write_int4(qcm_mqtt_packet_buffer *rb, qosa_uint32_t anInt);

/**
 * @brief Write the string into the buf of mqtt_packet_buffer.
 *
 * @param[out] qcm_mqtt_packet_buffer * rb
 *            - The buffer of type mqtt_packet_buffer to be written.
 * @param[in] const char * string
 *           - Pointer to the string data to be written.
 * @return qosa_bool_t
 *        - Returns OSA_TRUE to indicate a successful write.
 *        - Returns OSA_FALSE to indicate a write failure.
 */
void qcm_mqtt_write_string(qcm_mqtt_packet_buffer *rb, const char *string, qosa_uint32_t len);

/**
 *  @brief Read a character from the MQTT protocol.
 *
 * @param[in] qcm_mqtt_input_info_t * rb
 *           - Buffer for storing unparsed data.
 * @return qosa_uint8_t
 *        - Returns the read character value.
 */
qosa_uint8_t qcm_mqtt_read_char(qcm_mqtt_input_info_t *rb);

/**
 *  @brief Read integer values in the MQTT protocol.
 *
 * @param[in] qcm_mqtt_input_info_t * rb
 *           - Buffer for storing unparsed data.
 * @param[in] int offset
 *           - The offset read.
 * @return int
 *        - Returns the read integer value.
 */
int qcm_mqtt_read_int(qcm_mqtt_input_info_t *rb, int offset);

/**
 *  @brief Read the 4-byte integer value in the MQTT protocol.
 *
 * @param[in] qcm_mqtt_input_info_t * rb
 *           - Buffer for storing unparsed data.
 * @param[in] offset
 *           - The read offset.
 * @return qosa_uint32_t
 *        - Returns the read 4-byte integer value.
 */
qosa_uint32_t qcm_mqtt_read_int4(qcm_mqtt_input_info_t *rb, int offset);

/**
 *  @brief Read the string in the MQTT protocol.
 *
 * @param[in] qcm_mqtt_data_t * lenstring
 *           - Pointer to the structure storing the read string data.
 * @param[in] qcm_mqtt_input_info_t * rb
 *           - Buffer for storing unparsed data.
 * @return qosa_int32_t
 *        - Returns the length of the read string.
 */
qosa_int32_t qcm_mqtt_read_string(qcm_mqtt_data_t *lenstring, qcm_mqtt_input_info_t *rb);

/**
 * @brief Decode the remaining length field of the received data packet.
 *
 * @param[in] const char * stream
 *           - Pointer to the data stream to be parsed.
 *
 * @param[in] int size
 *           - The length of the remaining length field.
 *
 * @param[out] qosa_uint32_t * value
 *            - The actual packet length represented by the remaining length field.
 *
 * @return int
 *        - Returns a number greater than 0 indicating the valid length of the remaining length field.
 *        - Returns a number less than 0 to indicate a data parsing error.
 */
int qcm_mqtt_remaining_length_decode(const char *stream, int size, qosa_uint32_t *value);

/**
 * @brief According to the basic specification of the MTT protocol, check whether the first byte is the MQTT protocol.
 *
 * @param[in] qcm_mqtt_header header
 *       - The MQTT packet header to be checked.
 *
 * @return mqtt_packet_errors
 *        - Returns MQTTPACKET_READ_COMPLETE to indicate successful unpacking.
 *        - Returns other values indicating failure.
 */
mqtt_packet_errors qcm_mqtt_check_header(qcm_mqtt_header header);

/**
 * @brief MQTT protocol CONNECT packet assembly function.
 *
 * @param[out] qcm_mqtt_packet_buffer * rb
 *           - Data buffer for the CONNECT to be written.
 *
 * @param[in] mqtt_connect_packet * conn_pkt
 *           Pointer to the data structure of the -CONNECT packet.
 *
 * @return mqtt_packet_errors
 *        - Returns MQTTPACKET_READ_COMPLETE to indicate successful unpacking.
 *        - Returns other values indicating failure.
 */
mqtt_packet_errors qcm_mqtt_serialize_connect(qcm_mqtt_packet_buffer *rb, mqtt_connect_packet *conn_pkt);

/**
 * @brief The packet assembly function for Disconnect in the MQTT protocol.
 *
 * @param[out] qcm_mqtt_packet_buffer * rb
 *           - The assembled packet, the actual data pointer sent out using the socket.
 *
 * @param[in] mqtt_packet * mqtt_pkt
 *           -Data pointer for the Disconnect field parameter.
 *
 * @return mqtt_packet_errors
 *        - Returns MQTTPACKET_READ_COMPLETE to indicate successful unpacking.
 *        - Returns other values indicating failure.
 */
mqtt_packet_errors qcm_mqtt_serialize_zero(qcm_mqtt_packet_buffer *rb, mqtt_packet *mqtt_pkt);

/**
 * @brief The packet assembly function for MQTT client subscription topics.
 *
 * @param[in] rb
 * @param[in] sub_pkt
 * @return mqtt_packet_errors
 */
mqtt_packet_errors qcm_mqtt_serialize_subscribe(qcm_mqtt_packet_buffer *rb, mqtt_subscribe_packet *sub_pkt);

/**
 * @brief
 *
 * @param[in] rb
 * @param[in] mqtt_ack_pkt
 * @return mqtt_packet_errors
 */
mqtt_packet_errors qcm_mqtt_serialize_ack(qcm_mqtt_packet_buffer *rb, mqtt_ack *mqtt_ack_pkt);

/**
 * @brief The packet assembly function for Disconnect in the MQTT protocol.
 *
 * @param[out] qcm_mqtt_packet_buffer * rb
 *           - The assembled packet, the actual data pointer sent out using the socket.
 * @param[in] mqtt_disconnect_packet * mqtt_pkt
 *           -Data pointer for the Disconnect field parameter.
 * @return mqtt_packet_errors
 *        - Returns MQTTPACKET_READ_COMPLETE to indicate successful packet assembly.
 *        - Returns other values indicating failure.
 */
mqtt_packet_errors qcm_mqtt_serialize_disconnect(qcm_mqtt_packet_buffer *rb, mqtt_disconnect_packet *disc_pkt);

/**
 * @brief
 *
 * @param[in] rb
 * @param[in] pub_pkt
 * @return mqtt_packet_errors
 */
mqtt_packet_errors qcm_mqtt_serialize_publish(qcm_mqtt_packet_buffer *rb, mqtt_publish_packet *pub_pkt);

/**
 * @brief Parse the CONNECT ACK packet received by the MQTT client.
 *
 * @param[in] qcm_mqtt_packet_buffer * rb
 *           - The data buffer to be parsed for CONNECT.
 * @param[in] mqtt_connack_packet * connack_pkt
 *           - Stores the parsed data of the CONNECT ACK.
 * @param[in] qosa_uint32_t remain_len
 *           - Remaining length field value.
 * @return mqtt_packet_errors
 *        - Returns MQTTPACKET_READ_COMPLETE to indicate successful unpacking.
 *        - Returns other values indicating failure.
 */
mqtt_packet_errors qcm_mqtt_deserialize_connectack(qcm_mqtt_input_info_t *rb, mqtt_connack_packet *connack_pkt, qosa_uint32_t remain_len);

/**
 * @brief
 *
 * @param[in] rb
 * @param[in] mqtt_pkt
 * @param[in] remain_len
 * @return mqtt_packet_errors
 */
mqtt_packet_errors qcm_mqtt_deserialize_zero(qcm_mqtt_input_info_t *rb, mqtt_packet *mqtt_pkt, qosa_uint32_t remain_len);

/**
 * @brief Parse the SUBSCRIBE ACK packet received by the MQTT client.
 *
 * @param[in] rb
 * @param[in] suback_pkt
 * @param[in] remain_len
 * @return mqtt_packet_errors
 */
mqtt_packet_errors qcm_mqtt_deserialize_subscribe_ack(qcm_mqtt_input_info_t *rb, mqtt_suback_packet *suback_pkt, qosa_uint32_t remain_len);

/**
 * @brief
 *
 * @param[in] rb
 * @param[in] mqtt_ack_pkt
 * @param[in] remain_len
 * @return mqtt_packet_errors
 */
mqtt_packet_errors qcm_mqtt_deserialize_ack(qcm_mqtt_input_info_t *rb, mqtt_ack *mqtt_ack_pkt, qosa_uint32_t remain_len);

/**
 * @brief Parse the UNSUBACK packet received by the MQTT client.
 *
 * @param[in] qcm_mqtt_input_info_t * rb
 *           - The data buffer for the UNSUBACK to be parsed.
 * @param[in] mqtt_unsuback_packet * mqtt_ack_pkt
 *           - Stores the parsed data of UNSUBACK.
 * @param[in] int remain_len
 *           - Remaining length field value.
 * @return mqtt_packet_errors
 *        - Returns MQTTPACKET_READ_COMPLETE to indicate successful unpacking.
 */
mqtt_packet_errors qcm_mqtt_deserialize_unsuback(qcm_mqtt_input_info_t *rb, mqtt_unsuback_packet *unsuback_pkt, qosa_uint32_t remain_len);

/**
 * @brief
 *
 * @param[in] rb
 * @param[in] pub_pkt
 * @param[in] remain_len
 * @return mqtt_packet_errors
 */
mqtt_packet_errors qcm_mqtt_deserialize_publish(qcm_mqtt_input_info_t *rb, mqtt_publish_packet *pub_out_pkt);

#endif /* __QCM_MQTT_PACKET_H__ */
