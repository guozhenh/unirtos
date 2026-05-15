/*****************************************************************/ /**
* @file unirtos_unify_common.h
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
#ifndef _UNIRTOS_UNIFY_COMMON_H_
#define _UNIRTOS_UNIFY_COMMON_H_

#include "qcm_mqtt.h"
#include "qosa_dev.h"

#define OSA_UNIFY_URL_LEN_MAX                                      512

#define OSA_UNIFY_PK_STR_LEN_MAX                                   (OSA_UNIFY_PK_LEN_MAX + 1)
#define OSA_UNIFY_PS_STR_LEN_MAX                                   (OSA_UNIFY_PS_LEN_MAX + 1)
#define OSA_UNIFY_DK_STR_LEN_MAX                                   (OSA_UNIFY_DK_LEN_MAX + 1)
#define OSA_UNIFY_DS_STR_LEN_MAX                                   (OSA_UNIFY_DS_LEN_MAX + 1)
#define OSA_UNIFY_TOKEN_STR_LEN_MAX                                (OSA_UNIFY_TOKEN_LEN_MAX + 1)

#define OSA_UNIFY_PK_LEN_MAX                                       16
#define OSA_UNIFY_PS_LEN_MAX                                       32
#define OSA_UNIFY_DK_LEN_MIN                                       4   //Minimum length of DK
#define OSA_UNIFY_DK_LEN_MAX                                       32  //Maximum length of DK
#define OSA_UNIFY_DS_LEN_MAX                                       32
#define OSA_UNIFY_TOKEN_LEN_MAX                                    64

#define OSA_UNIFY_CLIENTID_LEN_MAX                                 (257)
#define OSA_UNIFY_USERNAME_LEN_MAX                                 (257)
#define OSA_UNIFY_PASSWORD_LEN_MAX                                 (257)

#define OSA_UNIFY_MQTT_SHADOW_UPDATA_PROPERTY_LENGTH_MAX           (16)
#define OSA_UNIFY_MQTT_SHADOW_UPDATA_STRING_VALUE_LENGTH_MAX       (32)
#define OSA_UNIFY_MQTT_SHADOW_PROPERTY_LENGTH_MAX                  (32)
#define OSA_UNIFY_MQTT_SHADOW_PROPERTY_LENGTH_MIN                  (1)
#define OSA_UNIFY_MQTT_SHADOW_PUB_PROPERTY_STRING_LENGTH_MAX       (16)
#define OSA_UNIFY_MQTT_SHADOW_VALUE_STRING_LENGTH_MAX              (32)

#define OSA_UNIFY_MQTT_AT_CXT_ID_DEFAULT                           (6)  //Default MQTT context ID used for China Unicom custom commands
#define OSA_UNIFY_SIM_ID_DEFAULT                                   (0)  //Default SIM card ID for China Unicom custom commands
#define OSA_UNIFY_MQTT_AT_PDP_ID_DEFAULT                           (1)  //Default PDP ID for China Unicom custom commands

#define OSA_UNIFY_SIGNMETHOD_TYPE_HMAC_SHA256                      (0)
#define OSA_UNIFY_SIGNMETHOD_TYPE_SM3                              (1)
#define OSA_UNIFY_SIGNMETHOD_TYPE_DEFAULT                          OSA_UNIFY_SIGNMETHOD_TYPE_HMAC_SHA256  //Encryption type for China Unicom custom commands

#define UNIR_UNIOT_OPERATOR_MODE_SN                                0
#define UNIR_UNIOT_OPERATOR_MODE_ICCID                             1
#define UNIR_UNIOT_OPERATOR_MODE_TEST                              2

#define UNIR_UNIOT_OPERATOR_ICCID_UNICOM                           1
#define UNIR_UNIOT_OPERATOR_ICCID_CMCC                             2
#define UNIR_UNIOT_OPERATOR_ICCID_CT                               3
#define UNIR_UNIOT_OPERATOR_ICCID_NRTA                             4

#define UNIR_UNIOT_AUTH_MODE_ONLINE                                0
#define UNIR_UNIOT_AUTH_MODE_ACTIVE                                1
#define UNIR_UNIOT_AUTH_MODE_GW_ACTIVE                             2
#define UNIR_UNIOT_AUTH_ONE_MODE_ACTIVE                            3
#define UNIR_UNIOT_AUTH_MODE_GW_ONLINE                             -2

#define UNIR_UNIOT_CJSON_SERIALIZE_TYPE_INIT32                     0
#define UNIR_UNIOT_CJSON_SERIALIZE_TYPE_STRING                     1
#define UNIR_UNIOT_CJSON_SERIALIZE_TYPE_DOUBLE                     2
#define UNIR_UNIOT_CJSON_SERIALIZE_TYPE_FLOAT                      3
#define UNIR_UNIOT_CJSON_SERIALIZE_TYPE_DEF                        4

#define UNIR_UNIOT_DMP_RESPONSE_ERROR_CODE_OK                      "000000"

// Clear the type of key info information
#define OSA_UNIFY_UNIKEYINFO_CLEAR_ALL_TEPY                        0         // Clear all.
#define OSA_UNIFY_UNIKEYINFO_CLEAR_DS_TEPY                         1         //Only clear the Device Secret.
#define OSA_UNIFY_UNIKEYINFO_CLEAR_TOKEN_TEPY                      2         //Only clear the token.

#define OSA_UNIFY_OPERATOR_SHORT_NAME_CHINAMOBILE                  "CMCC"    //Move
#define OSA_UNIFY_OPERATOR_SHORT_NAME_CHINAUNICOM                  "UNICOM"  //Connected
#define OSA_UNIFY_OPERATOR_SHORT_NAME_CHINATELECOM                 "CT"      //Telecommunications
#define OSA_UNIFY_OPERATOR_SHORT_NAME_CHINANRTA                    "NRTA"    //NRTA

#define OSA_UNIFY_DEFAULT_SN                                       "E1818IA100000340P"

#define OSA_UNIFY_MQTT_TOPIC_DEVICECARD_PUB                        "$sys/%s/%s/deviceCard/pub"           //Device requests binding to IoT relationship
#define OSA_UNIFY_MQTT_TOPIC_DEVICECARD_PUB_REPLY                  "$sys/%s/%s/deviceCard/pub_reply"     //Platform response to IoT relationship binding
#define OSA_UNIFY_MQTT_TOPIC_DEVICECARD_UPDATE                     "$sys/%s/%s/deviceCard/update"        //Device requests to update the physical network relationship
#define OSA_UNIFY_MQTT_TOPIC_DEVICECARD_UPDATE_REPLY               "$sys/%s/%s/deviceCard/update_reply"  //Platform responds to IoT relationship update
//Device Shadow related Topic
#define OSA_UNIFY_MQTT_TOPIC_DEVICESHADOW_GET                      "$sys/%s/%s/deviceShadow/get"            //Device retrieves device shadow data
#define OSA_UNIFY_MQTT_TOPIC_DEVICESHADOW_GET_REPLY                "$sys/%s/%s/deviceShadow/get_reply"      //Cloud response device to get device shadow
#define OSA_UNIFY_MQTT_TOPIC_DEVICESHADOW_COMMAND                  "$sys/%s/%s/deviceShadow/command"        //Remote device shadow command issued by the cloud
#define OSA_UNIFY_MQTT_TOPIC_DEVICESHADOW_COMMAND_REPLY            "$sys/%s/%s/deviceShadow/command_reply"  //Device responds to device shadow commands
#define OSA_UNIFY_MQTT_TOPIC_DEVICESHADOW_UPDATE                   "$sys/%s/%s/deviceShadow/update"         //Device reports attribute values to device shadow
#define OSA_UNIFY_MQTT_TOPIC_DEVICESHADOW_UPDATE_REPLY             "$sys/%s/%s/deviceShadow/update_reply"   //Cloud response to device reporting
//PSM
#define OSA_UNIFY_MQTT_TOPIC_PSMENABLE_GET                         "$sys/%s/%s/psmEnabled/get"        //Get the platform's PSM enable status
#define OSA_UNIFY_MQTT_TOPIC_PSMENABLE_GET_REPLY                   "$sys/%s/%s/psmEnabled/get_reply"  //Get the platform's PSM enable status

#define OSA_UNIFY_MQTT_TOPIC_DEVICESHADOW_COMMAND_STRING           "/deviceShadow/command"            //Remote device shadow command issued by the cloud
#define OSA_UNIFY_MQTT_TOPIC_DEVICESHADOW_GET_REPLY_STRING         "/deviceShadow/get_reply"          //Cloud response device to get device shadow
#define OSA_UNIFY_MQTT_TOPIC_DEVICESHADOW_COMMAND_REPLY_STRING     "/deviceShadow/command_reply"      //Device responds to device shadow commands
#define OSA_UNIFY_MQTT_TOPIC_DEVICESHADOW_UPDATE_REPLY_STRING      "/deviceShadow/update_reply"       //Cloud response to device reporting
#define OSA_UNIFY_MQTT_TOPIC_DEVICECARD_PUB_REPLY_STRING           "/deviceCard/pub_reply"            //Platform response to IoT relationship binding
#define OSA_UNIFY_MQTT_TOPIC_DEVICECARD_UPDATE_REPLY_STRING        "/deviceCard/update_reply"         //Platform responds to IoT relationship update
#define OSA_UNIFY_MQTT_TOPIC_PSMENABLE_GET_STRING                  "/psmEnabled/get"                  //Get the platform's PSM enable status
#define OSA_UNIFY_MQTT_TOPIC_PSMENABLE_GET_REPLY_STRING            "/psmEnabled/get_reply"

//Device Thing Model Related Topic
#define UNIR_UNIOT_MQTT_TOPIC_PROPERTY_PUB                         "$sys/%s/%s/property/pub"             //Device attribute reporting
#define UNIR_UNIOT_MQTT_TOPIC_PROPERTY_PUB_REPLY                   "$sys/%s/%s/property/pub_reply"       //Cloud response attribute reporting
#define UNIR_UNIOT_MQTT_TOPIC_PROPERTY_BATCH                       "$sys/%s/%s/property/batch"           //Device attribute batch reporting
#define UNIR_UNIOT_MQTT_TOPIC_PROPERTY_BATCH_REPLY                 "$sys/%s/%s/property/batch_reply"     //Batch reporting of cloud response attributes
#define UNIR_UNIOT_MQTT_TOPIC_PROPERTY_SET                         "$sys/%s/%s/property/set"             //Cloud-side setting property delivery
#define UNIR_UNIOT_MQTT_TOPIC_PROPERTY_SET_REPLY                   "$sys/%s/%s/property/set_reply"       //Device responds to cloud-initiated property settings
#define UNIR_UNIOT_MQTT_TOPIC_SERVICE_PUB                          "$sys/%s/%s/service/pub"              //Cloud service push (asynchronous)
#define UNIR_UNIOT_MQTT_TOPIC_SERVICE_PUB_REPLY                    "$sys/%s/%s/service/pub_reply"        //Device response service delivery (asynchronous)
#define UNIR_UNIOT_MQTT_TOPIC_SYNC_PUB                             "$sys/%s/%s/sync/pub"                 //Cloud service push (synchronization)
#define UNIR_UNIOT_MQTT_TOPIC_SYNC_PUB_REPLY                       "$sys/%s/%s/sync/pub_reply"           // Device response service delivery (synchronous)
#define UNIR_UNIOT_MQTT_TOPIC_EVENT_PUB                            "$sys/%s/%s/event/pub"                //Device event reporting
#define UNIR_UNIOT_MQTT_TOPIC_EVENT_PUB_REPLY                      "$sys/%s/%s/event/pub_reply"          //Cloud response event reporting
#define UNIR_UNIOT_MQTT_TOPIC_EVENT_BATCH                          "$sys/%s/%s/event/batch"              //Device event batch reporting
#define UNIR_UNIOT_MQTT_TOPIC_EVENT_BATCH_REPLY                    "$sys/%s/%s/event/batch_reply"        //Batch reporting of cloud response events
#define UNIR_UNIOT_MQTT_TOPIC_DEVICECARD_PUB                       "$sys/%s/%s/deviceCard/pub"           //Device requests binding to IoT relationship
#define UNIR_UNIOT_MQTT_TOPIC_DEVICECARD_PUB_REPLY                 "$sys/%s/%s/deviceCard/pub_reply"     //Platform response to IoT relationship binding
#define UNIR_UNIOT_MQTT_TOPIC_DEVICECARD_UPDATE                    "$sys/%s/%s/deviceCard/update"        //Device requests to update the physical network relationship
#define UNIR_UNIOT_MQTT_TOPIC_DEVICECARD_UPDATE_REPLY              "$sys/%s/%s/deviceCard/update_reply"  //Platform responds to IoT relationship update

//Gateway Sub-device Related Topics
#define UNIR_UNIOT_MQTT_TOPIC_SUBDEVICE_LOGIN                      "$sys/%s/%s/subdevice/login"         //Gateway sub-device online
#define UNIR_UNIOT_MQTT_TOPIC_SUBDEVICE_LOGIN_REPLY                "$sys/%s/%s/subdevice/login_reply"   // Cloud response gateway sub-device online
#define UNIR_UNIOT_MQTT_TOPIC_SUBDEVICE_REGIST                     "$sys/%s/%s/subdevice/regist"        //	Gateway requests sub-device dynamic registration
#define UNIR_UNIOT_MQTT_TOPIC_SUBDEVICE_REGIST_REPLY               "$sys/%s/%s/subdevice/regist_reply"  //Cloud Response Gateway Sub-device Dynamic Registration
#define UNIR_UNIOT_MQTT_TOPIC_SUBDEVICE_LOGOUT                     "$sys/%s/%s/subdevice/logout"        // Gateway sub-device goes offline
#define UNIR_UNIOT_MQTT_TOPIC_SUBDEVICE_LOGOUT_REPLY               "$sys/%s/%s/subdevice/logout_reply"  //Cloud response gateway sub-device offline
#define UNIR_UNIOT_MQTT_TOPIC_TOPO_NOTIFY                          "$sys/%s/%s/topo/notify"             //Cloud notification gateway sub-device topology status has changed
#define UNIR_UNIOT_MQTT_TOPIC_TOPO_NOTIFY_REPLY                    "$sys/%s/%s/topo/notify_reply"       //Device response to cloud notification
#define UNIR_UNIOT_MQTT_TOPIC_TOPO_ADD                             "$sys/%s/%s/topo/add"                //Gateway request to add topology relationship
#define UNIR_UNIOT_MQTT_TOPIC_TOPO_ADD_REPLY                       "$sys/%s/%s/topo/add_reply"          //Cloud response gateway adds topology relationship
#define UNIR_UNIOT_MQTT_TOPIC_TOPO_DELETE                          "$sys/%s/%s/topo/delete"             //	Gateway request to delete topology relationship
#define UNIR_UNIOT_MQTT_TOPIC_TOPO_DELETE_REPLY                    "$sys/%s/%s/topo/delete_reply"       //	Cloud response gateway deletes topology relationship
#define UNIR_UNIOT_MQTT_TOPIC_TOPO_GET                             "$sys/%s/%s/topo/get"                //Gateway request to obtain topology relationships
#define UNIR_UNIOT_MQTT_TOPIC_TOPO_GET_REPLY                       "$sys/%s/%s/topo/get_reply"          //Cloud responds to gateway request to obtain topology relationship
#define UNIR_UNIOT_MQTT_TOPIC_SUBDEVICE_BATCHLOGIN                 "$sys/%s/%s/subdevice/batchlogin"    //Batch online of sub-devices
#define UNIR_UNIOT_MQTT_TOPIC_SUBDEVICE_BATCHLOGIN_REPLY           "$sys/%s/%s/subdevice/batchlogin_reply"          //	Cloud response batch online request
#define UNIR_UNIOT_MQTT_TOPIC_SUBDEVICE_BATCHLOGOUT                "$sys/%s/%s/subdevice/batchlogout"               //	Batch offline of sub-devices
#define UNIR_UNIOT_MQTT_TOPIC_SUBDEVICE_BATCHLOGOUT_REPLY          "$sys/%s/%s/subdevice/batchlogout_reply"         //	Cloud response batch offline request
#define UNIR_UNIOT_MQTT_TOPIC_SUBDEVICE_PROPERTY_EVENT_PUB         "$sys/%s/%s/subdevice/property/event/pub"        //Sub-devices report data in batches through the gateway
#define UNIR_UNIOT_MQTT_TOPIC_SUBDEVICE_PROPERTY_EVENT_PUB_REPLY   "$sys/%s/%s/subdevice/property/event/pub_reply"  //	Cloud response gateway uploads data
#define UNIR_UNIOT_MQTT_TOPIC_SUBDEVICE_DELETE                     "$sys/%s/%s/subdevice/delete"                    //	Platform notifies gateway that sub-device has been deleted
#define UNIR_UNIOT_MQTT_TOPIC_SUBDEVICE_DELETE_REPLY               "$sys/%s/%s/subdevice/delete_reply"              //	Device responds to cloud notification

//Device Shadow related Topic
#define UNIR_UNIOT_MQTT_TOPIC_DEVICESHADOW_GET                     "$sys/%s/%s/deviceShadow/get"            //Device retrieves device shadow data
#define UNIR_UNIOT_MQTT_TOPIC_DEVICESHADOW_GET_REPLY               "$sys/%s/%s/deviceShadow/get_reply"      //	Cloud response device to get device shadow
#define UNIR_UNIOT_MQTT_TOPIC_DEVICESHADOW_COMMAND                 "$sys/%s/%s/deviceShadow/command"        //Remote device shadow command issued by the cloud
#define UNIR_UNIOT_MQTT_TOPIC_DEVICESHADOW_COMMAND_REPLY           "$sys/%s/%s/deviceShadow/command_reply"  //	Device responds to device shadow commands
#define UNIR_UNIOT_MQTT_TOPIC_DEVICESHADOW_UPDATE                  "$sys/%s/%s/deviceShadow/update"         //	Device reports attribute values to device shadow
#define UNIR_UNIOT_MQTT_TOPIC_DEVICESHADOW_UPDATE_REPLY            "$sys/%s/%s/deviceShadow/update_reply"   //	Cloud response to device reporting

//Dynamic registration of Topic
#define UNIR_UNIOT_MQTT_TOPIC_EXT_REGIST                           "$sys/%s/%s/ext/regist"        //	Dynamically register (pre-register) device keys issued by the cloud
#define UNIR_UNIOT_MQTT_TOPIC_EXT_REGIST_REPLY                     "$sys/%s/%s/ext/regist_reply"  //	Device response dynamic registration (pre-registration) cloud issues device secret key
#define UNIR_UNIOT_MQTT_TOPIC_EXT_AUTOREGIST                       "$sys/%s/%s/ext/autoregist"  //Dynamic registration (without pre-registration) cloud issues device secret
#define UNIR_UNIOT_MQTT_TOPIC_EXT_AUTOREGIST_REPLY                 "$sys/%s/%s/ext/autoregist_reply"  //	Device responds to dynamic registration (pre-registration exempt) and the cloud issues the device key
//Module
#define UNIR_UNIOT_MQTT_TOPIC_MODEULE_AUTOREGIST                   "$sys/%s/%s/module/autoregist"        //Module Dynamic Registration
#define UNIR_UNIOT_MQTT_TOPIC_MODULE_AUTOREGIST_REPLY              "$sys/%s/%s/module/autoregist_reply"  //	Module device responds to dynamic registration and cloud-issued device key

//Error Topic
#define UNIR_UNIOT_MQTT_TOPIC_EXT_ERROR                            "$sys/%s/%s/ext/error"  //Platform publishes error topic

//PSM
#define UNIR_UNIOT_MQTT_TOPIC_PSMENABLE_GET                        "$sys/%s/%s/psmEnabled/get"        //Get the platform's PSM enable status
#define UNIR_UNIOT_MQTT_TOPIC_PSMENABLE_GET_REPLY                  "$sys/%s/%s/psmEnabled/get_reply"  //Get the platform's PSM enable status
#define UNIR_UNIOT_MQTT_TOPIC_PSMENABLE_GET_STRING_END             "/psmEnabled/get"                  //Get the platform's PSM enable status
#define UNIR_UNIOT_MQTT_TOPIC_PSMENABLE_GET_REPLY_STRING_END       "/psmEnabled/get_reply"

#define UNIR_UNIOT_MQTT_TOPIC_EXT_REGIST_STRING_END                "/ext/regist"
#define UNIR_UNIOT_MQTT_TOPIC_EXT_REGIST_REPLY_STRING_END          "/ext/regist_reply"
#define UNIR_UNIOT_MQTT_TOPIC_EXT_AUTOREGIST_STRING_END            "/ext/autoregist"
#define UNIR_UNIOT_MQTT_TOPIC_EXT_MODULE_AUTOREGIST_STRING_END     "/module/autoregist_reply"  //One Module One Secret
#define UNIR_UNIOT_MQTT_TOPIC_EXT_AUTOREGIST_REPLY_STRING_END      "/ext/autoregist_reply"
#define UNIR_UNIOT_MQTT_TOPIC_DEVICESHADOW_COMMAND_STRING_END      "/deviceShadow/command"
#define UNIR_UNIOT_MQTT_TOPIC_PROPERTY_BATCH_REPLY_STRING_END      "/property/batch_reply"
//#define UNIR_UNIOT_MQTT_TOPIC_PSMENABLE_GET_STRING_END 				"/psmEnable/get"
//#define UNIR_UNIOT_MQTT_TOPIC_PSMENABLE_GET_REPLY_STRING_END 		"/psmEnable/get_reply"
#define UNIR_UNIOT_MQTT_TOPIC_DEVICECARD_PUB_STRING_END            "/deviceCard/pub"
#define UNIR_UNIOT_MQTT_TOPIC_DEVICECARD_PUB_REPLY_STRING_END      "/deviceCard/pub_reply"
#define UNIR_UNIOT_MQTT_TOPIC_DEVICESHADOW_GET_REPLY_STRING_END    "/deviceShadow/get_reply"
#define UNIR_UNIOT_MQTT_TOPIC_DEVICECARD_UPDATE_STRING_END         "/deviceCard/update"
#define UNIR_UNIOT_MQTT_TOPIC_DEVICECARD_UPDATE_REPLY_STRING_END   "/deviceCard/update_reply"

#define UNIR_UNIOT_MQTT_TOPIC_DEVICESHADOW_UPDATE_REPLY_STRING_END "/deviceShadow/update_reply"  //	Cloud response to device reporting
#define UNIR_UNIOT_MQTT_TOPIC_DEVICESHADOW_GET_STRING_END          "/deviceShadow/get"
#define UNIR_UNIOT_MQTT_TOPIC_PUB_REPLY_STRING_END                 "/pub_reply"
#define UNIR_UNIOT_MQTT_TOPIC_PUB_STRING_END                       "/pub"
#define UNIR_UNIOT_MQTT_TOPIC_GET_REPLY_STRING_END                 "/get_reply"

//Components of the message_id in the payload of the mqtt publish that constitutes the China Unicom custom command
#define OSA_UNIFY_MQTT_MSGID_VERSION_MAJOR                         2
#define OSA_UNIFY_MQTT_MSGID_VERSION_MINOR                         0
#define OSA_UNIFY_MQTT_MSGID_VERSION_PATCH                         0

typedef enum
{
    UNIR_UNIFY_GENERAL_ERROR = -1,                                 //error
    UNIR_UNIFY_SUCCESS = 0,                                        //Success
    UNIR_UNIFY_REPLY_CODE_SUCCESS = 00000,                         //Enable successful;
    UNIR_UNIFY_INTERNAL_SERVER_ERROR = 90000,                      // Internal server runtime error.
    UNIR_UNIFY_INVALID_TOPIC = 90003,                              //Invalid topic.
    UNIR_UNIFY_INVALID_JSON = 90007,                               //Invalid JSON format.
    UNIR_UNIFY_SUBDEVICE_NO_SESSION = 90036,                       //Sub-device session does not exist, device is offline.
    UNIR_UNIFY_REPLY_CODE_ACCOUNT_NOT_EXIT = 90068,                //Account does not exist 0
    UNIR_UNIFY_REPLY_CODE_CARD_BOUNDB = 90069,                     // Device has bound IoT relationship
    UNIR_UNIFY_REPLY_CODE_BOUND_BY_OTHERS = 90070,                 //ICCID or IMSI does not exist; 2
    UNIR_UNIFY_REPLY_CODE_ICCID_IMSI_NOT_EXIT = 90071,             // Device not bound to IoT relationship (update binding card); 3
    UNIR_UNIFY_REPLY_CODE_NOT_BIND_FOUND = 90072,                  //The input ICCID or IMSI already has a bound IoT relationship (first-time card binding); 4
    UNIR_UNIFY_REPLY_CODE_BINDING_FAILED = 90073,                  //Binding failed
    UNIR_UNIFY_MQTT_PARAM_NOT_CORRECT = 90095,                     //Incorrect parameter.
    UNIR_UNIFY_REPLY_CODE_UNSUBSCRIBED_OR_NO_RECEIVED = 92001,     //Not subscribed or no reply received;
    UNIR_UNIFY_REPLY_CODE_HARDWARE_ENABLE_UNSUCCESSFUL = 92002,    //Hardware enable failed;
    UNIR_UNIFY_REPLY_CODE_PLATFORM_IS_NOT_ENABLED = 92003,         //Failed, platform not enabled;
    UNIR_UNIFY_REPLY_CODE_INTERNET_THINGS_BINDING_FAILED = 92004,  //Failure, IoT binding unsuccessful;
    UNIR_UNIFY_REPLY_CODE_FAILED_UPDATE_INTERNET_THINGS = 92005,   //Failure, the physical network update was unsuccessful;
    UNIR_UNIFY_MQTT_NO_THIS_PROPERTY = 92006,                      //No such attribute
    UNIR_UNIFY_MQTT_AT_FORMAT_ERROR = 92007,                       //AT format error
    UNIR_UNIFY_OPERAT_KEY_FAILURE = 92008,                         //Key operation failed
    UNIR_UNIFY_AUTH_INFOR_ERR = 92012,                             //Authentication information error;
    UNIR_UNIFY_SERVER_REFUSE_ACCESS = 92013,                       //Platform refuses access;
    UNIR_UNIFY_NETWORK_EXCEPTION = 92014,                          //Network exception;
    UNIR_UNIFY_NO_MODULE_AUTH_INFO = 92015,                        //Module-less authentication information (one module, one secret login);
    UNIR_UNIFY_NO_MODULE_ACCOUT_OR_NOY_ACTIVA = 92016,             //No module account or module account not activated (one-module-one-secret login);
    UNIR_UNIFY_BINDING_FAILED = 92017,                             //Binding failed (card binding login);
    UNIR_UNIFY_EXECUTE_FAILED = 92018,                             //Execution failed
    UNIR_UNIFY_MQTT_OTHER_ERROR = 92019,                           //Other errors
    UNIR_UNIFY_DISCONNECT_FAILED = 92020,                          // Disconnect failed
    UNIR_UNIFY_MQTT_SUB_FAIL = 92021,                              // Subscription failed
    UNIR_UNIFY_MQTT_PUB_FAIL = 92022,                              //Publish failed
    UNIR_UNIFY_MQTT_CONNECT_FAIL = 92023,                          //Connection failed;
    UNIR_UNIFY_MQTT_COMMAND_PARAM_ERROR = 92024,                   //Command parameter error
    UNIR_UNIFY_MQTT_PSM_IS_ON_DISABLED_ERROR = 92026,              // PSM is enabled, failed to be disabled
} unir_unify_mqtt_error_e;

typedef enum
{
    UNIR_UNIFY_AUTH_TYPE_PRODUCT_REGIST_ONLINE = -2,  //Adopting the one-type-one-secret authentication method for online registration-free activation
    UNIR_UNIFY_AUTH_TYPE_DEVICE_ONLINE = 0,           //Using one-device-one-secret authentication method
    UNIR_UNIFY_AUTH_TYPE_PRODUCT_ACTIVE = 1,          //Using one-type-one-secret authentication pre-registration method
    UNIR_UNIFY_AUTH_TYPE_PRODUCT_REGIST_ACTIVE = 2,   //Activate using the one-type-one-secret authentication method without pre-registration
    UNIR_UNIFY_AUTH_TYPE_MODULE_LOGIN = 3,            //Use one-key-per-model authentication for login
    UNIR_UNIFY_AUTH_TYPE_MAX
} unir_unify_auth_type_e;

// Indicates the operator identifier. When the device uses an IoT card, please fill in "1" (China Unicom), "2" (China Mobile), "3" (China Telecom), "4" (China Broadcasting Network). If cellular network access is not used, please fill in "0";
typedef enum
{
    UNIR_UNIFY_OPERATOR_NOT_CELLULAR_NETWORK = 0,  //Please fill in the device's SN code or MAC address, within 10-32 characters, only numbers, uppercase/lowercase letters, and "-" are supported;
    UNIR_UNIFY_OPERATOR_UNICOM = 1,                //China Unicom SIM card ICCID
    UNIR_UNIFY_OPERATOR_CMCC = 2,                  //Mobile card ICCID
    UNIR_UNIFY_OPERATOR_CT = 3,                    //Telecom card ICCID
    UNIR_UNIFY_OPERATOR_NRTA = 4,                  //ICCID of the broadcasting and television card
    UNIR_UNIFY_OPERATOR_TEST = 5,                  //For testing purposes
    UNIR_UNIFY_OPERATOR_MAX
} unir_unify_operator_e;

typedef enum
{
    UNIR_UNIFY_DIRECT_CONNECT_DEVICE_MODEL = 0,  // Fill in the device's SN code or MAC address, within 10-32 characters, only numbers, uppercase/lowercase letters, and "-" are supported;
    UNIR_UNIFY_GATEWAY_SUBDEVICE_MODEL = 1,      // Fill in the ICCID number of the device's SIM card;
    UNIR_UNIFY_CUSTOM_DATA_SCHEMA = 2
} unir_unify_data_mode_e;

typedef enum
{
    UNIR_UNIFY_AUTO_BINDCAR_DISENABLE = 0,  // Fill in the device's SN code or MAC address, within 10-32 characters, supporting only digits, uppercase/lowercase letters, and "-".
    UNIR_UNIFY_AUTO_BINDCAR_ENABLE = 1,     // Fill in the ICCID number of the device's SIM card;
    UNIR_UNIFY_AUTO_BINDCAR_TEST
} unir_unify_auto_bind_flag_e;

typedef enum
{
    UNIR_UNIFY_MQTT_AT_REPLY_TIMRT_TYPE_INIT = 0,
    UNIR_UNIFY_MQTT_AT_REPLY_TIMRT_TYPE_DEVCARD_BIND = 1,  //Timer used by devCard bind
                                                           /*
	OSA_UNIFY_MQTT_AT_REPLY_TIMRT_TYPE_SHADOW_GET,
	OSA_UNIFY_MQTT_AT_REPLY_TIMRT_TYPE_SHADOW_UPDATA,
	Currently not required to be supported
*/
    UNIR_UNIFY_MQTT_AT_REPLY_TIMRT_TYPE_SET_PSM,           //Timer used by set psm
    UNIR_UNIFY_MQTT_AT_REPLY_TIMRT_TYPE_MAX = UNIR_UNIFY_MQTT_AT_REPLY_TIMRT_TYPE_INIT,
} unir_unify_mqtt_at_reply_timer_type_enum;                // Determine the type of timer startup

typedef enum
{
    UNIR_UNIFY_MQTT_AT_SUB_GENERAL_FLAG = 0,                    //General AT subscription, i.e., topics subscribed via the general sub AT command
    UNIR_UNIFY_MQTT_AT_SUB_DEVICECARD_PUB_REPLYL_FLAG = 1,      // Function  AT subscription IoT card first-time binding
    UNIR_UNIFY_MQTT_AT_SUB_DEVICECARD_UPDATE_REPLY_FLAG = 2,    // Function  AT subscribes to the IoT card update binding card
    UNIR_UNIFY_MQTT_AT_SUB_DEVICESHADOW_COMMAND_FLAG = 3,       // Function  AT subscribes to device shadow command
    UNIR_UNIFY_MQTT_AT_SUB_DEVICESHADOW_UPDATE_REPLY_FLAG = 4,  // Function AT subscribe to get device shadow reply
    UNIR_UNIFY_MQTT_AT_SUB_DEVICESHADOW_GET_REPLY_FLAG = 5,     // Function  AT subscribes to report device shadow reply
    UNIR_UNIFY_MQTT_AT_SUB_PSMENABLE_GET_REPLY_FLAG = 6,        // Function  AT subscription to get PSM reply
} unir_unify_mqtt_at_general_sub_flag_enum;

// Structure stored in the NV file.
typedef struct
{
    char PK[OSA_UNIFY_PK_STR_LEN_MAX];
    char PS[OSA_UNIFY_PS_STR_LEN_MAX];
    char DK[OSA_UNIFY_DK_STR_LEN_MAX];
    char DS[OSA_UNIFY_DS_STR_LEN_MAX];
    char token[OSA_UNIFY_TOKEN_STR_LEN_MAX];  //token cannot be obtained in AT commands, cannot be set manually, and can only be cleared.
} unir_unify_key_info_t;

typedef struct
{
    qosa_timer_t timer;
    int          mqtt_ctxid;
    int          wait_reply_type;
} unir_unify_platform_reply_timeout_t;

unir_unify_mqtt_error_e unir_unify_mqtt_save_auth_param(unir_unify_key_info_t *data, char *nvm_cfg_path);
unir_unify_mqtt_error_e unir_unify_mqtt_read_auth_param(unir_unify_key_info_t *data, char *nvm_cfg_path);
// typedef struct{
// 	int psm_state;											// Stores the current PSM state value that needs to be changed
// 	int psm_opt_stat;										//Stores the current status of the set PSM operation; for details, see the unir_unify_mqtt_at_psm_opt_stat_enum enumeration
// 	unir_unify_publish_t* 	get_psm_pub;					// Stores the information of the current publish's get
// 	unir_unify_publish_t* 	set_psm_pub;					// Stores the update information for the current publish
// 	unir_unify_sub_topic_t* get_reply_sub;					// Stores information for the current topic's get_reply
// 	unir_unify_platform_reply_timeout_t* timer_reply_ctx;	// Its primary purpose is to wait for the server's response, preventing the AT engine from not being released due to the server failing to return the get_reply information.
// }unir_unify_shadow_psm_t;

#endif /*_UNIRTOS_UNIFY_COMMON_H_*/
