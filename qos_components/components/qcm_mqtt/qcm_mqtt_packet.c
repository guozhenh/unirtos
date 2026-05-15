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
* @file qcm_mqtt_packet.c
* @brief
* @author larson.li@quectel.com
* @date 2023-12-07
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description
* <tr><td>2023-12-07 <td>1.0 <td>Larson.Li <td> Init
* <tr><td>2025-06-30 <td>1.1 <td>Lawrence.liu <td> Add MQTT5 properties support
* </table>
**********************************************************************/
#include "qcm_mqtt_private.h"
#include "qosa_log.h"
#include "qcm_mqtt_properties.h"
#include "qcm_mqtt_packet.h"
#include "qcm_mqtt.h"

/** Redefine the TAG of this file */
#define QOS_LOG_TAG      LOG_TAG_MQTT_API

#define MQTT_PROTOCOL_V3 "MQIsdp"
#define MQTT_PROTOCOL    "MQTT"

#ifdef CONFIG_QCM_MQTT5_FUNC
/**
 *  @brief Calculate the length of the remaining length field in the MQTT protocol.
 *
 * @param[in] qosa_uint32_t rem_len
 *           - The actual length of the remaining length field.
 * @return qosa_uint8_t
 *       - Returns the length of the remaining length field.
 */
qosa_uint8_t qcm_mqtt_packet_vbi_len(qosa_uint32_t rem_len)
{
    qosa_uint8_t rc = 0;

    if (rem_len < 128)
    {
        rc = 1;
    }
    else if (rem_len < 16384)
    {
        rc = 2;
    }
    else if (rem_len < 2097152)
    {
        rc = 3;
    }
    else
    {
        rc = 4;
    }

    return rc;
}
#endif /* CONFIG_QCM_MQTT5_FUNC */

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
int qcm_mqtt_remaining_length_encode(char *buf, int length)
{
    int rc = 0;

    do {
        char d = length % 128;
        length /= 128;
        /* if there are more digits to encode, set the top bit of this digit */
        if (length > 0)
        {
            d |= 0x80;
        }

        buf[rc++] = d;
    } while (length > 0 && rc < 4);

    QLOGV("rc=%d", rc);

    return rc;
}

/**
 * @brief Decode the remaining length field of the received data packet.
 *
 * @param[in] const char * stream
 *           - Pointer to the data stream to be parsed.
 * @param[in] int size
 *           - The length of the remaining length field.
 * @param[out] qosa_uint32_t * value
 *            - The actual packet length represented by the remaining length field.
 * @return int
 *        - Returns a number greater than 0 indicating the valid length of the remaining length field.
 *        - Returns a number less than 0 to indicate a data parsing error.
 */
int qcm_mqtt_remaining_length_decode(const char *stream, int size, qosa_uint32_t *value)
{
    const qosa_uint8_t *in = (const qosa_uint8_t *)stream;
    qosa_uint32_t       multiplier = 1;

    *value = 0;
    int i = 0;
    for (i = 0; i < size; ++i)
    {
        *value += (in[i] & 0x7f) * multiplier;

        if (!(in[i] & 0x80))
        {
            return i + 1;
        }

        if (multiplier >= 128 * 128 * 128 * 128)
        {
            return MQTTPACKET_BUFFER_TOO_SHORT;  // error, out of range
        }
        multiplier *= 128;
    }

    return MQTTPACKET_READ_ERROR;  // not complete
}

/**
 * @brief Write a char type data into the buf of buffer_block_type.
 *
 * @param[out] qcm_mqtt_packet_buffer * buf
 *            - The buffer of type mqtt_packet_buffer to be written.
 * @param[in] char c
 *           - The char type data to be written.
 */
void qcm_mqtt_write_char(qcm_mqtt_packet_buffer *rb, char c)
{
    if (rb->buf_len + 1 > rb->buf_ptr.data_len)
    {
        QLOGE("overflow :%d", rb->buf_len);
        return;
    }

    rb->buf_ptr.data_ptr[rb->buf_len++] = c;
}

/**
 * @brief Write an int type data into the buf of mqtt_packet_buffer.
 *
 * @param[out] qcm_mqtt_packet_buffer * rb
 *           - The buffer of type mqtt_packet_buffer to be written.
 * @param[in] int anInt
 *           - The int type data to be written.
 */
void qcm_mqtt_write_int(qcm_mqtt_packet_buffer *rb, qosa_int32_t anInt)
{
    qosa_uint8_t integer[2] = {0, 0};

    if (rb->buf_len + 2 > rb->buf_ptr.data_len)
    {
        QLOGE("overflow:%d", rb->buf_len);
        return;
    }

    integer[0] = (qosa_uint8_t)(anInt / 256);
    integer[1] = (qosa_uint8_t)(anInt % 256);

    qosa_memcpy(rb->buf_ptr.data_ptr + rb->buf_len, (void *)&integer[0], 2);
    rb->buf_len += 2;
}

/**
 * @brief Write a 4-byte osa_uint32_t type data into the buf of mqtt_packet_buffer.
 *
 * @param[out] qcm_mqtt_packet_buffer * rb
 *           - The buff of type mqtt_packet_buffer to be written.
 * @param[in] qosa_uint32_t anInt
 *           - Data of type osa_uint32_t to be written.
 */
void qcm_mqtt_write_int4(qcm_mqtt_packet_buffer *rb, qosa_uint32_t anInt)
{
    qosa_uint8_t integer[4] = {0, 0};

    if (rb->buf_len + 4 > rb->buf_ptr.data_len)
    {
        QLOGE("overflow:%d", rb->buf_len);
        return;
    }

    integer[0] = (qosa_uint8_t)(anInt / 16777216);
    anInt %= 16777216;
    integer[1] = (qosa_uint8_t)(anInt / 65536);
    integer[2] = (qosa_uint8_t)(anInt / 256);
    integer[3] = (qosa_uint8_t)(anInt % 256);
    qosa_memcpy(rb->buf_ptr.data_ptr + rb->buf_len, (void *)&integer[0], 4);
    rb->buf_len += 4;
}

/**
 * @brief Write the string into the buf of mqtt_packet_buffer.
 *
 * @param[out] qcm_mqtt_packet_buffer * rb
 *            - The buff of type mqtt_packet_buffer to be written.
 * @param[in] const char * string
 *           - Pointer to the string data to be written.
 * @return qosa_bool_t
 *        - Returns OSA_TRUE to indicate a successful write.
 *        - Returns OSA_FALSE to indicate a write failure.
 */
void qcm_mqtt_write_string(qcm_mqtt_packet_buffer *rb, const char *string, qosa_uint32_t len)
{
    if (rb->buf_len + len > rb->buf_ptr.data_len)
    {
        QLOGE("overflow:%d", rb->buf_len);
        return;
    }

    qosa_memcpy(rb->buf_ptr.data_ptr + rb->buf_len, string, len);

    rb->buf_len += len;
}

/**
 *  @brief Read a character from the MQTT protocol.
 *
 * @param[in] qcm_mqtt_packet_buffer * rb
 *           - Buffer for storing unparsed data.
 * @return qosa_uint8_t
 *        - Returns the read character value.
 */
qosa_uint8_t qcm_mqtt_read_char(qcm_mqtt_input_info_t *rb)
{
    qosa_uint8_t c = rb->buff[rb->buf_pos++];

    return c;
}

/**
 *  @brief Read integer values in the MQTT protocol.
 *
 * @param[in] qcm_mqtt_input_info_t * rb
 *           - Buffer for storing unparsed data.
 * @param[in] int offset
 *           - The read offset.
 * @return int
 *        - Returns the read integer value.
 */
int qcm_mqtt_read_int(qcm_mqtt_input_info_t *rb, int offset)
{
    qosa_uint8_t integer[2] = {0, 0};
    int          len = 0;

    qosa_memcpy((void *)&integer[0], rb->buff + offset, 2);
    QLOGV("buff0:%d buff1:%d", integer[0], integer[1]);

    len = (integer[0] << 8) + integer[1];
    QLOGV("len:%d", len);

    rb->buf_pos += 2;

    return len;
}

/**
 *  @brief Read the 4-byte integer value in the MQTT protocol.
 *
 * @param[in] qcm_mqtt_input_info_t * rb
 *           - Buffer for storing unparsed data.
 * @param[in] offset
 *           - The offset read.
 * @return qosa_uint32_t
 *        - Returns the read 4-byte integer value.
 */
qosa_uint32_t qcm_mqtt_read_int4(qcm_mqtt_input_info_t *rb, int offset)
{
    qosa_uint8_t  integer[4] = {0, 0};
    qosa_uint32_t len = 0;

    qosa_memcpy((void *)&integer[0], rb->buff + offset, 4);
    len = (integer[0] << 24) + (integer[1] << 16) + (integer[2] << 8) + integer[3];
    rb->buf_pos += 4;

    return len;
}

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
qosa_int32_t qcm_mqtt_read_string(qcm_mqtt_data_t *lenstring, qcm_mqtt_input_info_t *rb)
{
    qosa_int32_t len = -1;

    /* the first two bytes are the length of the string */
    if (rb->buf_len - 1 > rb->buf_pos) /* enough length to read the integer? */
    {
        lenstring->data_len = qcm_mqtt_read_int(rb, rb->buf_pos);
        if (rb->buf_len >= lenstring->data_len + rb->buf_pos)
        {
            lenstring->data_ptr = (char *)rb->buff + rb->buf_pos;
            rb->buf_pos += lenstring->data_len;
            len = 2 + lenstring->data_len;
        }
    }
    return len;
}

/**
 * @brief According to the basic specification of the MTT protocol, check whether the first byte is the MQTT protocol.
 *
 * @param[in] qcm_mqtt_header header
 *       - The MQTT packet header to be checked.
 * @return mqtt_packet_errors
 *        - Returns MQTTPACKET_READ_COMPLETE to indicate successful unpacking.
 *        - Returns other values indicating failure.
 */
mqtt_packet_errors qcm_mqtt_check_header(qcm_mqtt_header header)
{
    if (header.bits.type < MQTT_CONNECT)
    {
        return MQTTPACKET_READ_ERROR;
    }

    //In the MQTT protocol versions 3.1.1 and 5.0 specifications, the last 4 bits of the first byte in the header must not be zero.
    //PUBLISH , 0x62 = PUBREL   0x82 = SUBSCRIBE  0xA2 = UNSUBSCRIBE
    //The remaining four bytes being non-zero indicate an error; according to the MQTT protocol specification, upon receiving an unusable representation, the receiver must close the network connection.
    if (0 != (header.byte & 0x0F) && MQTT_PUBLISH != header.bits.type && 0x62 != header.byte && 0x82 != header.byte && 0xA2 != header.byte)
    {
        return MQTTPACKET_READ_ERROR;
    }

    //PUBLISH message qos cannot be 3, only 0, 1, or 2.
    if (MQTT_PUBLISH == header.bits.type && 3 == header.bits.qos)
    {
        return MQTTPACKET_READ_ERROR;
    }

    //Subsequently, additional protocol header verification will be added here.

    return MQTTPACKET_READ_COMPLETE;
}

/**
 * @brief MQTT protocol CONNECT packet assembly function.
 *
 * @param[out] qcm_mqtt_packet_buffer * rb
 *           - Data buffer for CONNECT to be written.
 * @param[in] mqtt_connect_packet * conn_pkt
 *           Pointer to the data structure of the -CONNECT packet.
 * @return mqtt_packet_errors
 *        - Returns MQTTPACKET_READ_COMPLETE to indicate successful unpacking.
 *        - Returns other values indicate failure.
 */
mqtt_packet_errors qcm_mqtt_serialize_connect(qcm_mqtt_packet_buffer *rb, mqtt_connect_packet *conn_pkt)
{
    int           rc = 0;
    int           remain_len = 0;
    qosa_uint8_t  rem_len[4] = {0};
    qosa_uint16_t len = 0;

    QLOGV("connect pkt dump:");
    QLOGV("will:%d, username:%d,password:%d", conn_pkt->flags.bits.will, conn_pkt->flags.bits.username, conn_pkt->flags.bits.password);
    QLOGV("header:%d, flag:%d", conn_pkt->header.byte, conn_pkt->flags.all);
    QLOGV("kalive:%d", conn_pkt->keep_alive_interval);

    if (conn_pkt->version == QCM_MQTT_VERSION_V3)
    {
        remain_len = 12;
    }
    else
    {
        remain_len = 10;
    }

    remain_len += (qosa_strlen(conn_pkt->client_id) + 2);
    if (conn_pkt->flags.bits.will)
    {
        remain_len += (qosa_strlen(conn_pkt->will_topic) + 2 + qosa_strlen(conn_pkt->will_msg) + 2);
    }

    if (conn_pkt->flags.bits.username)
    {
        remain_len += (qosa_strlen(conn_pkt->username) + 2);
    }

    if (conn_pkt->flags.bits.password)
    {
        remain_len += (qosa_strlen(conn_pkt->password) + 2);
    }

#ifdef CONFIG_QCM_MQTT5_FUNC
    if (conn_pkt->version >= QCM_MQTT_VERSION_V5)
    {
        QLOGV("MQTT V5 connect packet");
        remain_len += qcm_mqtt_properties_len(&conn_pkt->conn_props);
        if (conn_pkt->flags.bits.will)
        {
            remain_len += qcm_mqtt_properties_len(&conn_pkt->will_props);
        }
    }
#endif /* CONFIG_QCM_MQTT5_FUNC */

    QLOGV("remain len:%d", remain_len);
    rc = qcm_mqtt_remaining_length_encode((char *)&rem_len[0], remain_len);
    QLOGI("rc:%d", rc);

    rb->buf_ptr.data_len = remain_len + 1 + rc;
    rb->buf_ptr.data_ptr = qosa_malloc(rb->buf_ptr.data_len);  //1: message header
    if (rb->buf_ptr.data_ptr == QOSA_NULL)
    {
        return MQTTPACKET_BUFFER_TOO_SHORT;
    }

    qosa_memset(rb->buf_ptr.data_ptr, 0, rb->buf_ptr.data_len);
    rb->buf_len = 0;
    rb->buf_pos = 0;

    qcm_mqtt_write_char(rb, conn_pkt->header.byte);
    qcm_mqtt_write_string(rb, (void *)&rem_len[0], rc);

    if (conn_pkt->version == QCM_MQTT_VERSION_V3)
    {
        len = qosa_strlen(MQTT_PROTOCOL_V3);

        qcm_mqtt_write_int(rb, len);
        qcm_mqtt_write_string(rb, MQTT_PROTOCOL_V3, len);
    }
    else
    {
        len = qosa_strlen(MQTT_PROTOCOL);
        qcm_mqtt_write_int(rb, len);
        qcm_mqtt_write_string(rb, MQTT_PROTOCOL, len);
    }

    qcm_mqtt_write_char(rb, (char)conn_pkt->version);
    qcm_mqtt_write_char(rb, conn_pkt->flags.all);
    qcm_mqtt_write_int(rb, conn_pkt->keep_alive_interval);

#ifdef CONFIG_QCM_MQTT5_FUNC
    if (conn_pkt->version >= QCM_MQTT_VERSION_V5)
    {
        qcm_mqtt_properties_write(rb, &conn_pkt->conn_props);
    }
#endif /* CONFIG_QCM_MQTT5_FUNC */

    len = qosa_strlen(conn_pkt->client_id);
    qcm_mqtt_write_int(rb, len);
    qcm_mqtt_write_string(rb, conn_pkt->client_id, len);

    if (conn_pkt->flags.bits.will)
    {
#ifdef CONFIG_QCM_MQTT5_FUNC
        if (conn_pkt->version >= QCM_MQTT_VERSION_V5)
        {
            qcm_mqtt_properties_write(rb, &conn_pkt->will_props);
        }
#endif /* CONFIG_QCM_MQTT5_FUNC */
        len = qosa_strlen(conn_pkt->will_topic);
        qcm_mqtt_write_int(rb, len);
        qcm_mqtt_write_string(rb, conn_pkt->will_topic, len);
        len = qosa_strlen(conn_pkt->will_msg);
        qcm_mqtt_write_int(rb, len);
        qcm_mqtt_write_string(rb, conn_pkt->will_msg, len);
    }

    if (conn_pkt->flags.bits.username)
    {
        len = qosa_strlen(conn_pkt->username);
        qcm_mqtt_write_int(rb, len);
        qcm_mqtt_write_string(rb, conn_pkt->username, len);
    }

    if (conn_pkt->flags.bits.password)
    {
        len = qosa_strlen(conn_pkt->password);
        qcm_mqtt_write_int(rb, len);
        qcm_mqtt_write_string(rb, conn_pkt->password, len);
    }

    QLOGV("packet buf len:%d", rb->buf_len);
    return MQTTPACKET_READ_COMPLETE;
}

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
mqtt_packet_errors qcm_mqtt_serialize_disconnect(qcm_mqtt_packet_buffer *rb, mqtt_disconnect_packet *disc_pkt)
{
    int          rc = 0;
    int          remain_len = 0;
    qosa_uint8_t rem_len[4] = {0, 0, 0, 0};

#ifdef CONFIG_QCM_MQTT5_FUNC
    if (disc_pkt->version >= QCM_MQTT_VERSION_V5 && (disc_pkt->reason_code != 0))
    {
        QLOGV("MQTT V5 disconnect packet");
        remain_len += 1;                                             //reason code
        remain_len += qcm_mqtt_properties_len(disc_pkt->props) + 1;  //properties length
    }
#endif                                                               /* CONFIG_QCM_MQTT5_FUNC */
    rc = qcm_mqtt_remaining_length_encode((char *)&rem_len[0], remain_len);

    rb->buf_ptr.data_len = remain_len + 1 + rc;
    rb->buf_ptr.data_ptr = qosa_malloc(rb->buf_ptr.data_len);  //1: message header
    if (rb->buf_ptr.data_ptr == QOSA_NULL)
    {
        QLOGE("malloc fail!!:%d", rb->buf_ptr.data_len);
        return MQTTPACKET_BUFFER_TOO_SHORT;
    }

    qosa_memset(rb->buf_ptr.data_ptr, 0, rb->buf_ptr.data_len);
    rb->buf_pos = 0;
    rb->buf_len = 0;

    qcm_mqtt_write_char(rb, disc_pkt->header.byte);
    qcm_mqtt_write_string(rb, (void *)&rem_len[0], rc);

#ifdef CONFIG_QCM_MQTT5_FUNC
    if (disc_pkt->version >= QCM_MQTT_VERSION_V5 && (disc_pkt->reason_code != 0))
    {
        qcm_mqtt_write_char(rb, disc_pkt->reason_code);
        qcm_mqtt_properties_write(rb, disc_pkt->props);
    }
#endif /* CONFIG_QCM_MQTT5_FUNC */

    QLOGV("packet buf len:%d", rb->buf_len);

    return MQTTPACKET_READ_COMPLETE;
}

mqtt_packet_errors qcm_mqtt_serialize_zero(qcm_mqtt_packet_buffer *rb, mqtt_packet *mqtt_pkt)
{
    int          rc = 0;
    int          remain_len = 0;
    qosa_uint8_t rem_len[4] = {0, 0, 0, 0};

    rc = qcm_mqtt_remaining_length_encode((char *)&rem_len[0], remain_len);

    rb->buf_ptr.data_len = remain_len + 1 + rc;
    rb->buf_ptr.data_ptr = qosa_malloc(rb->buf_ptr.data_len);  //1: message header
    if (rb->buf_ptr.data_ptr == QOSA_NULL)
    {
        QLOGE("malloc fail!!:%d", rb->buf_ptr.data_len);
        return MQTTPACKET_BUFFER_TOO_SHORT;
    }

    qosa_memset(rb->buf_ptr.data_ptr, 0, rb->buf_ptr.data_len);
    rb->buf_len = 0;
    rb->buf_pos = 0;

    qcm_mqtt_write_char(rb, mqtt_pkt->header.byte);

    qcm_mqtt_write_string(rb, (void *)&rem_len[0], rc);

    QLOGV("packet buf len:%d", rb->buf_len);

    return MQTTPACKET_READ_COMPLETE;
}

/**
 * @brief The packet assembly function for MQTT client subscription topics.
 *
 * @param[in] rb
 * @param[in] sub_pkt
 * @return mqtt_packet_errors
 */
mqtt_packet_errors qcm_mqtt_serialize_subscribe(qcm_mqtt_packet_buffer *rb, mqtt_subscribe_packet *sub_pkt)
{
    int          rc = 0;
    int          remain_len = sub_pkt->packet_len;
    qosa_uint8_t rem_len[4] = {0, 0, 0, 0};

    QLOGV("remain len:%d", remain_len);
#ifdef CONFIG_QCM_MQTT5_FUNC
    if (sub_pkt->version >= QCM_MQTT_VERSION_V5)
    {
        QLOGI("MQTT V5 subscribe packet");
        remain_len += qcm_mqtt_properties_len(sub_pkt->props);
    }
#endif /* CONFIG_QCM_MQTT5_FUNC */

    rc = qcm_mqtt_remaining_length_encode((char *)&rem_len[0], remain_len);
    QLOGV("rc:%d", rc);

    rb->buf_ptr.data_len = remain_len + 1 + rc;
    rb->buf_ptr.data_ptr = qosa_malloc(rb->buf_ptr.data_len);  //1: message header
    if (rb->buf_ptr.data_ptr == QOSA_NULL)
    {
        return MQTTPACKET_BUFFER_TOO_SHORT;
    }
    rb->buf_pos = 0;
    rb->buf_len = 0;

    qcm_mqtt_write_char(rb, sub_pkt->header.byte);
    qcm_mqtt_write_string(rb, (void *)&rem_len[0], rc);
    qcm_mqtt_write_int(rb, sub_pkt->msg_id);

#ifdef CONFIG_QCM_MQTT5_FUNC
    if (sub_pkt->version >= QCM_MQTT_VERSION_V5)
    {
        qcm_mqtt_properties_write(rb, sub_pkt->props);
    }
#endif /* CONFIG_QCM_MQTT5_FUNC */

    for (; qosa_q_cnt(sub_pkt->topic_list) != 0;)
    {
        qcm_mqtt_list_topic_t *topic_list = (qcm_mqtt_list_topic_t *)qosa_q_get(sub_pkt->topic_list);

        QLOGV("data_len:%d", topic_list->topic.data_len);
        qcm_mqtt_write_int(rb, topic_list->topic.data_len);
        qcm_mqtt_write_string(rb, topic_list->topic.data_ptr, topic_list->topic.data_len);

        if (sub_pkt->header.bits.type == MQTT_SUBSCRIBE)
        {
#ifdef CONFIG_QCM_MQTT5_FUNC
            if (sub_pkt->version >= QCM_MQTT_VERSION_V5)
            {
                topic_list->qos |= (topic_list->opts.noLocal << 2);           /* 1 bit */
                topic_list->qos |= (topic_list->opts.retainAsPublished << 3); /* 1 bit */
                topic_list->qos |= (topic_list->opts.retainHandling << 4);    /* 2 bits */
            }
#endif                                                                        /* CONFIG_QCM_MQTT5_FUNC */
            qcm_mqtt_write_char(rb, topic_list->qos);
        }

        qosa_free(topic_list->topic.data_ptr);
        qosa_free(topic_list);
    }

    return MQTTPACKET_READ_COMPLETE;
}

/**
 * @brief
 *
 * @param[in] rb
 * @param[in] mqtt_ack_pkt
 * @return mqtt_packet_errors
 */
mqtt_packet_errors qcm_mqtt_serialize_ack(qcm_mqtt_packet_buffer *rb, mqtt_ack *mqtt_ack_pkt)
{
    int          rc = 0;
    int          remain_len = 0;
    qosa_uint8_t rem_len[4] = {0, 0, 0, 0};

    remain_len = 2;

    QLOGV("remain len:%d", remain_len);
    rc = qcm_mqtt_remaining_length_encode((char *)&rem_len[0], remain_len);
    QLOGI("rc:%d", rc);

    rb->buf_ptr.data_len = remain_len + 1 + rc;
    rb->buf_ptr.data_ptr = qosa_malloc(rb->buf_ptr.data_len);  //1: message header
    if (rb->buf_ptr.data_ptr == QOSA_NULL)
    {
        return MQTTPACKET_BUFFER_TOO_SHORT;
    }
    rb->buf_pos = 0;
    rb->buf_len = 0;

    qcm_mqtt_write_char(rb, mqtt_ack_pkt->header.byte);

    qcm_mqtt_write_string(rb, (void *)&rem_len[0], rc);

    QLOGV("msg_id:%d", mqtt_ack_pkt->msg_id);

    qcm_mqtt_write_int(rb, mqtt_ack_pkt->msg_id);
    return MQTTPACKET_READ_COMPLETE;
}

/**
 * @brief Parse the PUBLISH packet received by the MQTT client.
 *
 * @param[in] rb
 * @param[in] pub_pkt
 * @return mqtt_packet_errors
 */
mqtt_packet_errors qcm_mqtt_serialize_publish(qcm_mqtt_packet_buffer *rb, mqtt_publish_packet *pub_pkt)
{
    int           rc = 0;
    qosa_uint32_t remain_len = 0;
    qosa_uint8_t  rem_len[4] = {0, 0, 0, 0};

    remain_len = 2 + pub_pkt->topic.data_len + pub_pkt->payload.data_len;
    if (pub_pkt->header.bits.qos > QCM_MQTT_AT_MOST_ONCE_DELIVERY)
    {
        remain_len += MQTT_PACKET_HEAD_LEN;
    }

#ifdef CONFIG_QCM_MQTT5_FUNC
    if (pub_pkt->version >= QCM_MQTT_VERSION_V5)
    {
        QLOGV("MQTT V5 publish packet");
        remain_len += qcm_mqtt_properties_len(pub_pkt->props);
    }
#endif /* CONFIG_QCM_MQTT5_FUNC */
    QLOGV("remain len:%d", remain_len);
    rc = qcm_mqtt_remaining_length_encode((char *)&rem_len[0], remain_len);
    QLOGI("rc:%d", rc);

    rb->buf_ptr.data_len = remain_len + 1 + rc;
    rb->buf_ptr.data_ptr = qosa_malloc(rb->buf_ptr.data_len);  //1: message header
    if (rb->buf_ptr.data_ptr == QOSA_NULL)
    {
        return MQTTPACKET_BUFFER_TOO_SHORT;
    }
    rb->buf_pos = 0;
    rb->buf_len = 0;

    qcm_mqtt_write_char(rb, pub_pkt->header.byte);
    qcm_mqtt_write_string(rb, (void *)&rem_len[0], rc);

    QLOGV("qos:%d", pub_pkt->header.bits.qos);
    QLOGI("msg_id:%d", pub_pkt->msg_id);

    qcm_mqtt_write_int(rb, pub_pkt->topic.data_len);
    qcm_mqtt_write_string(rb, pub_pkt->topic.data_ptr, pub_pkt->topic.data_len);

    if (pub_pkt->header.bits.qos > 0)
    {
        qcm_mqtt_write_int(rb, pub_pkt->msg_id);
    }

#ifdef CONFIG_QCM_MQTT5_FUNC
    if (pub_pkt->version >= QCM_MQTT_VERSION_V5)
    {
        qcm_mqtt_properties_write(rb, pub_pkt->props);
    }
#endif /* CONFIG_QCM_MQTT5_FUNC */

    qcm_mqtt_write_string(rb, pub_pkt->payload.data_ptr, pub_pkt->payload.data_len);

    return MQTTPACKET_READ_COMPLETE;
}

/**
 * @brief Parse the CONNECT ACK packet received by the MQTT client.
 *
 * @param[in] qcm_mqtt_input_info_t * rb
 *           - The data buffer to be parsed for CONNECT.
 * @param[in] mqtt_connack_packet * connack_pkt
 *           - Stores the parsed data of the CONNECT ACK.
 * @param[in] qosa_uint32_t remain_len
 *           - Remaining length field value.
 * @return mqtt_packet_errors
 *        - Returns MQTTPACKET_READ_COMPLETE to indicate successful unpacking.
 *        - Returns other values indicating failure.
 */
mqtt_packet_errors qcm_mqtt_deserialize_connectack(qcm_mqtt_input_info_t *rb, mqtt_connack_packet *connack_pkt, qosa_uint32_t remain_len)
{
    if (remain_len >= 2)
    {
        connack_pkt->flags.all = qcm_mqtt_read_char(rb);
        remain_len -= 1;

        connack_pkt->return_code = qcm_mqtt_read_char(rb);
        remain_len -= 1;
    }
    else
    {
        return MQTTPACKET_BUFFER_TOO_SHORT;
    }

#ifdef CONFIG_QCM_MQTT5_FUNC
    if (connack_pkt->version >= QCM_MQTT_VERSION_V5 && remain_len > 0)
    {
        qcm_mqtt_properties_t connectack_props = QCM_MQTT_PROPERTY_INIT;
        qcm_mqtt_property_t  *connectack_prop = QOSA_NULL;

        if (qcm_mqtt_properties_read(&connectack_props, rb, remain_len) == 1)
        {
            connack_pkt->options = qosa_malloc(sizeof(qcm_mqtt_connack_properties_t));
            if (connack_pkt->options == QOSA_NULL)
            {
                qcm_mqtt_properties_free(&connectack_props);
                return MQTTPACKET_READ_ERROR;
            }

            connectack_prop = qcm_mqtt_properties_get_property(&connectack_props, QCM_MQTT_PROPERTY_CODE_SESSION_EXPIRY_INTERVAL);
            if (connectack_prop != QOSA_NULL)
            {
                connack_pkt->options->session_expiry_interval = connectack_prop->value.integer4;
                connectack_prop = QOSA_NULL;
            }

            connectack_prop = qcm_mqtt_properties_get_property(&connectack_props, QCM_MQTT_PROPERTY_CODE_RECEIVE_MAXIMUM);
            if (connectack_prop != QOSA_NULL)
            {
                connack_pkt->options->receive_maximum = connectack_prop->value.integer2;
                connectack_prop = QOSA_NULL;
            }

            connectack_prop = qcm_mqtt_properties_get_property(&connectack_props, QCM_MQTT_PROPERTY_CODE_MAXIMUM_QOS);
            if (connectack_prop != QOSA_NULL)
            {
                connack_pkt->options->maximum_qos = connectack_prop->value.byte;
                connectack_prop = QOSA_NULL;
            }

            connectack_prop = qcm_mqtt_properties_get_property(&connectack_props, QCM_MQTT_PROPERTY_CODE_RETAIN_AVAILABLE);
            if (connectack_prop != QOSA_NULL)
            {
                connack_pkt->options->retain_available = connectack_prop->value.byte;
                connectack_prop = QOSA_NULL;
            }

            connectack_prop = qcm_mqtt_properties_get_property(&connectack_props, QCM_MQTT_PROPERTY_CODE_MAXIMUM_PACKET_SIZE);
            if (connectack_prop != QOSA_NULL)
            {
                connack_pkt->options->maximum_packet_size = connectack_prop->value.integer4;
                connectack_prop = QOSA_NULL;
            }

            connectack_prop = qcm_mqtt_properties_get_property(&connectack_props, QCM_MQTT_PROPERTY_CODE_TOPIC_ALIAS_MAXIMUM);
            if (connectack_prop != QOSA_NULL)
            {
                connack_pkt->options->topic_alias_maximum = connectack_prop->value.integer2;

                connectack_prop = QOSA_NULL;
            }

            connectack_prop = qcm_mqtt_properties_get_property(&connectack_props, QCM_MQTT_PROPERTY_CODE_WILDCARD_SUBSCRIPTION_AVAILABLE);
            if (connectack_prop != QOSA_NULL)
            {
                connack_pkt->options->wildcard_subscription_available = connectack_prop->value.byte;
                connectack_prop = QOSA_NULL;
            }

            connectack_prop = qcm_mqtt_properties_get_property(&connectack_props, QCM_MQTT_PROPERTY_CODE_SUBSCRIPTION_IDENTIFIERS_AVAILABLE);
            if (connectack_prop != QOSA_NULL)
            {
                connack_pkt->options->subscription_identifier_available = connectack_prop->value.byte;
                connectack_prop = QOSA_NULL;
            }

            connectack_prop = qcm_mqtt_properties_get_property(&connectack_props, QCM_MQTT_PROPERTY_CODE_SHARED_SUBSCRIPTION_AVAILABLE);
            if (connectack_prop != QOSA_NULL)
            {
                connack_pkt->options->shared_subscription_available = connectack_prop->value.byte;
                connectack_prop = QOSA_NULL;
            }

            connectack_prop = qcm_mqtt_properties_get_property(&connectack_props, QCM_MQTT_PROPERTY_CODE_SERVER_KEEP_ALIVE);
            if (connectack_prop != QOSA_NULL)
            {
                connack_pkt->options->server_keep_alive = connectack_prop->value.integer2;
                connectack_prop = QOSA_NULL;
            }

            connectack_prop = qcm_mqtt_properties_get_property(&connectack_props, QCM_MQTT_PROPERTY_CODE_RESPONSE_INFORMATION);
            if (connectack_prop != QOSA_NULL)
            {
                connack_pkt->options->response_information.data_len = connectack_prop->value.data.data_len;
                connack_pkt->options->response_information.data_ptr = connectack_prop->value.data.data_ptr;
                connectack_prop->identifier = 0;
                connectack_prop = QOSA_NULL;
            }

            qosa_uint8_t i = 0;
            for (; i < 5; i++)
            {
                connectack_prop = qcm_mqtt_properties_get_property(&connectack_props, QCM_MQTT_PROPERTY_CODE_USER_PROPERTY);
                if (connectack_prop != QOSA_NULL)
                {
                    qosa_memcpy(&connack_pkt->options->user_property[i], &connectack_prop->value, sizeof(qcm_mqtt_user_property_t));
                    connectack_prop->identifier = 0;
                    connectack_prop = QOSA_NULL;
                    connack_pkt->options->user_property_cnt++;
                }
                else
                {
                    break;
                }
            }

            qcm_mqtt_properties_free(&connectack_props);
        }
        else
        {
            return MQTTPACKET_BUFFER_TOO_SHORT;
        }
    }
#endif /* CONFIG_QCM_MQTT5_FUNC */

    return MQTTPACKET_READ_COMPLETE;
}

/**
 * @brief Parse the SUBSCRIBE ACK packet received by the MQTT client.
 *
 * @param[in] qcm_mqtt_input_info_t * rb
 *           - Data buffer for the SUBACK to be parsed.
 * @param[in] mqtt_suback_packet * suback_pkt
 *           - Stores the parsed data of SUBACK.
 * @param[in] qosa_uint32_t remain_len
 *           - Remaining length field value.
 *
 * @return mqtt_packet_errors
 *        - Returns MQTTPACKET_READ_COMPLETE to indicate successful unpacking.
 */
mqtt_packet_errors qcm_mqtt_deserialize_subscribe_ack(qcm_mqtt_input_info_t *rb, mqtt_suback_packet *suback_pkt, qosa_uint32_t remain_len)
{
    mqtt_packet_errors ret = MQTTPACKET_READ_COMPLETE;

    if (remain_len >= 2)
    {
        suback_pkt->msg_id = qcm_mqtt_read_int(rb, rb->buf_pos);
        remain_len -= 2;
    }
    else
    {
        ret = MQTTPACKET_BUFFER_TOO_SHORT;
        goto exit;
    }

#ifdef CONFIG_QCM_MQTT5_FUNC
    if (suback_pkt->version >= QCM_MQTT_VERSION_V5 && remain_len > 0)
    {
        qcm_mqtt_properties_t suback_props = {0};
        qcm_mqtt_property_t  *suback_prop = QOSA_NULL;
        qosa_uint16_t         current_pos = rb->buf_pos;

        if (qcm_mqtt_properties_read(&suback_props, rb, remain_len) == 1)
        {
            if (suback_props.count > 0)
            {
                suback_pkt->options = qosa_malloc(sizeof(qcm_mqtt_suback_properties_t));
                if (suback_pkt->options == QOSA_NULL)
                {
                    qcm_mqtt_properties_free(&suback_props);
                    ret = MQTTPACKET_READ_ERROR;
                    goto exit;
                }

                qosa_uint8_t i = 0;
                for (; i < 5; i++)
                {
                    suback_prop = qcm_mqtt_properties_get_property(&suback_props, QCM_MQTT_PROPERTY_CODE_USER_PROPERTY);
                    if (suback_prop != QOSA_NULL)
                    {
                        qosa_memcpy(&suback_pkt->options->user_property[i], &suback_prop->value, sizeof(qcm_mqtt_user_property_t));
                        suback_prop->identifier = 0;
                        suback_prop = QOSA_NULL;
                        suback_pkt->options->user_property_cnt++;
                    }
                    else
                    {
                        break;
                    }
                }

                qcm_mqtt_properties_free(&suback_props);
            }
        }
        else
        {
            ret = MQTTPACKET_BUFFER_TOO_SHORT;
            goto exit;
        }
        remain_len -= rb->buf_pos - current_pos;
    }
#endif /* CONFIG_QCM_MQTT5_FUNC */

    if (remain_len > 0)
    {
        suback_pkt->qos_cnt = remain_len;
        QLOGI("qos_cnt:%d", suback_pkt->qos_cnt);

        suback_pkt->qoss = qosa_malloc(suback_pkt->qos_cnt);
        if (suback_pkt->qoss == QOSA_NULL)
        {
            ret = MQTTPACKET_READ_ERROR;
            goto exit;
        }

        qosa_memcpy(suback_pkt->qoss, rb->buff + rb->buf_pos, suback_pkt->qos_cnt);
        rb->buf_pos += suback_pkt->qos_cnt;
    }
    else
    {
        ret = MQTTPACKET_BUFFER_TOO_SHORT;
    }

exit:
    QLOGI("ret:%d", ret);
    return ret;
}

/**
 * @brief
 *
 * @param[in] rb
 * @param[in] mqtt_ack_pkt
 * @param[in] remain_len
 * @return mqtt_packet_errors
 */
mqtt_packet_errors qcm_mqtt_deserialize_ack(qcm_mqtt_input_info_t *rb, mqtt_ack *mqtt_ack_pkt, qosa_uint32_t remain_len)
{
    mqtt_packet_errors ret = MQTTPACKET_READ_ERROR;

    if (mqtt_ack_pkt->header.bits.type != MQTT_DISCONNECT
#ifdef CONFIG_QCM_MQTT5_FUNC
        && mqtt_ack_pkt->header.bits.type != MQTT_AUTH)
#else
    )
#endif /* CONFIG_QCM_MQTT5_FUNC */
    {
        if (remain_len >= 2)
        {
            mqtt_ack_pkt->msg_id = qcm_mqtt_read_int(rb, rb->buf_pos);
            remain_len -= 2;
            ret = MQTTPACKET_READ_COMPLETE;
        }
        else
        {
            ret = MQTTPACKET_BUFFER_TOO_SHORT;
            goto exit;
        }
    }

#ifdef CONFIG_QCM_MQTT5_FUNC
    if (mqtt_ack_pkt->version >= QCM_MQTT_VERSION_V5 && remain_len > 0)
    {
        mqtt_ack_pkt->reason_code = qcm_mqtt_read_char(rb);
        remain_len--;

        qcm_mqtt_properties_t ack_props = {0};
        qcm_mqtt_property_t  *ack_prop = QOSA_NULL;
        qosa_uint16_t         current_pos = rb->buf_pos;

        if (remain_len > 0 && qcm_mqtt_properties_read(&ack_props, rb, remain_len) == 1 && ack_props.length > 0)
        {
            mqtt_ack_pkt->options = qosa_malloc(sizeof(qcm_mqtt_ack_properties_t));
            if (mqtt_ack_pkt->options == QOSA_NULL)
            {
                qcm_mqtt_properties_free(&ack_props);
                ret = MQTTPACKET_READ_ERROR;
                goto exit;
            }

            qosa_uint8_t i = 0;
            for (; i < 5; i++)
            {
                ack_prop = qcm_mqtt_properties_get_property(&ack_props, QCM_MQTT_PROPERTY_CODE_USER_PROPERTY);
                if (ack_prop != QOSA_NULL)
                {
                    qosa_memcpy(&mqtt_ack_pkt->options->user_property[i], &ack_prop->value, sizeof(qcm_mqtt_user_property_t));
                    ack_prop->identifier = 0;
                    ack_prop = QOSA_NULL;
                    mqtt_ack_pkt->options->user_property_cnt++;
                }
            }

            qcm_mqtt_properties_free(&ack_props);
        }
        remain_len -= rb->buf_pos - current_pos;
    }
#endif /* CONFIG_QCM_MQTT5_FUNC */

exit:
    QLOGI("ret:%d", ret);
    return ret;
}

/**
 * @brief Parse the UNSUBACK packet received by the MQTT client.
 *
 * @param[in] qcm_mqtt_input_info_t * rb
 *           - The data buffer of the UNSUBACK to be parsed.
 * @param[in] mqtt_unsuback_packet * mqtt_ack_pkt
 *           - Stores the parsed data of UNSUBACK.
 * @param[in] qosa_uint32_t remain_len
 *           - Remaining length field value.
 * @return mqtt_packet_errors
 *        - Returns MQTTPACKET_READ_COMPLETE to indicate successful unpacking.
 */
mqtt_packet_errors qcm_mqtt_deserialize_unsuback(qcm_mqtt_input_info_t *rb, mqtt_unsuback_packet *unsuback_pkt, qosa_uint32_t remain_len)
{
    QOSA_UNUSED(remain_len);
    unsuback_pkt->msg_id = qcm_mqtt_read_int(rb, rb->buf_pos);
    remain_len -= 2;

#ifdef CONFIG_QCM_MQTT5_FUNC
    if (unsuback_pkt->version >= QCM_MQTT_VERSION_V5 && remain_len > 0)
    {
        qcm_mqtt_properties_t unsuback_props = {0};
        qcm_mqtt_property_t  *unsuback_prop = QOSA_NULL;
        qosa_uint16_t         i = 0;
        qosa_uint16_t         current_pos = rb->buf_pos;

        if (qcm_mqtt_properties_read(&unsuback_props, rb, remain_len) == 1 && unsuback_props.length > 0)
        {
            unsuback_pkt->options = qosa_malloc(sizeof(qcm_mqtt_ack_properties_t));
            if (unsuback_pkt->options == QOSA_NULL)
            {
                qcm_mqtt_properties_free(&unsuback_props);
                return MQTTPACKET_READ_ERROR;
            }

            for (; i < 5; i++)
            {
                unsuback_prop = qcm_mqtt_properties_get_property(&unsuback_props, QCM_MQTT_PROPERTY_CODE_USER_PROPERTY);
                if (unsuback_prop != QOSA_NULL)
                {
                    qosa_memcpy(&unsuback_pkt->options->user_property[i], &unsuback_prop->value, sizeof(qcm_mqtt_user_property_t));
                    unsuback_prop->identifier = 0;
                    unsuback_prop = QOSA_NULL;
                    unsuback_pkt->options->user_property_cnt++;
                }
            }

            qcm_mqtt_properties_free(&unsuback_props);
        }
        remain_len -= rb->buf_pos - current_pos;

        if (remain_len == 0)
        {
            return MQTTPACKET_READ_COMPLETE;
        }

        for (i = 0; i < remain_len && i < QCM_MQTT_MAX_TOPIC_NUM; i++)
        {
            unsuback_pkt->reason_code[i] = qcm_mqtt_read_char(rb);
        }

        return MQTTPACKET_READ_COMPLETE;
    }
#endif /* CONFIG_QCM_MQTT5_FUNC */

    return MQTTPACKET_READ_COMPLETE;
}

/**
 * @brief Processing of MQTT keep-alive packets
 *
 * @param[in] qcm_mqtt_input_info_t * rb
 *           - Data buffer to be parsed.
 * @param[in] mqtt_packet * mqtt_pkt
 * @param[in] qosa_uint32_t remain_len
 *
 *
 * @return mqtt_packet_errors
 *        - Returns MQTTPACKET_READ_COMPLETE to indicate successful unpacking.
 */
mqtt_packet_errors qcm_mqtt_deserialize_zero(qcm_mqtt_input_info_t *rb, mqtt_packet *mqtt_pkt, qosa_uint32_t remain_len)
{
    QOSA_UNUSED(rb);
    QOSA_UNUSED(mqtt_pkt);
    QOSA_UNUSED(remain_len);
    return MQTTPACKET_READ_COMPLETE;
}

/**
 * @brief Parse the UNSUBACK packet received by the MQTT client.
 *
 * @param[in] qcm_mqtt_input_info_t * rb
 *           - The PUB data buffer to be parsed.
 * @param[in] mqtt_publish_packet * pub_out_pkt
 *           - Stores the parsed data from PUB.
 *
 * @return mqtt_packet_errors
 *        - Returns MQTTPACKET_READ_COMPLETE to indicate successful unpacking.
 */
mqtt_packet_errors qcm_mqtt_deserialize_publish(qcm_mqtt_input_info_t *rb, mqtt_publish_packet *pub_out_pkt)
{
    mqtt_input_publish_packet *pub_pkt = QOSA_NULL;
    qosa_uint32_t              remain_len = rb->varible_header.remain_len;

    if (rb->input_buff == QOSA_NULL)
    {
        pub_pkt = qosa_malloc(sizeof(mqtt_input_publish_packet));
        if (pub_pkt == QOSA_NULL)
        {
            QLOGE("NULL");
            return MQTTPACKET_BUFFER_TOO_SHORT;
        }
        qosa_memset(pub_pkt, 0x00, sizeof(mqtt_input_publish_packet));

        qosa_memcpy(&pub_pkt->header, &rb->header, sizeof(qcm_mqtt_header));
        rb->input_buff = (void *)pub_pkt;
        pub_pkt->version = pub_out_pkt->version;
    }
    else
    {
        pub_pkt = (mqtt_input_publish_packet *)rb->input_buff;
    }

    if (pub_pkt->topic == QOSA_NULL)
    {
        if ((rb->buf_len < (rb->buf_pos + 2)))
        {
            return MQTTPACKET_READ_ERROR;
        }

        pub_pkt->topic = qosa_malloc(sizeof(qcm_mqtt_packet_buffer));
        if (pub_pkt->topic == QOSA_NULL)
        {
            goto exit;
        }

        pub_pkt->topic->buf_len = qcm_mqtt_read_int(rb, rb->buf_pos);

        pub_pkt->topic->buf_ptr.data_len = pub_pkt->topic->buf_len + 1;
        pub_pkt->topic->buf_ptr.data_ptr = qosa_malloc(sizeof(char) * pub_pkt->topic->buf_ptr.data_len);
        if (pub_pkt->topic->buf_ptr.data_ptr == QOSA_NULL)
        {
            QLOGE("NULL");
            goto exit;
        }
        qosa_memset(pub_pkt->topic->buf_ptr.data_ptr, 0x00, sizeof(char) * pub_pkt->topic->buf_ptr.data_len);
    }

    QLOGV("buf_pos:%d buf_len:%d", pub_pkt->topic->buf_pos, pub_pkt->topic->buf_len);

    if (pub_pkt->topic->buf_pos < pub_pkt->topic->buf_len)
    {
        qosa_uint32_t read_len = 0;

        //Calculate whether the remaining readable length is sufficient for the topic's required length.
        read_len = (pub_pkt->topic->buf_len - pub_pkt->topic->buf_pos) > (qosa_uint32_t)(rb->buf_len - rb->buf_pos)
                       ? (qosa_uint32_t)(rb->buf_len - rb->buf_pos)
                       : (pub_pkt->topic->buf_len - pub_pkt->topic->buf_pos);
        qosa_memcpy(pub_pkt->topic->buf_ptr.data_ptr + pub_pkt->topic->buf_pos, rb->buff + rb->buf_pos, read_len);

        pub_pkt->topic->buf_pos += read_len;
        rb->buf_pos += read_len;

        if (pub_pkt->topic->buf_pos != pub_pkt->topic->buf_len)
        {
            return MQTTPACKET_READ_ERROR;
        }
    }

    if (pub_pkt->payload == QOSA_NULL)
    {
        QLOGV("buf_len:%d buf_pos:%d", rb->buf_len, rb->buf_pos);

        if (pub_pkt->header.bits.qos != 0)
        {
            if (rb->buf_len < rb->buf_pos + 2)
            {
                return MQTTPACKET_READ_ERROR;
            }

            pub_pkt->msg_id = qcm_mqtt_read_int(rb, rb->buf_pos);
            remain_len -= 2;
        }
        else
        {
            pub_pkt->msg_id = 0;
        }

#ifdef CONFIG_QCM_MQTT5_FUNC
        if (pub_pkt->version >= QCM_MQTT_VERSION_V5 && remain_len > 0)
        {
            qcm_mqtt_properties_t pub_props = {0};
            qcm_mqtt_property_t  *pub_prop = QOSA_NULL;
            qosa_uint16_t         current_pos = rb->buf_pos;

            if (qcm_mqtt_properties_read(&pub_props, rb, remain_len) == 1 && pub_props.length > 0)
            {
                pub_pkt->options = qosa_malloc(sizeof(qcm_mqtt_pub_properties_t));
                if (pub_pkt->options == QOSA_NULL)
                {
                    qcm_mqtt_properties_free(&pub_props);
                    return MQTTPACKET_READ_ERROR;
                }

                pub_prop = qcm_mqtt_properties_get_property(&pub_props, QCM_MQTT_PROPERTY_CODE_MESSAGE_EXPIRY_INTERVAL);
                if (pub_prop != QOSA_NULL)
                {
                    pub_pkt->options->message_expiry_interval = pub_prop->value.integer4;
                    pub_prop = QOSA_NULL;
                }

                pub_prop = qcm_mqtt_properties_get_property(&pub_props, QCM_MQTT_PROPERTY_CODE_SUBSCRIPTION_IDENTIFIER);
                if (pub_prop != QOSA_NULL)
                {
                    pub_pkt->options->subscription_identifier = pub_prop->value.integer4;
                    pub_prop = QOSA_NULL;
                }

                pub_prop = qcm_mqtt_properties_get_property(&pub_props, QCM_MQTT_PROPERTY_CODE_TOPIC_ALIAS);
                if (pub_prop != QOSA_NULL)
                {
                    pub_pkt->options->topic_alias = pub_prop->value.integer2;
                    pub_prop = QOSA_NULL;
                }

                pub_prop = qcm_mqtt_properties_get_property(&pub_props, QCM_MQTT_PROPERTY_CODE_PAYLOAD_FORMAT_INDICATOR);
                if (pub_prop != QOSA_NULL)
                {
                    pub_pkt->options->payload_format_indicator = pub_prop->value.byte;
                    pub_prop = QOSA_NULL;
                }

                pub_prop = qcm_mqtt_properties_get_property(&pub_props, QCM_MQTT_PROPERTY_CODE_RESPONSE_TOPIC);
                if (pub_prop != QOSA_NULL)
                {
                    pub_pkt->options->response_topic.data_len = pub_prop->value.data.data_len;
                    pub_pkt->options->response_topic.data_ptr = pub_prop->value.data.data_ptr;
                    pub_prop->identifier = 0;
                }

                pub_prop = qcm_mqtt_properties_get_property(&pub_props, QCM_MQTT_PROPERTY_CODE_CONTENT_TYPE);
                if (pub_prop != QOSA_NULL)
                {
                    pub_pkt->options->content_type.data_len = pub_prop->value.data.data_len;
                    pub_pkt->options->content_type.data_ptr = pub_prop->value.data.data_ptr;
                    pub_prop->identifier = 0;
                }

                pub_prop = qcm_mqtt_properties_get_property(&pub_props, QCM_MQTT_PROPERTY_CODE_CORRELATION_DATA);
                if (pub_prop != QOSA_NULL)
                {
                    pub_pkt->options->correlation_data.data_len = pub_prop->value.data.data_len;
                    pub_pkt->options->correlation_data.data_ptr = pub_prop->value.data.data_ptr;
                    pub_prop->identifier = 0;
                }

                qosa_uint8_t i = 0;
                for (; i < 5; i++)
                {
                    pub_prop = qcm_mqtt_properties_get_property(&pub_props, QCM_MQTT_PROPERTY_CODE_USER_PROPERTY);
                    if (pub_prop != QOSA_NULL)
                    {
                        qosa_memcpy(&pub_pkt->options->user_property[i], &pub_prop->value, sizeof(qcm_mqtt_user_property_t));
                        pub_prop->identifier = 0;
                        pub_prop = QOSA_NULL;
                        pub_pkt->options->user_property_cnt++;
                    }
                }

                qcm_mqtt_properties_free(&pub_props);
            }
            remain_len -= rb->buf_pos - current_pos;
        }
#endif /* CONFIG_QCM_MQTT5_FUNC */

        pub_pkt->payload = qosa_malloc(sizeof(qcm_mqtt_packet_buffer));
        if (pub_pkt->payload == QOSA_NULL)
        {
            QLOGE("NULL");
            goto exit;
        }
        qosa_memset(pub_pkt->payload, 0x00, sizeof(qcm_mqtt_packet_buffer));

        pub_pkt->payload->buf_len = remain_len - pub_pkt->topic->buf_len - 2;
        pub_pkt->payload->buf_ptr.data_len = pub_pkt->payload->buf_len + 1;
        pub_pkt->payload->buf_ptr.data_ptr = qosa_malloc(sizeof(char) * pub_pkt->payload->buf_ptr.data_len);
        if (pub_pkt->payload->buf_ptr.data_ptr == QOSA_NULL)
        {
            QLOGE("NULL");
            goto exit;
        }
        qosa_memset(pub_pkt->payload->buf_ptr.data_ptr, 0x00, sizeof(char) * pub_pkt->payload->buf_ptr.data_len);
    }

    QLOGV("topic:%d, payload:%d", pub_pkt->topic->buf_len, pub_pkt->payload->buf_len);

    if (pub_pkt->payload->buf_pos < pub_pkt->payload->buf_len)
    {
        qosa_uint32_t read_len = 0;

        //Calculate whether the remaining readable length is sufficient for the topic's required length.
        read_len = (pub_pkt->payload->buf_len - pub_pkt->payload->buf_pos) > (qosa_uint32_t)(rb->buf_len - rb->buf_pos)
                       ? (qosa_uint32_t)(rb->buf_len - rb->buf_pos)
                       : (pub_pkt->payload->buf_len - pub_pkt->payload->buf_pos);
        qosa_memcpy(pub_pkt->payload->buf_ptr.data_ptr + pub_pkt->payload->buf_pos, rb->buff + rb->buf_pos, read_len);

        pub_pkt->payload->buf_pos += read_len;
        rb->buf_pos += read_len;

        if (pub_pkt->payload->buf_pos != pub_pkt->payload->buf_len)
        {
            return MQTTPACKET_READ_ERROR;
        }
    }

    pub_out_pkt->header.byte = pub_pkt->header.byte;
    pub_out_pkt->msg_id = pub_pkt->msg_id;
    pub_out_pkt->topic.data_len = (qosa_uint16_t)pub_pkt->topic->buf_ptr.data_len;
    pub_out_pkt->topic.data_ptr = pub_pkt->topic->buf_ptr.data_ptr;
    pub_out_pkt->payload.data_len = pub_pkt->payload->buf_len;
    pub_out_pkt->payload.data_ptr = pub_pkt->payload->buf_ptr.data_ptr;
#ifdef CONFIG_QCM_MQTT5_FUNC
    pub_out_pkt->options = pub_pkt->options;
#endif /* CONFIG_QCM_MQTT5_FUNC */

    qosa_free(pub_pkt->topic);
    qosa_free(pub_pkt->payload);
    qosa_free(pub_pkt);
    rb->input_buff = QOSA_NULL;

    return MQTTPACKET_READ_COMPLETE;

exit:
    qcm_mqtt_free_data(&pub_pkt->topic->buf_ptr);
    qcm_mqtt_free_data(&pub_pkt->payload->buf_ptr);

#ifdef CONFIG_QCM_MQTT5_FUNC
    if (pub_pkt->options != QOSA_NULL)
    {
        qcm_mqtt_free_data(&pub_pkt->options->response_topic);
        qcm_mqtt_free_data(&pub_pkt->options->content_type);
        qcm_mqtt_free_data(&pub_pkt->options->correlation_data);
        qcm_mqtt_user_property_free_data(pub_pkt->options->user_property_cnt, pub_pkt->options->user_property);

        qosa_free(pub_pkt->options);
    }
#endif /* CONFIG_QCM_MQTT5_FUNC */

    qosa_free(pub_pkt);
    rb->input_buff = QOSA_NULL;
    return MQTTPACKET_BUFFER_TOO_SHORT;
}
