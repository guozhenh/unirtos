/*****************************************************************/ /**
* @file qcm_mqtt_properties.c
* @brief
* @author lawrence.liu@quectel.com
* @date 2025-06-30
*
* @copyright Copyright (c) 2025 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description
* <tr><td>2025-06-30 <td>1.0 <td>lawrence.liu <td> Init
* </table>
**********************************************************************/

#include "qcm_mqtt_properties.h"
#include "qcm_mqtt.h"
#include "qcm_mqtt_packet.h"
#include "qosa_log.h"

/** Redefine the TAG of this file */
#define QOS_LOG_TAG LOG_TAG_MQTT_API
#ifdef CONFIG_QCM_MQTT5_FUNC
static struct osa_name_to_type
{
    qcm_mqtt_property_code_e  name;
    qcm_mqtt_property_types_e type;
} osa_name_to_types[]
    = {{QCM_MQTT_PROPERTY_CODE_PAYLOAD_FORMAT_INDICATOR, QCM_MQTT_PROPERTY_TYPE_BYTE},
       {QCM_MQTT_PROPERTY_CODE_MESSAGE_EXPIRY_INTERVAL, QCM_MQTT_PROPERTY_TYPE_FOUR_BYTE_INTEGER},
       {QCM_MQTT_PROPERTY_CODE_CONTENT_TYPE, QCM_MQTT_PROPERTY_TYPE_UTF_8_ENCODED_STRING},
       {QCM_MQTT_PROPERTY_CODE_RESPONSE_TOPIC, QCM_MQTT_PROPERTY_TYPE_UTF_8_ENCODED_STRING},
       {QCM_MQTT_PROPERTY_CODE_CORRELATION_DATA, QCM_MQTT_PROPERTY_TYPE_BINARY_DATA},
       {QCM_MQTT_PROPERTY_CODE_SUBSCRIPTION_IDENTIFIER, QCM_MQTT_PROPERTY_TYPE_VARIABLE_BYTE_INTEGER},
       {QCM_MQTT_PROPERTY_CODE_SESSION_EXPIRY_INTERVAL, QCM_MQTT_PROPERTY_TYPE_FOUR_BYTE_INTEGER},
       {QCM_MQTT_PROPERTY_CODE_ASSIGNED_CLIENT_IDENTIFIER, QCM_MQTT_PROPERTY_TYPE_UTF_8_ENCODED_STRING},
       {QCM_MQTT_PROPERTY_CODE_SERVER_KEEP_ALIVE, QCM_MQTT_PROPERTY_TYPE_TWO_BYTE_INTEGER},
       {QCM_MQTT_PROPERTY_CODE_AUTHENTICATION_METHOD, QCM_MQTT_PROPERTY_TYPE_UTF_8_ENCODED_STRING},
       {QCM_MQTT_PROPERTY_CODE_AUTHENTICATION_DATA, QCM_MQTT_PROPERTY_TYPE_BINARY_DATA},
       {QCM_MQTT_PROPERTY_CODE_REQUEST_PROBLEM_INFORMATION, QCM_MQTT_PROPERTY_TYPE_BYTE},
       {QCM_MQTT_PROPERTY_CODE_WILL_DELAY_INTERVAL, QCM_MQTT_PROPERTY_TYPE_FOUR_BYTE_INTEGER},
       {QCM_MQTT_PROPERTY_CODE_REQUEST_RESPONSE_INFORMATION, QCM_MQTT_PROPERTY_TYPE_BYTE},
       {QCM_MQTT_PROPERTY_CODE_RESPONSE_INFORMATION, QCM_MQTT_PROPERTY_TYPE_UTF_8_ENCODED_STRING},
       {QCM_MQTT_PROPERTY_CODE_SERVER_REFERENCE, QCM_MQTT_PROPERTY_TYPE_UTF_8_ENCODED_STRING},
       {QCM_MQTT_PROPERTY_CODE_REASON_STRING, QCM_MQTT_PROPERTY_TYPE_UTF_8_ENCODED_STRING},
       {QCM_MQTT_PROPERTY_CODE_RECEIVE_MAXIMUM, QCM_MQTT_PROPERTY_TYPE_TWO_BYTE_INTEGER},
       {QCM_MQTT_PROPERTY_CODE_TOPIC_ALIAS_MAXIMUM, QCM_MQTT_PROPERTY_TYPE_TWO_BYTE_INTEGER},
       {QCM_MQTT_PROPERTY_CODE_TOPIC_ALIAS, QCM_MQTT_PROPERTY_TYPE_TWO_BYTE_INTEGER},
       {QCM_MQTT_PROPERTY_CODE_MAXIMUM_QOS, QCM_MQTT_PROPERTY_TYPE_BYTE},
       {QCM_MQTT_PROPERTY_CODE_RETAIN_AVAILABLE, QCM_MQTT_PROPERTY_TYPE_BYTE},
       {QCM_MQTT_PROPERTY_CODE_USER_PROPERTY, QCM_MQTT_PROPERTY_TYPE_UTF_8_STRING_PAIR},
       {QCM_MQTT_PROPERTY_CODE_MAXIMUM_PACKET_SIZE, QCM_MQTT_PROPERTY_TYPE_FOUR_BYTE_INTEGER},
       {QCM_MQTT_PROPERTY_CODE_WILDCARD_SUBSCRIPTION_AVAILABLE, QCM_MQTT_PROPERTY_TYPE_BYTE},
       {QCM_MQTT_PROPERTY_CODE_SUBSCRIPTION_IDENTIFIERS_AVAILABLE, QCM_MQTT_PROPERTY_TYPE_BYTE},
       {QCM_MQTT_PROPERTY_CODE_SHARED_SUBSCRIPTION_AVAILABLE, QCM_MQTT_PROPERTY_TYPE_BYTE}};

/**
 *  @brief Copy data.
 *
 * @param[in] qcm_mqtt_data_t* str
 *           - Pointer to the qcm_mqtt_data_t structure, which contains the data to be copied and its length.
 * @return qosa_uint8_t*
 *        - Returns a pointer to newly allocated memory containing the copied data.
 *          If memory allocation fails, return OSA_NULL.
 */
static char* osa_datadup(qcm_mqtt_data_t* str)
{
    if (str->data_len == 0)
    {
        return QOSA_NULL;  //If the data length is 0, return NULL directly.
    }

    char* temp = qosa_malloc(str->data_len + 1);
    if (temp != QOSA_NULL)
    {
        qosa_memset(temp, 0, str->data_len + 1);
        qosa_memcpy(temp, str->data_ptr, str->data_len);
    }

    return temp;
}

/**
 *  @brief Get the type of MQTT 5.0 properties.
 *
 * @param[in] qcm_mqtt_property_code_e value
 *           - The input MQTT 5.0 property ID value.
 * @return qcm_mqtt_property_types_e
 *        - Returns the corresponding MQTT 5.0 property type.
 */
qcm_mqtt_property_types_e qcm_mqtt_property_get_type(qcm_mqtt_property_code_e value)
{
    qosa_size_t               i;
    qcm_mqtt_property_types_e rc = (qcm_mqtt_property_types_e)-1;

    for (i = 0; i < QOSA_ARRAY_SIZE(osa_name_to_types); ++i)
    {
        if (osa_name_to_types[i].name == value)
        {
            rc = osa_name_to_types[i].type;
            break;
        }
    }
    return rc;
}

/**
 * @brief Free the memory occupied by the MQTT property list.
 *
 * @param[in] qcm_mqtt_properties_t* * props
 *           - Pointer to the list of MQTT attributes to be released.
 */
void qcm_mqtt_properties_free(qcm_mqtt_properties_t* props)
{
    qosa_uint32_t i = 0;
    qosa_int32_t  id = 0;
    qosa_int32_t  type = 0;

    if (props == QOSA_NULL)
    {
        goto exit;
    }

    for (i = 0; i < props->count; i++)
    {
        id = props->array[i].identifier;
        type = qcm_mqtt_property_get_type(id);

        switch (type)
        {
            case QCM_MQTT_PROPERTY_TYPE_BINARY_DATA:
            case QCM_MQTT_PROPERTY_TYPE_UTF_8_ENCODED_STRING:
            case QCM_MQTT_PROPERTY_TYPE_UTF_8_STRING_PAIR:
                qosa_free(props->array[i].value.data.data_ptr);
                if (type == QCM_MQTT_PROPERTY_TYPE_UTF_8_STRING_PAIR)
                {
                    qosa_free(props->array[i].value.value.data_ptr);
                }
                break;
        }
    }

    if (props->array)
    {
        qosa_free(props->array);
    }
    qosa_memset(props, '\0', sizeof(qcm_mqtt_properties_t)); /* zero all fields */
exit:
    return;
}

/**
 *  @brief Calculate the length of the MQTT property field.
 *
 * @param[in] qcm_mqtt_properties_t* props
 *           - Pointer to the qcm_mqtt_properties_t structure, which contains the property list.
 * @return qosa_uint32_t
 *        - Returns the length of the attribute list and group header in bytes.
 */
qosa_uint32_t qcm_mqtt_properties_len(qcm_mqtt_properties_t* props)
{
    /* properties length is an mbi */
    return (props == QOSA_NULL) ? 1 : props->length + qcm_mqtt_packet_vbi_len(props->length);
}

/**
 *  @brief Store the data into the property structure.
 *
 * @param[in] qcm_mqtt_properties_t* props
 *           - Attribute structure pointer
 * @param[in] qcm_mqtt_property_code_e identifier
 *           - Enumeration of IDs for storing data
 * @param[in] void * data
 *           - Data type pointer
 *
 * @return qcm_mqtt_eercode_e
 *        - Returns QCM_MQTT_RES_OK for success, other values indicate failure
 */
qcm_mqtt_eercode_e qcm_mqtt_data_to_property(qcm_mqtt_properties_t* props, qcm_mqtt_property_code_e identifier, void* data)
{
    qosa_int32_t        type = -1;
    qcm_mqtt_eercode_e  ret = QCM_MQTT_RES_OK;
    qcm_mqtt_property_t property_info = {0};

    if (props == QOSA_NULL || data == QOSA_NULL)
    {
        return QCM_MQTT_RES_ERROR_PARAM_INVALID;
    }

    property_info.identifier = identifier;

    type = qcm_mqtt_property_get_type(identifier);
    switch (type)
    {
        case QCM_MQTT_PROPERTY_TYPE_BYTE: {
            property_info.value.byte = *(qosa_uint8_t*)data;
        }
        break;
        case QCM_MQTT_PROPERTY_TYPE_TWO_BYTE_INTEGER: {
            property_info.value.integer2 = *(qosa_uint16_t*)data;
        }
        break;
        case QCM_MQTT_PROPERTY_TYPE_VARIABLE_BYTE_INTEGER:
        case QCM_MQTT_PROPERTY_TYPE_FOUR_BYTE_INTEGER: {
            property_info.value.integer4 = *(qosa_uint32_t*)data;
        }
        break;
        case QCM_MQTT_PROPERTY_TYPE_BINARY_DATA:
        case QCM_MQTT_PROPERTY_TYPE_UTF_8_ENCODED_STRING: {
            qcm_mqtt_data_t* buff = (qcm_mqtt_data_t*)data;

            property_info.value.data.data_ptr = buff->data_ptr;
            property_info.value.data.data_len = buff->data_len;
        }
        break;
        case QCM_MQTT_PROPERTY_TYPE_UTF_8_STRING_PAIR: {
            qcm_mqtt_user_property_t* buff = (qcm_mqtt_user_property_t*)data;

            property_info.value.data.data_ptr = buff->key.data_ptr;
            property_info.value.data.data_len = buff->key.data_len;
            property_info.value.value.data_ptr = buff->value.data_ptr;
            property_info.value.value.data_len = buff->value.data_len;
        }
        break;
    }

    qcm_mqtt_properties_add(props, &property_info);

    return ret;
}

/**
 * Add a new property to a property list
 * @param props the property list
 * @param prop the new property
 * @return code 0 is success
 */
qcm_mqtt_eercode_e qcm_mqtt_properties_add(qcm_mqtt_properties_t* props, qcm_mqtt_property_t* prop)
{
    qcm_mqtt_eercode_e        rc = 0;
    qcm_mqtt_property_types_e type = QCM_MQTT_PROPERTY_TYPE_BYTE;

    if (props == QOSA_NULL)
    {
        rc = QCM_MQTT_RES_ERROR_NO_MEMORY;
        goto exit;
    }
    type = qcm_mqtt_property_get_type(prop->identifier);
    if (type < QCM_MQTT_PROPERTY_TYPE_BYTE || type > QCM_MQTT_PROPERTY_TYPE_UTF_8_STRING_PAIR)
    {
        /*StackTrace_printStack(stdout);*/
        rc = QCM_MQTT_RES_ERROR_INVALID_PROPERTY_ID;
        goto exit;
    }
    else if (props->array == QOSA_NULL)
    {
        props->max_count = 10;
        props->array = qosa_malloc(sizeof(qcm_mqtt_property_t) * props->max_count);
    }
    else if (props->count == props->max_count)
    {
        props->max_count += 10;
        void* newPtr = qosa_realloc(props->array, sizeof(qcm_mqtt_property_t) * props->max_count);
        if (newPtr == QOSA_NULL)
        {
            qosa_free(props->array);
            props->array = QOSA_NULL;
        }
        else
        {
            props->array = newPtr;
        }
    }

    if (props->array)
    {
        int len = 0;

        props->array[props->count++] = *prop;
        /* calculate length */
        switch (type)
        {
            case QCM_MQTT_PROPERTY_TYPE_BYTE:
                len = 1;
                break;
            case QCM_MQTT_PROPERTY_TYPE_TWO_BYTE_INTEGER:
                len = 2;
                break;
            case QCM_MQTT_PROPERTY_TYPE_FOUR_BYTE_INTEGER:
                len = 4;
                break;
            case QCM_MQTT_PROPERTY_TYPE_VARIABLE_BYTE_INTEGER:
                len = qcm_mqtt_packet_vbi_len(prop->value.integer4);
                break;
            case QCM_MQTT_PROPERTY_TYPE_BINARY_DATA:
            case QCM_MQTT_PROPERTY_TYPE_UTF_8_ENCODED_STRING:
            case QCM_MQTT_PROPERTY_TYPE_UTF_8_STRING_PAIR:
                len = 2 + prop->value.data.data_len;
                props->array[props->count - 1].value.data.data_ptr = osa_datadup(&prop->value.data);
                props->array[props->count - 1].value.data.data_len = prop->value.data.data_len;

                if (type == QCM_MQTT_PROPERTY_TYPE_UTF_8_STRING_PAIR)
                {
                    len += 2 + prop->value.value.data_len;
                    props->array[props->count - 1].value.value.data_ptr = osa_datadup(&prop->value.value);
                    props->array[props->count - 1].value.value.data_len = prop->value.value.data_len;
                }
                break;
            default:
                break;
        }
        props->length += len + 1; /* add identifier byte */
    }
    else
    {
        rc = QCM_MQTT_RES_ERROR_NO_MEMORY;
    }

exit:
    return rc;
}

/**
 *  @brief Write a single MQTT attribute to the character buffer.
 *
 * @param[in] qosa_uint8_t** pptr
 *           - A pointer to a pointer to a character buffer, used for writing attribute data.
 * @param[in] qcm_mqtt_property_t* prop
 *           - Pointer to a qcm_mqtt_property_t structure, which contains the property data to be written.
 * @return qosa_int32_t
 *        - Returns the number of bytes written, or a negative value if an error occurs.
 */
static qosa_int32_t qcm_mqtt_property_write(qcm_mqtt_packet_buffer* rb, qcm_mqtt_property_t* prop)
{
    qosa_int32_t rc = -1, type = -1;
    char         rem_len[4] = {0};

    type = qcm_mqtt_property_get_type(prop->identifier);
    if (type >= QCM_MQTT_PROPERTY_TYPE_BYTE && type <= QCM_MQTT_PROPERTY_TYPE_UTF_8_STRING_PAIR)
    {
        qcm_mqtt_write_char(rb, prop->identifier);
        switch (type)
        {
            case QCM_MQTT_PROPERTY_TYPE_BYTE:
                qcm_mqtt_write_char(rb, prop->value.byte);
                rc = 1;
                break;
            case QCM_MQTT_PROPERTY_TYPE_TWO_BYTE_INTEGER:
                qcm_mqtt_write_int(rb, prop->value.integer2);
                rc = 2;
                break;
            case QCM_MQTT_PROPERTY_TYPE_FOUR_BYTE_INTEGER:
                qcm_mqtt_write_int4(rb, prop->value.integer4);
                rc = 4;
                break;
            case QCM_MQTT_PROPERTY_TYPE_VARIABLE_BYTE_INTEGER:
                rc = qcm_mqtt_remaining_length_encode(rem_len, prop->value.integer4);
                qcm_mqtt_write_string(rb, (void*)&rem_len[0], rc);
                break;
            case QCM_MQTT_PROPERTY_TYPE_BINARY_DATA:
            case QCM_MQTT_PROPERTY_TYPE_UTF_8_ENCODED_STRING:
                qcm_mqtt_write_int(rb, prop->value.data.data_len);
                qcm_mqtt_write_string(rb, prop->value.data.data_ptr, prop->value.data.data_len);
                rc = prop->value.data.data_len + 2; /* include length field */
                break;
            case QCM_MQTT_PROPERTY_TYPE_UTF_8_STRING_PAIR:
                qcm_mqtt_write_int(rb, prop->value.data.data_len);
                qcm_mqtt_write_string(rb, prop->value.data.data_ptr, prop->value.data.data_len);

                qcm_mqtt_write_int(rb, prop->value.value.data_len);
                qcm_mqtt_write_string(rb, prop->value.value.data_ptr, prop->value.value.data_len);
                rc = prop->value.data.data_len + prop->value.value.data_len + 4; /* include length fields */
                break;
        }
    }
    return rc + 1; /* include identifier byte */
}

/**
 *  @brief Write the MQTT property list to the character buffer.
 *
 * @param[in] qcm_mqtt_packet_buffer* pptr
 *           - A pointer to a pointer to a character buffer, used for writing attribute data.
 * @param[in] qcm_mqtt_properties_t* properties
 *           - Pointer to the qcm_mqtt_properties_t structure, which contains the property list.
 * @return qosa_int32_t
 *        - Returns the number of bytes written, or a negative value if an error occurs.
 */
qosa_int32_t qcm_mqtt_properties_write(qcm_mqtt_packet_buffer* rb, qcm_mqtt_properties_t* properties)
{
    qosa_int32_t  rc = -1;
    qosa_uint32_t i = 0, len = 0;
    char          rem_len[4] = {0};

    /* write the entire property list length first */
    if (properties == QOSA_NULL)
    {
        qcm_mqtt_write_char(rb, 0);
        rc = 1;
    }
    else
    {
        rc = qcm_mqtt_remaining_length_encode(rem_len, properties->length);
        qcm_mqtt_write_string(rb, (void*)&rem_len[0], rc);
        len = rc = 1;

        for (i = 0; i < properties->count; ++i)
        {
            rc = qcm_mqtt_property_write(rb, &properties->array[i]);
            if (rc < 0)
            {
                break;
            }
            else
            {
                len += rc;
            }
        }

        if (rc >= 0)
        {
            rc = len;
        }
    }
    return rc;
}

/**
 *  @brief Parse the attributes in the data packet and store the parsing results into the attribute structure.
 *
 * @param[out] qcm_mqtt_property_t* prop
 *           - Pointer to the qcm_mqtt_properties_t structure, which is used to store the read property list.
 * @param[in] qcm_mqtt_input_info_t*  rb
 *           - Pointer to the qcm_mqtt_input_info_t structure, which contains input buffer information.
 *
 * @return qosa_int32_t
 */
qosa_int32_t qcm_mqtt_property_read(qcm_mqtt_property_t* prop, qcm_mqtt_input_info_t* rb)
{
    qosa_int32_t type = -1, len = -1;

    qosa_memset(prop, 0, sizeof(qcm_mqtt_property_t));
    prop->identifier = qcm_mqtt_read_char(rb);
    type = qcm_mqtt_property_get_type(prop->identifier);

    QLOGI("identifier:%d type:%d", prop->identifier, type);

    if (type >= QCM_MQTT_PROPERTY_TYPE_BYTE && type <= QCM_MQTT_PROPERTY_TYPE_UTF_8_STRING_PAIR)
    {
        switch (type)
        {
            case QCM_MQTT_PROPERTY_TYPE_BYTE:
                prop->value.byte = qcm_mqtt_read_char(rb);
                QLOGV("integer:%d", prop->value.byte);
                len = 1;
                break;
            case QCM_MQTT_PROPERTY_TYPE_TWO_BYTE_INTEGER:
                prop->value.integer2 = qcm_mqtt_read_int(rb, rb->buf_pos);
                QLOGV("integer2:%d", prop->value.integer2);
                len = 2;
                break;
            case QCM_MQTT_PROPERTY_TYPE_FOUR_BYTE_INTEGER:
                prop->value.integer4 = qcm_mqtt_read_int4(rb, rb->buf_pos);
                QLOGV("integer4:%d", prop->value.integer4);
                len = 4;
                break;
            case QCM_MQTT_PROPERTY_TYPE_VARIABLE_BYTE_INTEGER:
                len = qcm_mqtt_remaining_length_decode((const char*)(rb->buff + rb->buf_pos), 4, &prop->value.integer4);
                if(len > 0)
                {
                    rb->buf_pos += len;
                }
                break;
            case QCM_MQTT_PROPERTY_TYPE_BINARY_DATA:
            case QCM_MQTT_PROPERTY_TYPE_UTF_8_ENCODED_STRING:
            case QCM_MQTT_PROPERTY_TYPE_UTF_8_STRING_PAIR:
                if ((len = qcm_mqtt_read_string(&prop->value.data, rb)) == -1)
                {
                    break; /* error */
                }

                //The module neither parses nor stores the reason string.
                if (prop->identifier == QCM_MQTT_PROPERTY_CODE_REASON_STRING)
                {
                    prop->value.data.data_len = 0;
                    prop->value.data.data_ptr = QOSA_NULL;
                    break;
                }

                if ((prop->value.data.data_ptr = osa_datadup(&prop->value.data)) == QOSA_NULL)
                {
                    len = -1;
                    break; /* error */
                }

                if (type == QCM_MQTT_PROPERTY_TYPE_UTF_8_STRING_PAIR)
                {
                    int proplen = qcm_mqtt_read_string(&prop->value.value, rb);

                    if (proplen == -1)
                    {
                        len = -1;
                        qosa_free(prop->value.data.data_ptr);
                        break;
                    }
                    len += proplen;

                    if ((prop->value.value.data_ptr = osa_datadup(&prop->value.value)) == QOSA_NULL)
                    {
                        len = -1;
                        qosa_free(prop->value.data.data_ptr);
                        break;
                    }
                }
                break;
        }
    }
    return (len == -1) ? -1 : len + 1; /* 1 byte for identifier */
}

/**
 *  @brief Read MQTT attribute list.
 *
 * @param[in] qcm_mqtt_properties_t * properties
 *           - Pointer to the qcm_mqtt_properties_t structure, which is used to store the read property list.
 * @param[in] qcm_mqtt_input_info_t * rb
 *           - Pointer to the qcm_mqtt_input_info_t structure, which contains input buffer information.
 * @param[in] qosa_uint32_t remain_len
 *           - Remaining length, used to indicate the number of bytes to read.
 * @return qosa_int32_t
 *       - Returns the number of bytes read, or a negative value if an error occurs.
 */
qosa_int32_t qcm_mqtt_properties_read(qcm_mqtt_properties_t* properties, qcm_mqtt_input_info_t* rb, qosa_uint32_t remain_len)
{
    qosa_int32_t  rc = 0;
    qosa_uint32_t remlength = 0;

    if (remain_len > 0)
    {
        qosa_int32_t proplen = 0;

        rc = qcm_mqtt_remaining_length_decode((const char*)(rb->buff + rb->buf_pos), 4, &remlength);
        if (rc < 0)
        {
            return -1;
        }
        rb->buf_pos += rc;
        properties->length = remlength;

        if (remain_len < remlength)
        {
            return -1;
        }

        while (remlength > 0)
        {
            if (properties->count == properties->max_count)
            {
                properties->max_count += 10;
                if (properties->max_count == 10)
                {
                    properties->array = qosa_malloc(sizeof(qcm_mqtt_property_t) * properties->max_count);
                }
                else
                {
                    void* newPtr = qosa_realloc(properties->array, sizeof(qcm_mqtt_property_t) * properties->max_count);
                    if (newPtr == QOSA_NULL)
                    {
                        qosa_free(properties->array);
                        properties->array = QOSA_NULL;
                    }
                    else
                    {
                        properties->array = newPtr;
                    }
                }
            }
            if (properties->array == QOSA_NULL)
            {
                rc = QCM_MQTT_RES_ERROR_NO_MEMORY;
                goto exit;
            }
            if ((proplen = qcm_mqtt_property_read(&properties->array[properties->count], rb)) > 0)
            {
                remlength -= proplen;
            }
            else
            {
                break;
            }

            properties->count++;
        }

        if (remlength == 0)
        {
            rc = 1; /* data read successfully */
        }
    }

    if ((rc != 1 && properties->array != QOSA_NULL) 
        || (properties->count == 1  && properties->array != QOSA_NULL && properties->array[0].identifier == QCM_MQTT_PROPERTY_CODE_REASON_STRING))
    {
        rc = QCM_MQTT_RES_ERROR_INVALID_PROPERTY_ID;
        qcm_mqtt_properties_free(properties);
    }

exit:
    return rc;
}

/**
 *  @brief Check whether the specified attribute is included in the MQTT attribute list.
 *
 * @param[in] qcm_mqtt_properties_t* props
 *           - Pointer to the qcm_mqtt_properties_t structure, which contains the property list.
 * @param[in] qcm_mqtt_property_code_e propid
 *           - Enumeration of IDs for storing data
 *
 * @return qosa_int32_t
 */
qosa_bool_t qcm_mqtt_properties_has_property(qcm_mqtt_properties_t* props, qcm_mqtt_property_code_e propid)
{
    qosa_uint32_t i = 0;
    qosa_bool_t   found = QOSA_FALSE;

    for (i = 0; props != QOSA_NULL && i < props->count; ++i)
    {
        if (propid == props->array[i].identifier)
        {
            found = QOSA_TRUE;
            break;
        }
    }
    return found;
}

/**
 *  @brief Calculate the number of MQTT properties.
 *
 * @param[in] qcm_mqtt_properties_t* props
 *           - Pointer to the qcm_mqtt_properties_t structure, which contains the property list.
 * @param[in] qcm_mqtt_property_code_e propid
 *           - Enumeration of IDs for storing data
 *
 * @return qosa_uint32_t
 */
qosa_uint32_t qcm_mqtt_properties_property_count(qcm_mqtt_properties_t* props, qcm_mqtt_property_code_e propid)
{
    qosa_uint32_t i = 0;
    qosa_uint32_t count = 0;

    for (i = 0; props != QOSA_NULL && i < props->count; ++i)
    {
        if (propid == props->array[i].identifier)
        {
            count++;
        }
    }
    return count;
}

/**
 *  @brief Get the value of the specified attribute.
 *
 * @param[in] qcm_mqtt_properties_t* props
 *           - Pointer to the qcm_mqtt_properties_t structure, which contains the property list.
 * @param[in] qcm_mqtt_property_code_e propid
 *           - Enumeration of IDs for storing data
 * @param[in] qosa_uint32_t index
 *           - If there are multiple identical attributes, this indicates which occurrence of the identical attribute to select
 *
 * @return qosa_int64_t
 */
qosa_int64_t qcm_mqtt_properties_get_numeric_value_at(qcm_mqtt_properties_t* props, qcm_mqtt_property_code_e propid, qosa_uint32_t index)
{
    qosa_uint32_t i = 0;
    qosa_int64_t  rc = -9999999;
    qosa_uint32_t cur_index = 0;

    for (i = 0; props != QOSA_NULL && i < props->count; ++i)
    {
        qosa_uint32_t id = props->array[i].identifier;

        if (id == propid)
        {
            if (cur_index < index)
            {
                cur_index++;
                continue;
            }

            switch (qcm_mqtt_property_get_type(id))
            {
                case QCM_MQTT_PROPERTY_TYPE_BYTE:
                    rc = props->array[i].value.byte;
                    break;
                case QCM_MQTT_PROPERTY_TYPE_TWO_BYTE_INTEGER:
                    rc = props->array[i].value.integer2;
                    break;
                case QCM_MQTT_PROPERTY_TYPE_FOUR_BYTE_INTEGER:
                case QCM_MQTT_PROPERTY_TYPE_VARIABLE_BYTE_INTEGER:
                    rc = props->array[i].value.integer4;
                    break;
                default:
                    rc = -999999;
                    break;
            }
            break;
        }
    }
    return rc;
}

/**
 *  @brief Get the value of the specified attribute.
 *
 * @param[in] qcm_mqtt_properties_t* props
 *           - Pointer to the qcm_mqtt_properties_t structure, which contains the property list.
 * @param[in] qcm_mqtt_property_code_e propid
 *           - Enumeration of IDs for storing data
 *
 * @return qosa_int64_t
 */
qosa_int64_t qcm_mqtt_properties_get_numeric_value(qcm_mqtt_properties_t* props, qcm_mqtt_property_code_e propid)
{
    return qcm_mqtt_properties_get_numeric_value_at(props, propid, 0);
}

/**
 *  @brief Get the structure position of the specified attribute.
 *
 * @param[in] qcm_mqtt_properties_t* props
 *           - Pointer to the qcm_mqtt_properties_t structure, which contains the property list.
 * @param[in] qcm_mqtt_property_code_e propid
 *           - Enumeration of IDs for storing data
 * @param[in] qosa_uint32_t index
 *           - If there are multiple identical attributes, this indicates which occurrence of the identical attribute to select
 *
 * @return qosa_int64_t
 */
qcm_mqtt_property_t* qcm_mqtt_properties_get_property_at(qcm_mqtt_properties_t* props, qcm_mqtt_property_code_e propid, qosa_uint32_t index)
{
    qosa_uint32_t        i = 0;
    qcm_mqtt_property_t* result = QOSA_NULL;
    qosa_uint32_t        cur_index = 0;

    if(QOSA_NULL == props) 
    {
        return QOSA_NULL;
    }
    QLOGI("count:%d", props->count);

    for (i = 0; props != QOSA_NULL && i < props->count; ++i)
    {
        qosa_uint32_t id = props->array[i].identifier;

        if (id == propid)
        {
            if (cur_index == index)
            {
                result = &props->array[i];
                break;
            }
            else
            {
                cur_index++;
            }
        }
    }
    return result;
}

/**
 *  @brief Get the structure position of the specified attribute.
 *
 * @param[in] qcm_mqtt_properties_t* props
 *           - Pointer to the qcm_mqtt_properties_t structure, which contains the property list.
 * @param[in] qcm_mqtt_property_code_e propid
 *           - Enumeration of IDs for storing data
 *
 * @return qosa_int64_t
 */
qcm_mqtt_property_t* qcm_mqtt_properties_get_property(qcm_mqtt_properties_t* props, qcm_mqtt_property_code_e propid)
{
    return qcm_mqtt_properties_get_property_at(props, propid, 0);
}
#endif /* CONFIG_QCM_MQTT5_FUNC */
