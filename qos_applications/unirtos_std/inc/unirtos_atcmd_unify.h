/*****************************************************************/ /**
* @file unirtos_atcmd_unify.h
* @brief
* @author lawrence.liu@quectel.com
* @date 2023-11-13
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description
* <tr><td>2023-11-13 <td>1.0 <td>lawrence.liu <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_UNIFY_H__
#define __UNIRTOS_ATCMD_UNIFY_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "unirtos_unify_common.h"
#include "qosa_at_param.h"
#include "qosa_at_cmd.h"

/** The platform version number for interfacing with China Unicom Yanfei. */
#define UNIR_UNIFY_VERSION                           "V2.4.1"
/** Maximum number of UNIFY task message queues. */
#define UNIR_UNIFY_MSGQ_MAX_CNT                      20
#define UNIR_UNIFY_TASK_PRIO                         QOSA_PRIORITY_NORMAL

#define UNIR_UNIFY_REP_STR_LEN_MAX                   1024  //Length of urc or at resp buffer

#define UNIR_UNIFY_PK_STR_MAX                        (OSA_UNIFY_PK_LEN_MAX + 4)
#define UNIR_UNIFY_PS_STR_MAX                        (OSA_UNIFY_PS_LEN_MAX + 4)
#define UNIR_UNIFY_DK_STR_MAX                        (OSA_UNIFY_DK_LEN_MAX + 4)
#define UNIR_UNIFY_DS_STR_MAX                        (OSA_UNIFY_DS_LEN_MAX + 4)

/** Default number of input parameters for the China Unicom Yanfei AT+UNIMQTTCONN command. */
#define UNIR_UNIFY_MQTT_CONN_PARAM_NUM               8
/** Default number of input parameters for the China Unicom Yanfei AT+UNIMQTTDISCON command. */
#define UNIR_UNIFY_MQTT_DISCON_PARAM_NUM             0

#define UNIR_UNIFY_AUTH_MODE_DEFAULT                 0
#define UNIR_UNIFY_TLS_FLAG_DEFAULT                  0
#define UNIR_UNIFY_DATA_MODE_DEFAULT                 0
#define UNIR_UNIFY_BIND_FLAG_DEFAULT                 0

/**
 * @brief Input parameter section for China Unicom Yanfei AT+UNIMQTTCONN command.
 */
/** Default server port number for China Unicom Yanfei AT+UNIMQTTCONN command. */
#define UNIR_UNIFY_SERVER_PORT_DEFAULT               500
/** Minimum value for server port number input in China Unicom Yanfei AT+UNIMQTTCONN command. */
#define UNIR_UNIFY_SERVER_PORT_MIN                   500
/** Maximum value for server port number input in China Unicom Yanfei AT+UNIMQTTCONN command. */
#define UNIR_UNIFY_SERVER_PORT_MAX                   10000

/** Default timeout for the Unicom Yanfei AT+UNIMQTTCONN command. */
#define UNIR_UNIFY_TIMEOUT_DEFAULT                   500
/** Minimum value for the timeout input of the China Unicom Yanfei AT+UNIMQTTCONN command. */
#define UNIR_UNIFY_TIMEOUT_MIN                       100
/** Maximum value for the timeout input of the China Unicom Yanfei AT+UNIMQTTCONN command. */
#define UNIR_UNIFY_TIMEOUT_MAX                       5000

/** Default keep-alive time for the China Unicom Yanfei AT+UNIMQTTCONN command. */
#define UNIR_UNIFY_KEEPALIVE_DEFAULT                 300
/** Minimum value for the keep-alive time input of the China Unicom Yanfei AT+UNIMQTTCONN command. */
#define UNIR_UNIFY_KEEPALIVE_MIN                     30
/** Maximum value for the keep-alive time input of the China Unicom Yanfei AT+UNIMQTTCONN command. */
#define UNIR_UNIFY_KEEPALIVE_MAX                     600

/**
 * @brief Input parameter section for China Unicom Yanfei AT+UNIMQTTSUB command.
 */
#define UNIR_UNIFY_SUBCRIBE_QOS_DEFAULT              QCM_MQTT_AT_MOST_ONCE_DELIVERY
#define UNIR_UNIFY_SUBCRIBE_QOS_MIN                  QCM_MQTT_AT_MOST_ONCE_DELIVERY
#define UNIR_UNIFY_SUBCRIBE_QOS_MAX                  QCM_MQTT_AT_LEAST_ONCE_DELIVERY

/** Maximum number of topics supported when subscribing to all topics. */
#define OSA_UNIFY_MQTT_SUB_ALL_TOPIC_MAX             12
#define OSA_UNIFY_MQTT_TOPIC_LENGTH_MAX              256

#define OSA_UNIFY_MQTT_PUBLISH_LENGTH_MAX            1024
#define OSA_UNIFY_MQTT_PUBLISH_EDIT_TIMEOUT_MAX      150 * 1000

#define OSA_UNIFY_MQTT_PSM_START                     1
#define OSA_UNIFY_MQTT_PSM_STOP                      0

#define UNIR_UNIFY_KEY_INFO_STR_MAX                  (UNIR_UNIFY_PK_STR_MAX + UNIR_UNIFY_PS_STR_MAX + UNIR_UNIFY_DK_STR_MAX + UNIR_UNIFY_DS_STR_MAX)
#define UNIR_UNIFY_KEY_INFO_RSP_STR_MAX              (UNIR_UNIFY_KEY_INFO_STR_MAX + 16)

#define UNIR_UNIFY_MQTT_AT_PLATFORM_REPLY_TIMEOUT_MS 5000  // In China Unicom custom commands, AT commands that require waiting for server response need to release the AT engine after a timeout period.

typedef enum
{
    UNIR_UNIFY_MQTT_UNSUBCRIBE = 0,
    UNIR_UNIFY_MQTT_SUBCRIBE = 1
} unir_unify_mqtt_sub_e;

typedef enum
{
    UNIR_UNIFY_MQTT_AT_PLATFORM_REPLY_TIMEOUT = 0x081,  // In China Unicom custom commands, AT commands that need to wait for server response require a timeout event after a certain period.
    UNIR_UNIFY_AT_PROCESS_MSGID_MAX = 0xffffffff
} unir_unifyMqtAt_timer_msgid_enum;

/**
 * @enum unir_unify_mqtt_task_event_e.
 * @brief Events in the UNIFY task.
 */
typedef enum
{
    UNIR_UNIFY_MQTT_OPEN_EVENT = 0x1,         /* UNIFY task MQTT server TCP connection event. */
    UNIR_UNIFY_MQTT_CONNECT_EVENT = 0x2,      /* UNIFY task MQTT CONNECT event. */
    UNIR_UNIFY_MQTT_CLOSE_EVENT = 0x4,        /* UNIFY task MQTT CLOSE event. */
    UNIR_UNIFY_MQTT_READ_SUB_EVENT = 0x8,     /* UNIFY task MQTT subscription event. */
    UNIR_UNIFY_MQTT_SEND_SUB_EVENT = 0x10,    /* UNIFY task MQTT subscription event. */
    UNIR_UNIFY_MQTT_UNSUB_EVENT = 0x11,       /* UNIFY task MQTT unsubscribe event. */
    UNIR_UNIFY_MQTT_PSM_EVENT = 0x12,         /* UNIFY task PSM event. */
    UNIR_UNIFY_MQTT_PUB_EVENT = 0x14,         /* UNIFY task publish message event. */
    UNIR_UNIFY_MQTT_PUB_ACK_EVENT = 0x18,     /* UNIFY task When QoS equals 1, publish the ack event. */
    UNIR_UNIFY_MQTT_PUB_REC_EVENT = 0x20,     /* UNIFY task When QoS equals 2, publish the rec event. */
    UNIR_UNIFY_MQTT_PUB_REL_EVENT = 0x21,     /* UNIFY task When QoS equals 2, publish ack event. */
    UNIR_UNIFY_MQTT_PUB_COM_EVENT = 0x22,     /* UNIFY task When Qos equals 2, publish a comp event. */
    UNIR_UNIFY_MQTT_PDP_DEACTED_EVENT = 0x24, /* UNIFY task Event triggered when PDP is deactivated. */
    UNIR_UNIFY_MQTT_PDP_RECON_EVENT = 0x28,   /* UNIFY task reconnection event. */
    UNIR_UNIFY_MQTT_RELEASE_EVENT = 0x40,
} unir_unify_mqtt_task_event_e;

typedef enum
{
    UNIR_UNIFY_MQTT_AT_CON_STAT_INIT = 0,   // Initialization and de-initialization status
    UNIR_UNIFY_MQTT_AT_CON_STAT_RECON,      //Waiting for reconnection status
    UNIR_UNIFY_MQTT_AT_CON_STAT_RESUME,     //Restore connection status
    UNIR_UNIFY_MQTT_AT_CON_STAT_CONING,     //Connecting, the state from sending the CONN command to receiving and processing the server information.
    UNIR_UNIFY_MQTT_AT_CON_STAT_CONED,      //State after processing the server's connect ack
    UNIR_UNIFY_MQTT_AT_CON_STAT_DISCONING,  //Disconnecting, the state from sending the DISCONN command to receiving and processing the server response.
    UNIR_UNIFY_MQTT_AT_CON_STAT_DISCONED = UNIR_UNIFY_MQTT_AT_CON_STAT_INIT,  // Disconnect completed, release memory, deinitialize operation.
} unir_unify_mqtt_at_con_stat_e;                                              // China Unicom customized AT command connection operation status

void qstd_exec_mqtt_unikeyinfo_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_mqtt_unikeyinfom_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_mqtt_unidelkeyinfo_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_mqtt_unidelfkeyinfom_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_mqtt_unimqttconn_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_mqtt_unimqttdiscon_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_mqtt_unimqttstate_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_mqtt_unimqttsub_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_mqtt_unimqttpub_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_mqtt_unipsmset_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_mqtt_universion_cmd_func(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_UNIFY_H__ */
