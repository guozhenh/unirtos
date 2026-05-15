#ifndef _MQTT_CONFIG_H
#define _MQTT_CONFIG_H

/** Minimum supported MQTT protocol version. */
#define QCM_MQTT_MIN_SUPPORT_VERSION                 3
/** Maximum supported MQTT protocol version. */
#define QCM_MQTT_MAX_SUPPORT_VERSION                 4
/** Maximum number of large data packets (packets with length between [MQTT_BIG_MSG_MIN_PACKET, MQTT_BIG_MSG_MAX_PACKET]) that can be cached by a single MQTT client. */
#define QCM_MQTT_MAX_MSG_CNT                         2
/** Maximum number of topics that can be subscribed in a single MQTT subscription. */
#define QCM_MQTT_MAX_TOPIC_NUM                       5
/** Maximum string length for MQTT will message topic. */
#define QCM_MQTT_MAX_WILL_TOPIC                      257
/** Maximum string length for MQTT will message content. */
#define QCM_MQTT_MAX_WILL_MESSAGE_LEN                257
/** Maximum number of MQTT business layer task message queues. */
#define QCM_MQTT_MSGQ_MAX_CNT                        20
/** MQTT timer precision adjustment. */
#define QCM_MQTT_TIMEOUT_CRITICAL_POINT              0
/** Offset value for the external handle ID of MQTT client. */
#define QCM_MQTT_CLIENT_ID_OFFSET                    1
/** Maximum number of simultaneously supported MQTT clients, also represents MQTT client numbers in the range [0, (QCM_MQTT_MAX_CLIENT_CNT-1)]. */
#define QCM_MQTT_MAX_CLIENT_CNT                      (6)
/** Maximum client ID that can be used by MQTT. */
#define QCM_MQTT_MAX_CLIENT_NUMBER_ID                (QCM_MQTT_MAX_CLIENT_CNT - QCM_MQTT_CLIENT_ID_OFFSET)
/** Maximum of 20 messages per MQTT client, limited by MQTT config store_cnt configuration */
#define QCM_MQTT_MAX_STORE_MSG_CNT                   5
#define QCM_MQTT_MAX_READ_BUFF_LEN                   1500
#define QCM_MQTT_MAX_PDP_ACTIVE_TIMEOUT              90

/** Represents the maximum number of MQTT user attributes supported */
#define QCM_MQTT_MAX_USER_PROPERTY_SUPPORT           5
#define QCM_MQTT_MAX_TOPIC_ALIAS_CNT                 25

/**
 * @brief Default values for MQTT client configuration.
 */
/** MQTT uses SIM card 0 by default. */
#define QCM_MQTT_DEFAULT_SIM_ID                      0
/** MQTT uses PDP CID 1 by default. */
#define QCM_MQTT_DEFAULT_PDP_CID                     QOSA_PDP_CID_MIN
/** MQTT uses protocol 3.1 for connection by default. */
#define QCM_MQTT_DEFAULT_VERSION                     3
/** MQTT uses clean session by default. */
#define QCM_MQTT_DEFAULT_CLEAN_SESSION               1
/** MQTT default keep-alive interval. */
#define QCM_MQTT_DEFAULT_KALIVE_TIME                 120
/** MQTT default timeout timer interval. */
#define QCM_MQTT_DEFAULT_DELIVERY_TIME               5
/** MQTT default timeout timer trigger count. */
#define QCM_MQTT_DEFAULT_DELIVERY_CNT                3
/** MQTT does not report URC notification on every timeout by default. */
#define QCM_MQTT_DEFAULT_DELIVERY_TIMEOUT_INDICATION 0
/** MQTT does not enable will message by default. */
#define QCM_MQTT_DEFAULT_WILL_FLAG                   0
/** MQTT default QoS level for will message. */
#define QCM_MQTT_DEFAULT_WILL_QOS                    0
/** MQTT default will message is not a retained message. */
#define QCM_MQTT_DEFAULT_WILL_RETAIN                 0
/** MQTT default maximum delay close time after ping timeout */
#define QCM_MQTT_DEFAULT_CLOSE_TIME_INTERVAL         5

#endif
