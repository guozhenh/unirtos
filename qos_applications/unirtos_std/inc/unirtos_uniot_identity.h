/*****************************************************************/ /**
* @file unirtos_uniot_identity.h
* @brief
* @author lawrence.liu@quectel.com
* @date 2023-12-11
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description
* <tr><td>2023-12-11 <td>1.0 <td>lawrence.liu <td> Init
* </table>
**********************************************************************/
#ifndef UNIR_CUIOT_IDENTITY_H
#define UNIR_CUIOT_IDENTITY_H

#include "unirtos_unify_common.h"

#define UNIR_UNIOT_IDENTITY_AUTO_REGIST_PERIOD_CNT_ONCE      -2
#define UNIR_UNIOT_IDENTITY_AUTO_REGIST_PERIOD_CNT_ONLY_ONCE -1
#define UNIR_UNIOT_IDENTITY_AUTO_REGIST_PERIOD_CNT_SOPT      0
#define UNIR_UNIOT_IDENTITY_AUTO_REGIST_PERIOD_CNT_MAX       99999999
#define UNIR_UNIOT_IDENTITY_AUTO_REGIST_PERIOD_TIME          1 * 60 * 60 * 1000
#define UNIR_UNIOT_IDENTITY_AUTO_REGIST_HOUR_TIME_MS         1 * 60 * 60 * 1000

#define UNIR_UNIOT_IDENTITY_DEF_HOST                         "dmp-mqtt.cuiot.cn"
#define UNIR_UNIOT_IDENTITY_DEF_PORT                         1883
#define UNIR_UNIOT_IDENTITY_DEF_PDP_ID                       1
#define UNIR_UNIOT_IDENTITY_DEF_SIM_ID                       0
#define UNIR_UNIOT_IDENTITY_DEF_SSL_ID                       0
#define UNIR_UNIOT_IDENTITY_DEF_DATAMODE                     0
#define UNIR_UNIOT_IDENTITY_DEF_RECON_INTR                   0
#define UNIR_UNIOT_IDENTITY_DEF_RECON_TIME                   0
#define UNIR_UNIOT_IDENTITY_DEF_KEEP_ALIVE                   300
#define UNIR_UNIOT_IDENTITY_DEF_TIMEOUT                      2000
#define UNIR_UNIOT_IDENTITY_DEF_PKT_TIMEOUT                  5  //Retry interval time Unit: second s
#define UNIR_UNIOT_IDENTITY_DEF_RETRY_TIMES                  3  //Number of retry attempts

#define UNIR_UNIOT_IDENTITY_DEF_OPERATOR                     UNIR_UNIOT_OPERATOR_MODE_TEST
#define UNIR_UNIOT_IDENTITY_DEF_SIGNMETHOD                   0
#define UNIR_UNIOT_IDENTITY_DEF_AUTHTYPE                     UNIR_UNIOT_AUTH_MODE_GW_ACTIVE

#define UNIR_UNIOT_IDENTITY_DEF_NTP_HOSTNAME                 "ntp.aliyun.com"
#define UNIR_UNIOT_IDENTITY_DEF_NTP_RETRY_CNT                3
#define UNIR_UNIOT_IDENTITY_DEF_NTP_RETRY_TIME               60

#define UNIR_UNIOT_TEXT_LENGTH_DEFAULT                       256

#define UNIR_UNIOT_IDENTITY_INPUB_SEM_TIMEOUT_OFFSET         500

#define UNIR_UNIOT_CONNECTION_MODE_2G                        "2G"
#define UNIR_UNIOT_CONNECTION_MODE_4G                        "4G"
#define UNIR_UNIOT_CONNECTION_MODE_OTHER                     "other"

typedef enum
{
    UNIR_UNIOT_IDENTITY_INIT_OPT_EVENT_ID = 1,
    UNIR_UNIOT_IDENTITY_MQTT_RECON_OPT_EVENT_ID = 2,
    UNIR_UNIOT_IDENTITY_MQTT_OPEN_OPT_EVENT_ID = 3,
    UNIR_UNIOT_IDENTITY_MQTT_CONN_OPT_EVENT_ID = 4,
    UNIR_UNIOT_IDENTITY_MQTT_SET_INPUT_OPT_EVENT_ID = 5,
    UNIR_UNIOT_IDENTITY_MQTT_UN_SUB_OPT_EVENT_ID = 6,
    UNIR_UNIOT_IDENTITY_MQTT_PUBLIST_OPT_EVENT_ID = 7,
    UNIR_UNIOT_IDENTITY_MQTT_NEW_MESSAGE_OPT_EVENT_ID = 8,
    UNIR_UNIOT_IDENTITY_MQTT_DISCONN_OPT_EVENT_ID = 9,
    UNIR_UNIOT_IDENTITY_MQTT_CLOSE_OPT_EVENT_ID = 10,
    UNIR_UNIOT_IDENTITY_MQTT_MSG_TIMER_OPT_EVENT_ID = 11,
    UNIR_UNIOT_IDENTITY_RECONN_START_OPT_EVENT_ID = 12,
    UNIR_UNIOT_IDENTITY_END_OPT_EVENT_ID = 13,
} unir_uniot_identity_opt_event_id_e;

typedef enum
{
    UNIR_UNIOT_IDENTITY_INIT_STAT = 0,
    UNIR_UNIOT_IDENTITY_STARTUP_NW_STAT = 1,
    UNIR_UNIOT_IDENTITY_MQTT_OPEN_STAT = 2,

    UNIR_UNIOT_IDENTITY_MQTT_REGIST_CONNING_STAT = 3,
    UNIR_UNIOT_IDENTITY_MQTT_REGIST_CONNED_STAT = 4,

    UNIR_UNIOT_IDENTITY_MQTT_REGIST_DISCONNING_STAT = 5,
    UNIR_UNIOT_IDENTITY_MQTT_REGIST_DISCONNED_STAT = 6,

    UNIR_UNIOT_IDENTITY_MQTT_CONNING_STAT = 7,
    UNIR_UNIOT_IDENTITY_MQTT_CONNED_STAT = 8,

    UNIR_UNIOT_IDENTITY_MQTT_SUB_EXT_AUTOREGISTING_STAT = 9,
    UNIR_UNIOT_IDENTITY_MQTT_SUB_EXT_AUTOREGIST_WAIT_MSG_STAT = 10,
    UNIR_UNIOT_IDENTITY_MQTT_SUB_EXT_AUTOREGISTED_STAT = 11,

    UNIR_UNIOT_IDENTITY_MQTT_SUB_DEVICESHADOW_COMMANDING_STAT = 12,
    UNIR_UNIOT_IDENTITY_MQTT_SUB_DEVICESHADOW_COMMAND_WAIT_MSG_STAT = 13,
    UNIR_UNIOT_IDENTITY_MQTT_SUB_DEVICESHADOW_COMMANDED_STAT = 14,

    UNIR_UNIOT_IDENTITY_MQTT_UNSUB_DEVICESHADOW_COMMANDING_STAT = 15,
    UNIR_UNIOT_IDENTITY_MQTT_UNSUB_DEVICESHADOW_COMMANDED_STAT = 16,

    UNIR_UNIOT_IDENTITY_MQTT_SUB_DEVICESHADOW_UPDATE_REPLYING_STAT = 17,
    UNIR_UNIOT_IDENTITY_MQTT_SUB_DEVICESHADOW_UPDATE_REPLY_WAIT_MSG_STAT = 18,
    UNIR_UNIOT_IDENTITY_MQTT_SUB_DEVICESHADOW_UPDATE_REPLYED_STAT = 19,

    UNIR_UNIOT_IDENTITY_MQTT_PUB_DEVICESHADOW_UPDATEING_STAT = 20,
    UNIR_UNIOT_IDENTITY_MQTT_PUB_DEVICESHADOW_UPDATEED_STAT = 21,

    UNIR_UNIOT_IDENTITY_MQTT_SUB_PROPERTY_BATCH_REPLYING_STAT = 22,
    UNIR_UNIOT_IDENTITY_MQTT_SUB_PROPERTY_BATCH_REPLY_WIAIT_MSG_STAT = 23,
    UNIR_UNIOT_IDENTITY_MQTT_SUB_PROPERTY_BATCH_REPLYED_STAT = 24,

    UNIR_UNIOT_IDENTITY_MQTT_PUB_PROPERTY_BATCHING_STAT = 25,
    UNIR_UNIOT_IDENTITY_MQTT_PUB_PROPERTY_BATCHED_STAT = 26,

    UNIR_UNIOT_IDENTITY_MQTT_DISCONNING_STAT = 27,
    UNIR_UNIOT_IDENTITY_MQTT_DISCONNED_STAT = 28,

    UNIR_UNIOT_IDENTITY_MQTT_CLOSE_STAT = 29,

    UNIR_UNIOT_IDENTITY_RECONN_START_STAT = 30,

    UNIR_UNIOT_IDENTITY_WAIT_MSG_STAT = 31,

    UNIR_UNIOT_IDENTITY_END_STAT = 32,
} unir_uniot_identity_conn_stat_e;

typedef enum
{
    UNIR_UNIOT_FIX_PUB_PROPERTY_BATCH_OPT = 1,
    UNIR_UNIOT_FIX_PUB_DEVICESHADOW_UPDATE_OPT,
} unir_uniot_fix_publist_opt_e;

typedef enum
{
    UNIR_UNIOT_IDENTITY_OK = 0,
    UNIR_UNIOT_IDENTITY_STOP,
    UNIR_UNIOT_IDENTITY_PDP_ACTIVE_ERR,
    UNIR_UNIOT_IDENTITY_MQTT_OPEN_ERR,
    UNIR_UNIOT_IDENTITY_MQTT_CONN_ERR,
    UNIR_UNIOT_IDENTITY_MQTT_ERR,
    UNIR_UNIOT_IDENTITY_GET_PLMN_ERR,
    UNIR_UNIOT_IDENTITY_GET_ICCID_ERR,
    UNIR_UNIOT_IDENTITY_TOKEN_OPERATOR_MODE_ERR,
    UNIR_UNIOT_IDENTITY_TOKEN_AUTH_MODE_ERR,
    UNIR_UNIOT_IDENTITY_MEMORY_ERR,
    UNIR_UNIOT_IDENTITY_PARA_NULL_ERR,
    UNIR_UNIOT_IDENTITY_READ_NVM_ERR,
    UNIR_UNIOT_IDENTITY_SAVE_NVM_ERR,
    UNIR_UNIOT_IDENTITY_CJSON_PARSE_ERR,
    UNIR_UNIOT_IDENTITY_DMP_REPLYCODE_ERR,
    UNIR_UNIOT_IDENTITY_TOPIC_NOT_SUPPROT_ERR,
    UNIR_UNIOT_IDENTITY_AUTH_PARAM_NOT_CFG_ERR,
    UNIR_UNIOT_IDENTITY_ALREADY_RUN_ERR,
    UNIR_UNIOT_IDENTITY_TIMER_START_FAILED,
    UNIR_UNIOT_IDENTITY_FIX_PUBLIST_OPT_ERR,
    UNIR_UNIOT_IDENTITY_PSM_ENABLE_FAILED,
    UNIR_UNIOT_IDENTITY_PSM_NOTSUPPOT_ERR,
} unir_uniot_identity_error_e;

typedef struct
{
    char manufacturer[UNIR_UNIOT_TEXT_LENGTH_DEFAULT];  //"manufacturer","Module Manufacturer","text","length": "2048"
    char moduleType[UNIR_UNIOT_TEXT_LENGTH_DEFAULT];    //"moduleType","Module Model","text","length": "2048"
    char moduleIMEI[UNIR_UNIOT_TEXT_LENGTH_DEFAULT];    //"moduleIMEI","Module IMEI","text","length": "2048"
    char chipManufacturer[UNIR_UNIOT_TEXT_LENGTH_DEFAULT];
    char chipType[UNIR_UNIOT_TEXT_LENGTH_DEFAULT];
    char moduleSoftwareVersion[UNIR_UNIOT_TEXT_LENGTH_DEFAULT];
    char moduleHardwareVersion[UNIR_UNIOT_TEXT_LENGTH_DEFAULT];

    char         connectionMode[UNIR_UNIOT_TEXT_LENGTH_DEFAULT];
    char         moduleIMSI[UNIR_UNIOT_TEXT_LENGTH_DEFAULT];   //"moduleIMSI","Module IMSI","text","length": "2048"
    char         moduleICCID[UNIR_UNIOT_TEXT_LENGTH_DEFAULT];  //"moduleICCID","Module ICCID","text","length": "2048"
    qosa_int32_t csq;
    qosa_int32_t rsrp;
    float        rsrq;
    qosa_int32_t signalECL;
    qosa_int32_t sinr;
    char         cellID[UNIR_UNIOT_TEXT_LENGTH_DEFAULT];  //"cellID","Cell ID","text","length": "2048"
    char         APN[UNIR_UNIOT_TEXT_LENGTH_DEFAULT];

    double altitude;                                      //Altitude
    double latitude;                                      //Latitude positioning
    double longitude;                                     //Longitude positioning
    char   moduleTime[UNIR_UNIOT_TEXT_LENGTH_DEFAULT];    //"moduleTime","Module Time","text","length": "2048"
    char   internetTime[UNIR_UNIOT_TEXT_LENGTH_DEFAULT];  //"moduleTime","Network Time","text","length": "2048"
    int    autoRegperiod;  //"autoRegperiod", "Auto-registration upload period", "int32","minimum": "0","maximum": "99999999","unit": "h","unitName": "hour","step": "1"
    int    powerSavingMode;  //"powerSavingMode","Power Saving Mode",enum,"0": "PSM","1": "DRX","2": "eDRX","20": "Not Activated"
} unir_uniot_property_info_t;

typedef struct
{
    int autoRegperiod;  //"autoRegperiod", "Auto-registration upload period", "int32","minimum": "0","maximum": "99999999","unit": "h","unitName": "hour","step": "1"
    int version;
} unir_uniot_deviceShadow_info_t;

typedef struct
{
    qosa_uint8_t                    client_id; /*!< Handle of the MQTT client. */
    unir_uniot_identity_conn_stat_e stat;
    qosa_timer_t                    msg_timer;
    qosa_uint16_t                   packet_id_num;
    int                             msg_timeout;
    qosa_timer_t                    reconn_timer;
    int                             reconn_timeout;
} unir_uniot_identity_conn_t;

typedef struct
{
    unir_uniot_identity_opt_event_id_e msg_id;
    int                                err_code;
    void                              *argv;
} unir_uniot_identity_event_t;

typedef struct
{
    qosa_uint8_t nSim_id;
    int operator;
    int signMethod;
    int authType;

    char PK[OSA_UNIFY_PK_STR_LEN_MAX];
    char DK[OSA_UNIFY_DK_STR_LEN_MAX];
    char Secret[OSA_UNIFY_TOKEN_STR_LEN_MAX];

    char clientId[OSA_UNIFY_CLIENTID_LEN_MAX];
    char username[OSA_UNIFY_USERNAME_LEN_MAX];
    char password[OSA_UNIFY_PASSWORD_LEN_MAX];
} unir_uniot_token_info_t;

#endif /*UNIR_CUIOT_IDENTITY_H*/
