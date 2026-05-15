/*****************************************************************/ /**
* @file unirtos_atcmd_mqtt.h
* @brief
* @author lawrence.liu@quectel.com
* @date 2023-12-20
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description
* <tr><td>2023-12-20 <td>1.0 <td>lawrence.liu <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_MQTT_H
#define __UNIRTOS_ATCMD_MQTT_H

#include "qcm_mqtt_config.h"
#include "qosa_at_param.h"
#include "qcm_mqtt.h"

/**
 * @brief: Default input configuration for MQTT AT.
 */
#define UNIR_MQTT_DEFAULT_PDP_CID             1
#define UNIR_MQTT_DEFAULT_CLIENT_KALIVE       120
#define UNIR_MQTT_DEFAULT_PKT_TIMEOUT         5
#define UNIR_MQTT_DEFAULT_RETRY_TIMES         3
#define UNIR_MQTT_DEFAULT_CLOSE_TIME_INTERVAL 5
#define UNIR_MQTT_DEFAULT_PUB_URC_MAX         CONFIG_UNIRTOS_MQTT_DEFAULT_PUB_URC_MAX

/**
 * @brief: Maximum and minimum values for MQTT AT input parameters.
 */
/** Minimum MQTT client ID (typically 0) */
#define UNIR_MQTT_MIN_CLIENT_ID               0
/** Maximum MQTT client ID (limited by the platform's maximum number of clients) */
#define UNIR_MQTT_MAX_CLIENT_ID               QCM_MQTT_MAX_CLIENT_NUMBER_ID

/** Minimum QoS level supported by MQTT (corresponding to QoS 0) */
#define UNIR_MQTT_MIN_QOS_LEVEL               QCM_MQTT_AT_MOST_ONCE_DELIVERY
#ifdef CONFIG_QCM_MQTT5_FUNC
/** Maximum QoS level supported by MQTT 5.0 (0x3f, 6-bit selectable) */
#define UNIR_MQTT_MAX_QOS_LEVEL 0x3f
#else
/** Maximum QoS level in non-MQTT 5.0 mode (corresponds to QoS 2) */
#define UNIR_MQTT_MAX_QOS_LEVEL QCM_MQTT_EXACTLY_ONCE_DELIVERY
#endif /* CONFIG_QCM_MQTT5_FUNC */

/** Minimum keep-alive time for MQTT client (unit: seconds) */
#define UNIR_MQTT_MIN_CLIENT_KALIVE      0
/** MQTT client maximum keep-alive time (unit: seconds) */
#define UNIR_MQTT_MAX_CLIENT_KALIVE      3600

/** MQTT minimum packet timeout (unit: seconds) */
#define UNIR_MQTT_MIN_PKT_TIMEOUT        1
/** MQTT maximum packet timeout (unit: seconds) */
#define UNIR_MQTT_MAX_PKT_TIMEOUT        60

/** MQTT minimum retry count */
#define UNIR_MQTT_MIN_RETRY_TIMES        1
/** MQTT maximum retry count */
#define UNIR_MQTT_MAX_RETRY_TIMES        10

/** MQTT close timeout notification (0 means off) */
#define UNIR_MQTT_CLOSE_TIMEOUT_NOTICE   0
/** MQTT connection timeout notification (1 indicates enabled) */
#define UNIR_MQTT_OPEN_TIMEOUT_NOTICE    1

/** MQTT minimum close wait time (unit: seconds) */
#define UNIR_MQTT_MIN_CLOSE_TIME         5
/** MQTT maximum close wait time (unit: seconds) */
#define UNIR_MQTT_MAX_CLOSE_TIME         60

#define UNIR_MQTT_DEF_EDIT_TIMEOUT       CONFIG_UNIRTOS_EDIT_TIMEOUT_DEFAULT
/** Minimum timeout for exiting edit mode */
#define UNIR_MQTT_MIN_EDIT_TIMEOUT       0
/** edit Maximum timeout for exiting edit mode */
#define UNIR_MQTT_MAX_EDIT_TIMEOUT       180

#define UNIR_MQTT_MIN_TOPIC_LEN          1
#define UNIR_MQTT_MAX_TOPIC_LEN          256

#define UNIR_MQTT_MIN_WILL_MESSAGE_LEN   0
#define UNIR_MQTT_MAX_WILL_MESSAGE_LEN   256

#define UNIR_MQTT_MIN_RECV_URCMAX        5 * 1024
#define UNIR_MQTT_MAX_RECV_URCMAX        CONFIG_UNIRTOS_MQTT_MAX_PUBLISH_MSG_LEN

#define UNIR_MQTT_MAX_USER_NAME_LEN      256
#define UNIR_MQTT_MAX_PASSWORD_LEN       256
#define UNIR_MQTT_MAX_CLIENTID_LEN       256

#define UNIR_MQTT_MIN_SERVER_PORT_NUMBER 1
#define UNIR_MQTT_MAX_SERVER_PORT_NUMBER 65535

#define UNIR_MQTT_MIN_QOS0_MSG_ID        0
#define UNIR_MQTT_MIN_QOS1_MSG_ID        1
#define UNIR_MQTT_MAX_MSG_ID             QCM_MQTT_MAX_PACKET_ID

#define UNIR_MQTT_MIN_SUB_MSG_ID         1
#define UNIR_MQTT_MAX_SUB_MSG_ID         UNIR_MQTT_MAX_MSG_ID

#define UNIR_MQTT_DEF_PUB_MSG_LEN        30 * 1024
#define UNIR_MQTT_MAX_PUB_MSG_LEN        100 * 1024

#define UNIR_MQTT_MIN_STORE_ID           0
#define UNIR_MQTT_MAX_STORE_ID           QCM_MQTT_MAX_STORE_MSG_CNT - 1

#define UNIR_AT_MQTT_FAIL                -1
#define UNIR_AT_MQTT_SUCCESS             0
#define UNIR_AT_MQTT_OUT_OF_MEMORY       6

/**
 * @enum unir_mqtt_err_code_e
 * @brief Error codes generated when executing MQTT AT-side functions.
 */
typedef enum
{
    UNIR_MQTT_OK = 0,
    UNIR_MQTT_ERROR_GENERAL = 1,
    UNIR_MQTT_ERROR_MEMORY = 2,
    UNIR_MQTT_ERROR_AT_FORMAT = 3,
    UNIR_MQTT_ERROR_CLIENT_STATUS = 4,
    UNIR_MQTT_ERROR_PARAM_INVALID = 5,
    UNIR_MQTT_ERROR_PARAM_IS_NULL = 6,
    UNIR_MQTT_ERROR_AT_OUTPUT_FAIL = 7,
    UNIR_MQTT_ERROR_OPEN_FAIL = 8,
    UNIR_MQTT_ERROR_SSL_CONFIG_FAIL = 9,
    UNIR_MQTT_ERROR_SUB_FAIL = 10,
    UNIR_MQTT_ERROR_PUB_FAIL = 11,
    UNIR_MQTT_ERROR_EDIT_DATA_FAIL = 12,
    UNIR_MQTT_ERROR_RECV_DATA_FAIL = 13,
} unir_mqtt_err_code_e;

typedef enum
{
    UNIR_AT_MQTT_OPEN_PARAM_INVALID = 1,  /** Parameter error */
    UNIR_AT_MQTT_OPEN_CLIENT_ID_USED = 2, /** MQTT identifier is occupied */
    UNIR_AT_MQTT_OPEN_PDP_FAIL = 3,       /** Activation of PDP failed */
    UNIR_AT_MQTT_OPEN_DNS_FAIL = 4,       /** Domain name resolution failed */
    UNIR_AT_MQTT_OPEN_CNN_FAIL = 5,       /** Network disconnection caused error */
} unir_mqtt_open_err;

typedef enum
{
    UNIR_AT_MQTT_RESULT_SUCCESS = 0,        /** Packet sent successfully and ACK received from server */
    UNIR_AT_MQTT_RESULT_PKT_RETRANSMIT = 1, /** Packet retransmission */
    UNIR_AT_MQTT_RESULT_SEND_PKT_FAIL = 2,  /** Packet sending failed */
} unir_mqtt_result_err;

/** MQTT protocol error, indicates protocol processing failure */
#define UNIR_MQTT_ERROR_PROTOCOL_FAIL 130

#if 0

/**
 * @enum unir_at_mqtt_code_report_e
 * @brief MQTT AT Side URC Report Error Codes.
 */
typedef enum
{
    AT_MQTT_OK                                    = 0,    /** Success. */
    AT_MQTT_ERROR_SERVER_DISCONNECT               = 1,    /** Connection was disconnected or reset by the server. */
    AT_MQTT_ERROR_PING_TIMEOUT                    = 2,    /** Timeout or failure in sending PINGREQ packet. */
    AT_MQTT_ERROR_CONNECT_TIMEOUT                 = 3,    /** Timeout or failure in sending CONNECT packet. */
    AT_MQTT_ERROR_CONNACK_TIMEOUT                 = 4,    /** Timeout or failure in receiving the CONNACK packet. */
    AT_MQTT_ERROR_CLIENT_DISCONNECT_SERVER_CLOSE  = 5,    /** The client sends a DISCONNECT packet to the server, but the server actively disconnects the MQTT connection. */
    AT_MQTT_ERROR_SEND_FAIL                       = 6,    /**  Because sending data packets always fails, the client actively disconnects the MQTT connection. */
    AT_MQTT_ERROR_NETWORK                         = 7,    /** Link is not working or server is unavailable. */
    AT_MQTT_ERROR_CLIENT_DISCONNECT               = 8,    /** Client actively disconnects the MQTT connection. */
    AT_MQTT_ERROR_MAX,
} unir_at_mqtt_code_report_e;
#endif

/**
 * @enum unir_mqtt_state_code_e
 * @brief Enumeration definition for MQTT client state codes, defining the state codes of the MQTT client at different stages, used to indicate the connection status of the client.
 */
typedef enum
{
    UNIR_MQTT_CLIENT_INIT = 0,                        /** MQTT client initialization status. */
    UNIR_MQTT_CLIENT_TCP_OPENING = 1,                 /** MQTT client TCP connection in progress. */
    UNIR_MQTT_CLIENT_TCP_OPENED = 2,                  /** MQTT client TCP connection established. */
    UNIR_MQTT_CLIENT_CONNECTING = 3,                  /** MQTT client is performing MQTT protocol connection. */
    UNIR_MQTT_CLIENT_CONNECTED = 4,                   /** MQTT client has completed the MTT protocol connection. */
    UNIR_MQTT_CLIENT_CLOSEING = 5,                    /** MQTT client TCP connection is closing. */
    UNIR_MQTT_CLIENT_CLOSEED = UNIR_MQTT_CLIENT_INIT, /** MQTT client TCP connection closed, returned to initialization state. */
} unir_mqtt_state_code_e;

/**
 * @enum unir_mqtt_conn_state_code_e
 * @brief MQTT connection reporting status.
 */
typedef enum
{
    UNIR_MQTT_CONN_INIT = 1,
    UNIR_MQTT_CONN_CONNECTING = 2,
    UNIR_MQTT_CONN_CONNECTED = 3,
    UNIR_MQTT_CONN_CLOSEING = 4,
} unir_mqtt_conn_state_code_e;

/**
 * @enum unir_mqtt_recv_mode_e
 * @brief MQTT data reception reporting mode.
 */
typedef enum
{
    UNIR_MQTT_PUBLISH_DIRECT_OUTPUT = 0,
    UNIR_MQTT_PUBLISH_CACHE = 1,
} unir_mqtt_recv_mode_e;

/**
 * @enum unir_mqtt_at_event_e
 * @brief Events that MQTT AT needs to process in the center task.
 */
typedef enum
{
    UNIR_MQTT_CLIENT_EVENT = 1,
    UNIR_MQTT_EDIT_TIMEOUT_EVENT = 2,
    UNIR_MQTT_ESCAPE_EDIT_EVENT = 3, /** Listens for the channel to exit edit mode or passthrough mode. */
} unir_mqtt_at_event_e;

typedef struct
{
    unir_mqtt_at_event_e    event_id;
    qcm_mqtt_client_event_e mqtt_id;
    void                   *mqtt_param;
} unir_mqtt_msg_t;

void qstd_exec_mqtt_qmtcfg_cmd(qosa_at_cmd_t *cmd);

void qstd_exec_mqtt_qmtopen_cmd(qosa_at_cmd_t *cmd);

void qstd_exec_mqtt_qmtconn_cmd(qosa_at_cmd_t *cmd);

void qstd_exec_mqtt_qmtsub_cmd(qosa_at_cmd_t *cmd);

void qstd_exec_mqtt_qmtunsub_cmd(qosa_at_cmd_t *cmd);

void qstd_exec_mqtt_qmtpub_cmd(qosa_at_cmd_t *cmd);

void qstd_exec_mqtt_qmtdisc_cmd(qosa_at_cmd_t *cmd);

void qstd_exec_mqtt_qmtrecv_cmd(qosa_at_cmd_t *cmd);

void qstd_exec_mqtt_qmtclose_cmd(qosa_at_cmd_t *cmd);

#ifdef CONFIG_QCM_MQTT5_FUNC
void qstd_exec_mqtt_qmtsvrinfo_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_mqtt_qmtsubprop_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_mqtt_qmtunsprop_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_mqtt_qmtpubprop_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_mqtt_qmtdiscprop_cmd(qosa_at_cmd_t *cmd);
#endif /* CONFIG_QCM_MQTT5_FUNC */

#endif /* __UNIRTOS_ATCMD_MQTT_H */
