/*****************************************************************/ /**
* @file qosa_qvsim_adapt.h
* @brief
* @author joe.tu@quectel.com
* @date 2025-11-19
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2025-11-19 <td>1.0 <td>joe.tu <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_QVSIM_H__
#define __QOSA_QVSIM_H__

#include "qosa_def.h"

/** Maximum length of VSIM OTA HTTP URL */
#define QVSIM_OTA_HTTP_URL_LEN_MAX      (128 - 1)
/** Maximum length of VSIM OTA HTTP username */
#define QVSIM_OTA_HTTP_USERNAME_LEN_MAX (32 - 1)
/** Maximum length of VSIM OTA HTTP password */
#define QVSIM_OTA_HTTP_PASSWORD_LEN_MAX (32 - 1)

/** Maximum length of VSIM OTA PDN APN */
#define QVSIM_OTA_PDN_APN_LEN_MAX       (32 - 1)
/** Maximum length of VSIM OTA PDN username */
#define QVSIM_OTA_PDN_USER_LEN_MAX      (32 - 1)
/** Maximum length of VSIM OTA PDN password */
#define QVSIM_OTA_PDN_PWD_LEN_MAX       (32 - 1)

/** VSIM select profile by slot */
#define QOSA_QVSIM_SELECT_BY_SLOT       0
/** VSIM select profile by ICCID */
#define QOSA_QVSIM_SELECT_BY_ICCID      1

/** qvsim component error code base */
#define QOSA_ERRCODE_QVSIM_BASE         (QOSA_COMPONENT_QVSIM << 16)

/**
 * @enum qosa_qvsim_err_e
 * @brief VSIM error code enumeration type
 */
typedef enum
{
    QOSA_QVSIM_ERR_OK = 0, /*!< Operation successful */

    QOSA_QVSIM_ERR_UNKNOW = 1 | QOSA_ERRCODE_QVSIM_BASE,          /*!< Unknown error */
    QOSA_QVSIM_ERR_INVAL_PARM = 2 | QOSA_ERRCODE_QVSIM_BASE,      /*!< Invalid parameter error */
    QOSA_QVSIM_ERR_NOMEM = 3 | QOSA_ERRCODE_QVSIM_BASE,           /*!< Insufficient memory error */
    QOSA_QVSIM_ERR_BUSY = 4 | QOSA_ERRCODE_QVSIM_BASE,            /*!< Busy error, only one VSIM service can be executed at the same time */
    QOSA_QVSIM_ERR_ALREADY_STARTED = 5 | QOSA_ERRCODE_QVSIM_BASE, /*!< VSIM service is already started */
    QOSA_QVSIM_ERR_NO_PROFILE = 6 | QOSA_ERRCODE_QVSIM_BASE,      /*!< No profile error */
} qosa_qvsim_err_e;

/**
 * @enum qosa_qvsim_ota_err_e
 * @brief VSIM OTA error code enumeration type
 */
typedef enum
{
    QOSA_QVSIM_OTA_HTTP_OK = 0,                                         /*!< HTTP OK */
    QOSA_QVSIM_OTA_PDN_ACTIVATE_FAIL = 1,                               /*!< PDN activate failed */
    QOSA_QVSIM_OTA_DNS_FAIL = 2,                                        /*!< DNS resolution failed */
    QOSA_QVSIM_OTA_HTTP_CONNECT_FAIL = 3,                               /*!< HTTP connection failed */
    QOSA_QVSIM_OTA_RESPONSE_TIMEOUT = 4,                                /*!< Response timeout */
    QOSA_QVSIM_OTA_RESPONSE_DATA_ERROR = 5,                             /*!< Response data error */
    QOSA_QVSIM_OTA_UNKNOWN_ERROR = 6,                                   /*!< Unknown error */
    QOSA_QVSIM_OTA_PDN_DEACTIVATED_ERROR = 7,                           /*!< PDN deactivated error */
    QOSA_QVSIM_OTA_GET_TOKEN_ERROR = 8,                                 /*!< Get token error */
    QOSA_QVSIM_OTA_GET_HEARTBEAT_ERROR = 9,                             /*!< Get heartbeat error */
    QOSA_QVSIM_OTA_GET_RESULT_ERROR = 10,                               /*!< Get result error */
    QOSA_QVSIM_OTA_IS_DELETE_CURRENT_PROFILE = 11,                      /*!< Delete current profile */
    QOSA_QVSIM_OTA_IS_SLOT_FULL = 12,                                   /*!< Slot is full */
    QOSA_QVSIM_OTA_IS_PROFILE_ALREADY_EXIST = 13,                       /*!< Profile already exists */
    QOSA_QVSIM_OTA_SSL_CONFIG_FAIL = 14,                                /*!< SSL configuration failed */
    QOSA_QVSIM_OTA_NOT_ALLOWED_MODIFY_CUR_PROFILE = 15,                 /*!< Not allowed to modify current profile */

    QOSA_QVSIM_OTA_SERVER_ERROR_SERVICE_EXCEPTION = 101,                /*!< Server service exception */
    QOSA_QVSIM_OTA_SERVER_ERROR_CLIENT_INFO_IS_NULL_OR_EMPTY = 102,     /*!< Client info is null or empty */
    QOSA_QVSIM_OTA_SERVER_ERROR_CLIENT_INFO_NOT_EXISTS = 103,           /*!< Client info not exists */
    QOSA_QVSIM_OTA_SERVER_ERROR_CLIENT_SECRET_VALIDATE_INCORRECT = 104, /*!< Client secret validate incorrect */
    QOSA_QVSIM_OTA_SERVER_ERROR_PARAM_CHECK_ERROR = 105,                /*!< Parameter check error */
    QOSA_QVSIM_OTA_SERVER_ERROR_TOKEN_NOT_EXIST_OR_EXPIRED = 106,       /*!< Token not exist or expired */
    QOSA_QVSIM_OTA_SERVER_ERROR_UNAUTHORIZED = 107,                     /*!< Unauthorized */
    QOSA_QVSIM_OTA_SERVER_ERROR_IMEI_NOT_VALID = 108,                   /*!< IMEI not valid (IMEI: International Mobile Equipment Identity) */
    QOSA_QVSIM_OTA_SERVER_ERROR_IMEI_NOT_MATCH_UID = 109,               /*!< IMEI not match UID (IMEI: International Mobile Equipment Identity) */
    QOSA_QVSIM_OTA_SERVER_ERROR_NO_SIM = 110,                           /*!< No SIM card */
    QOSA_QVSIM_OTA_SERVER_ERROR_NO_ORDER = 111,                         /*!< No order */
    QOSA_QVSIM_OTA_SERVER_ERROR_RECORD_ID_ERROR = 112,                  /*!< Record ID error */
    QOSA_QVSIM_OTA_SERVER_ERROR_CODE_IS_ERROR = 113,                    /*!< Server error code is error */

    QOSA_QVSIM_OTA_GO_TO_FINISH = 1001,                                 /*!< Go to finish */
    QOSA_QVSIM_OTA_HAVE_MORE_TASK_WAITING = 1002,                       /*!< Have more task waiting */

    //pls add above.
    QOSA_QVSIM_OTA_ERROR_CODE_MAX /*!< Maximum OTA error code */
} qosa_qvsim_ota_err_e;

/**
 * @enum qosa_qvsim_status_e
 * @brief VSIM functional status (Enabled/Disabled)
 */
typedef enum
{
    QOSA_QVSIM_STATUS_DISABLED = 0, /*!< VSIM function is disabled / closed */
    QOSA_QVSIM_STATUS_ENABLED  = 1, /*!< VSIM function is enabled / opened */
} qosa_qvsim_status_e;

/**
 * @enum qosa_qvsim_event_e
 * @brief VSIM event enumeration type
 */
typedef enum
{
    QOSA_QVSIM_EVENT_INIT_DONE,          /*!< VSIM init done event */
    QOSA_QVSIM_EVENT_SELECT_PROFILE,     /*!< VSIM select profile event, data: qosa_qvsim_result_select_profile_t */
    QOSA_QVSIM_EVENT_START_PROFILE,      /*!< VSIM start profile event, data: qosa_qvsim_result_start_profile_t */
    QOSA_QVSIM_EVENT_SWITCH_IR,          /*!< VSIM switch IR event, data: qosa_qvsim_result_switch_ir_t */
    QOSA_QVSIM_EVENT_OTA_ADD_PROFILE,    /*!< VSIM OTA add profile event, data: qosa_qvsim_ota_event_data_t */
    QOSA_QVSIM_EVENT_OTA_DELETE_PROFILE, /*!< VSIM OTA delete profile event, data: qosa_qvsim_ota_event_data_t */
    QOSA_QVSIM_EVENT_OTA_SWITCH_PROFILE, /*!< VSIM OTA switch profile event, data: qosa_qvsim_ota_event_data_t */
    QOSA_QVSIM_EVENT_OTA_NO_TASK,        /*!< VSIM OTA no task event, data: qosa_qvsim_ota_event_data_t */
    QOSA_QVSIM_EVENT_OTA_CONNECTED,      /*!< VSIM OTA connected event, data: qosa_qvsim_ota_event_data_t */
    QOSA_QVSIM_EVENT_OTA_FINISHED,       /*!< VSIM OTA finished event, data: qosa_qvsim_ota_event_data_t */
    QOSA_QVSIM_EVENT_OTA_ERROR,          /*!< VSIM OTA error event, data: qosa_qvsim_ota_event_data_t */
} qosa_qvsim_event_e;

typedef struct
{
    qosa_qvsim_status_e status; /*!< VSIM functional status */
}qosa_qvsim_init_done_t;


/**
 * @struct qosa_qvsim_result_select_profile_t
 * @brief VSIM select profile result event data structure
 */
typedef struct
{
    qosa_int32_t result; /*!< Operation result code, 0:success, other:failed */
} qosa_qvsim_result_select_profile_t;

/**
 * @struct qosa_qvsim_result_start_profile_t
 * @brief VSIM start profile result event data structure
 */
typedef struct
{
    qosa_int32_t result; /*!< Operation result code, 0:success, other:failed */
} qosa_qvsim_result_start_profile_t;

/**
 * @struct qosa_qvsim_result_switch_ir_t
 * @brief VSIM switch IR result event data structure
 */
typedef struct
{
    qosa_int32_t result; /*!< Operation result code, 0:success, other:failed */
} qosa_qvsim_result_switch_ir_t;

/**
 * @struct qosa_qvsim_ota_event_data_t
 * @brief VSIM OTA event data structure
 */
typedef struct
{
    union
    {
        struct
        {
            char iccid[24]; /*!< ICCID string for profile operations */
        } profile;          // add/delete/switch profile
        struct
        {
            int result; /*!< OTA result, 0=success, 1=failed */
        } finished;
        struct
        {
            int err_code; /*!< OTA error code, ref qosa_qvsim_ota_err_e */
        } error;
    } data;
} qosa_qvsim_ota_event_data_t;

/**
 * @struct qosa_qvsim_profile_t
 * @brief VSIM profile structure
 */
typedef struct
{
    int  slot;      /*!< Slot number */
    char iccid[32]; /*!< ICCID string */
} qosa_qvsim_profile_t;

/**
 * @struct qosa_qvsim_ota_config_t
 * @brief VSIM OTA configuration structure
 */
typedef struct
{
    char          username[QVSIM_OTA_HTTP_USERNAME_LEN_MAX]; /*!< Cloud server username */
    char          password[QVSIM_OTA_HTTP_PASSWORD_LEN_MAX]; /*!< Cloud server password */
    char          url[QVSIM_OTA_HTTP_URL_LEN_MAX];           /*!< Cloud server address */
    qosa_uint16_t ota_time;                                  /*!< Maximum waiting time for OTA completion, range 100-1200 seconds, default 120 seconds */
} qosa_qvsim_ota_config_t;

/**
 * @struct qosa_qvsim_ota_pdn_config_t
 * @brief VSIM OTA PDN configuration structure
 */
typedef struct
{
    char apn[QVSIM_OTA_PDN_APN_LEN_MAX];       /*!< PDN context APN */
    char username[QVSIM_OTA_PDN_USER_LEN_MAX]; /*!< PDN context username */
    char password[QVSIM_OTA_PDN_PWD_LEN_MAX];  /*!< PDN context password */
} qosa_qvsim_ota_pdn_config_t;

/** VSIM event callback function type definition */
typedef void (*qosa_qvsim_event_cb_t)(qosa_qvsim_event_e event, void *event_data, void *user_data);

/**
 * @brief Register VSIM event callback function
 *
 * @param[in] callback
 *          - Event callback function pointer
 * @param[in] user_data
 *          - User data pointer passed to callback function
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_register_callback(qosa_qvsim_event_cb_t callback, void *user_data);

/**
 * @brief Start VSIM service
 *
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_start(void);

/**
 * @brief Stop VSIM service
 *
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_stop(void);

/**
 * @brief Query VSIM service running status
 *
 * @return qosa_qvsim_status_e
 */
qosa_qvsim_status_e qosa_qvsim_get_status(void);

/**
 * @brief List all available VSIM profiles
 *
 * @param[out] profiles
 *          - Profile array to store returned profile information
 * @param[out] profile_count
 *          - Actual number of returned profiles
 * @param[in] max_count
 *          - Maximum capacity of profile array
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_list_profiles(qosa_qvsim_profile_t *profiles, qosa_uint8_t *profile_count, qosa_uint8_t max_count);

/**
 * @brief Select VSIM profile
 *
 * @param[in] type
 *          - Selection type, 0: select by slot, 1: select by ICCID
 * @param[in] value_slot
 *          - Valid when type=0, represents slot number
 * @param[in] iccid
 *          - Valid when type=1, represents ICCID string
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_select_profile(int type, int value_slot, const char *iccid);

/**
 * @brief Query currently enabled VSIM profile
 *
 * @param[out] profile
 *          - Output current profile information
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_get_current_profile(qosa_qvsim_profile_t *profile);

/**
 * @brief Get COS version information
 *
 * @param[out] version
 *          - Version information string
 * @param[in] len
 *          - Length of version buffer
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_get_version(char *version, qosa_uint8_t len);

/**
 * @brief Switch IR (International Roaming)
 *
 * @param[in] ir
 *          - IR identifier
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_switch_ir(char *ir);

/**
 * @brief Get current IR (International Roaming) information
 *
 * @param[out] ir
 *          - IR identifier
 * @param[in] len
 *          - Length of ir buffer
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_get_current_ir(char *ir, qosa_uint8_t len);

/**
 * @brief Get sub version information
 *
 * @param[out] sub_version
 *          - Sub version information string
 * @param[in] len
 *          - Length of sub_version buffer
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_get_sub_version(char *sub_version, qosa_uint8_t len);

/**
 * @brief Get device unique identifier
 *
 * @param[out] uid
 *          - Device unique identifier string
 * @param[in] len
 *          - Length of uid buffer
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_get_uid(char *uid, qosa_uint8_t len);

/**
 * @brief Write VSIM profile
 *
 * @param[in] slot
 *          - Slot number
 * @param[in] profile_data
 *          - Profile data
 * @param[in] len
 *          - Length of profile data
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_write_profile(qosa_uint32_t slot, const qosa_uint8_t *profile_data, qosa_uint32_t len);

/**
 * @brief Format VSIM storage
 *
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_format(void);

/**
 * @brief Configure VSIM OTA parameters
 *
 * @param[in] config
 *          - OTA configuration parameters
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_set_ota_config(const qosa_qvsim_ota_config_t *config);

/**
 * @brief Query VSIM OTA parameters
 *
 * @param[out] config
 *          - Buffer to store current configuration
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_get_ota_config(qosa_qvsim_ota_config_t *config);

/**
 * @brief Configure VSIM OTA PDN parameters
 *
 * @param[in] pdn_config
 *          - OTA PDN configuration parameters
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_set_ota_pdn_config(const qosa_qvsim_ota_pdn_config_t *pdn_config);

/**
 * @brief Query VSIM OTA PDN parameters
 *
 * @param[out] pdn_config
 *          - Buffer to store current PDN configuration
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_get_ota_pdn_config(qosa_qvsim_ota_pdn_config_t *pdn_config);

/**
 * @brief Trigger VSIM OTA function
 *
 * @return qosa_qvsim_err_e
 */
qosa_qvsim_err_e qosa_qvsim_start_ota(void);

#endif  // __QOSA_QVSIM_H__
