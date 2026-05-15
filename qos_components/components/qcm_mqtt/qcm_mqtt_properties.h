/*****************************************************************/ /**
* @file qcm_mqtt_properties.h
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

#ifndef __QCM_MQTT_PROPERTIES_H__
#define __QCM_MQTT_PROPERTIES_H__

#include "qcm_mqtt.h"
#include "qcm_mqtt_private.h"
#ifdef CONFIG_QCM_MQTT5_FUNC

/**
 * @enum qcm_mqtt_property_types_e.
 * @brief Enumeration of attribute fields in MQTT 5.0, indicating the corresponding attribute configuration in the attribute structure array.
 */
typedef enum
{
    QCM_MQTT_PROPERTY_TYPE_INVALID = -1,
    QCM_MQTT_PROPERTY_TYPE_BYTE,
    QCM_MQTT_PROPERTY_TYPE_TWO_BYTE_INTEGER,
    QCM_MQTT_PROPERTY_TYPE_FOUR_BYTE_INTEGER,
    QCM_MQTT_PROPERTY_TYPE_VARIABLE_BYTE_INTEGER,
    QCM_MQTT_PROPERTY_TYPE_BINARY_DATA,
    QCM_MQTT_PROPERTY_TYPE_UTF_8_ENCODED_STRING,
    QCM_MQTT_PROPERTY_TYPE_UTF_8_STRING_PAIR
} qcm_mqtt_property_types_e;

/**
 *  @brief Get the type of MQTT 5.0 properties.
 *
 * @param[in] qcm_mqtt_property_code_e value
 *           - The input MQTT 5.0 property ID value.
 * @return qcm_mqtt_property_types_e
 *        - Returns the corresponding MQTT 5.0 property type.
 */
qcm_mqtt_property_types_e qcm_mqtt_property_get_type(qcm_mqtt_property_code_e value);

/** Initialization of the MQTT 5.0 attribute structure.*/
#define QCM_MQTT_PROPERTY_INIT                                                                                                                                 \
    {                                                                                                                                                          \
        0, 0, 0, NULL                                                                                                                                          \
    }

/**
 *  @brief Calculate the length of the MQTT property field.
 *
 * @param[in] qcm_mqtt_properties_t* props
 *           - Pointer to the qcm_mqtt_properties_t structure, which contains the property list.
 * @return qosa_uint32_t
 *        - Returns the length of the attribute list and group header in bytes.
 */
qosa_uint32_t qcm_mqtt_properties_len(qcm_mqtt_properties_t* props);

qcm_mqtt_eercode_e qcm_mqtt_data_to_property(qcm_mqtt_properties_t* props, qcm_mqtt_property_code_e identifier, void* data);

/**
 * Add a property pointer to the property array. Memory is allocated in this function,
 * so MQTTClient_create or MQTTAsync_create must be called first to initialize the
 * internal heap tracking. Alternatively MQTTAsync_global_init() can be called first
 * or build with the HIGH_PERFORMANCE option which disables the heap tracking.
 * @param props The property list to add the property to.
 * @param prop The property to add to the list.
 * @return 0 on success, -1 on failure.
 */
qcm_mqtt_eercode_e qcm_mqtt_properties_add(qcm_mqtt_properties_t* props, qcm_mqtt_property_t* prop);

/**
 *  @brief Write the MQTT attribute list to the character buffer.
 *
 * @param[in] qcm_mqtt_packet_buffer * rb
 *           - Pointer to the character buffer for writing attribute data.
 * @param[in] qcm_mqtt_properties_t * properties
 *           - Pointer to the qcm_mqtt_properties_t structure, which contains the property list.
 * @return qosa_int32_t
 *        - Returns the number of bytes written, or a negative value if an error occurs.
 */
qosa_int32_t qcm_mqtt_properties_write(qcm_mqtt_packet_buffer* rb, qcm_mqtt_properties_t* properties);

/**
 *  @brief Read the MQTT property list.
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
qosa_int32_t qcm_mqtt_properties_read(qcm_mqtt_properties_t* properties, qcm_mqtt_input_info_t* rb, qosa_uint32_t remain_len);

/**
 * @brief Free the memory occupied by the MQTT property list.
 *
 * @param[in] qcm_mqtt_properties_t * props
 *           - Pointer to the list of MQTT attributes to be released.
 */
void qcm_mqtt_properties_free(qcm_mqtt_properties_t* props);

/**
 * Checks if property list contains a specific property.
 * @param props pointer to the property list.
 * @param propid the property id to check for.
 * @return 1 if found, 0 if not.
 */
qosa_bool_t qcm_mqtt_properties_has_property(qcm_mqtt_properties_t* props, qcm_mqtt_property_code_e propid);

/**
 * Returns the number of instances of a property id. Most properties can exist only once.
 * User properties and subscription ids can exist more than once.
 * @param props pointer to the property list.
 * @param propid the property id to check for.
 * @return the number of times found.  Can be 0.
 */
qosa_uint32_t qcm_mqtt_properties_property_count(qcm_mqtt_properties_t* props, qcm_mqtt_property_code_e propid);

/**
 * Returns the integer value of a specific property.  The property given must be a numeric type.
 * @param props pointer to the property list.
 * @param propid the property id to check for.
 * @return the integer value of the property. -9999999 on failure.
 */
qosa_int64_t qcm_mqtt_properties_get_numeric_value(qcm_mqtt_properties_t* props, qcm_mqtt_property_code_e propid);

/**
 * Returns the integer value of a specific property when it's not the only instance.
 * The property given must be a numeric type.
 * @param props pointer to the property list.
 * @param propid the property id to check for.
 * @param index the instance number, starting at 0.
 * @return the integer value of the property. -9999999 on failure.
 */
qosa_int64_t qcm_mqtt_properties_get_numeric_value_at(qcm_mqtt_properties_t* props, qcm_mqtt_property_code_e propid, qosa_uint32_t index);

/**
 * Returns a pointer to the property structure for a specific property.
 * @param props pointer to the property list.
 * @param propid the property id to check for.
 * @return the pointer to the property structure if found.  NULL if not found.
 */
qcm_mqtt_property_t* qcm_mqtt_properties_get_property(qcm_mqtt_properties_t* props, qcm_mqtt_property_code_e propid);

/**
 * Returns a pointer to the property structure for a specific property when it's not the only instance.
 * @param props pointer to the property list.
 * @param propid the property id to check for.
 * @param index the instance number, starting at 0.
 * @return the pointer to the property structure if found.  NULL if not found.
 */
qcm_mqtt_property_t* qcm_mqtt_properties_get_property_at(qcm_mqtt_properties_t* props, qcm_mqtt_property_code_e propid, qosa_uint32_t index);
#endif /* CONFIG_QCM_MQTT5_FUNC */

#endif /* MQTTPROPERTIES_H */
