/*****************************************************************/ /**
* @file qcm_lbs_app.h
* @brief
* @author lambert.zhao@quectel.com
* @date 2024-07-10
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-07-10 <td>1.0 <td>Lambert.Zhao <td> Init
* </table>
**********************************************************************/
#ifndef __QCM_LBS_APP_H__
#define __QCM_LBS_APP_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_rtc.h"

/** Maximum number of base stations */
#define QCM_LBS_MAX_CELL_NUM           6
/** Maximum number of location information */
#define QCM_LBS_MAX_POS_NUM            6
/** Maximum number of WiFi */
#define QCM_LBS_MAX_WIFI_NUM           6
/** Maximum length of username and password */
#define QCM_LBS_MAX_AUTH_INFO_LENGTH   64
/** Maximum length of token */
#define QCM_LBS_MAX_TOKEN_LENGTH       128
/** Maximum length of IMEI */
#define QCM_LBS_MAX_IMEI_LENGTH        64
/** Maximum length of WiFi MAC address */
#define QCM_LBS_MAX_MAC_LENGTH         18
/** Maximum length of WiFi SSID */
#define QCM_LBS_MAX_SSID_LENGTH        32

/** LBS uses SIM card 0 by default. */
#define QCM_LBS_DEFAULT_SIM_ID         0

/** Platform behavior differs; the platform specifies the output order of longitude and latitude */
#if defined(CONFIG_QOSA_RDA8910_R06_PLATFORM_FUNC)
#define QCM_LBS_LATORDER 1
#else  /*CONFIG_QOSA_RDA8910_R06_PLATFORM_FUNC*/
#define QCM_LBS_LATORDER 0
#endif /*CONFIG_QOSA_RDA8910_R06_PLATFORM_FUNC*/

/** LBS client ID */
typedef qosa_int32_t qcm_lbs_client_id;

/**
 * @enum qcm_lbs_location_method_e
 * @brief lbs location method
 */
typedef enum
{
    QCM_LBS_SINGLE_CELL_LOC_METHOD = 1, /** Single cell positioning */
    QCM_LBS_CELL_LOC_METHOD        = 4, /** Multiple cell positioning */
    QCM_LBS_CELL_WIFI_LOC_METHOD   = 5, /** Cell + WiFi hybrid positioning */
    QCM_LBS_WIFI_LOC_METHOD        = 6, /** WiFi positioning */
} qcm_lbs_location_method_e;

/**
 * @struct qcm_lbs_basic_info_t
 * @brief Basic information used for LBS positioning
 */
typedef struct
{
    qosa_uint8_t type;       /*!< Type: 01 for positioning, 02 for reporting */
    qosa_uint8_t encrypt;    /*!< Encryption scheme: 01 for XOR */
    qosa_uint8_t key_index;  /*!< In 01 encryption scheme, this parameter represents "scrambling KEY number", locally generated randomly, range 0-7 */
    qosa_uint8_t pos_format; /*!< Response packet type: 01 no address information needed, 02 address information needed */
    qosa_uint8_t loc_method; /*!< 01-Ordinary cell polling, 04-Multiple cell polling, 05-Cell+WiFi positioning, 06-WiFi only positioning */
} qcm_lbs_basic_info_t;

/**
 * @struct qcm_lbs_auth_info_t
 * @brief Authentication information for logging into LBS server
 */
typedef struct
{
    char          user_name[QCM_LBS_MAX_AUTH_INFO_LENGTH]; /*!< Username */
    char          user_pwd[QCM_LBS_MAX_AUTH_INFO_LENGTH];  /*!< Password */
    char          token[QCM_LBS_MAX_TOKEN_LENGTH];         /*!< Positioning access credential */
    char          imei[QCM_LBS_MAX_IMEI_LENGTH];           /*!< International Mobile Equipment Identity */
    qosa_uint16_t rand;                                    /*!< Random number */
} qcm_lbs_auth_info_t;

/**
 * @struct qcm_lbs_cell_info_t
 * @brief Save base station information obtained from LBS positioning
 */
typedef struct
{
    qosa_uint8_t  radio;    /*!< Radio type */
    qosa_uint16_t mcc;      /*!< Mobile Country Code */
    qosa_uint16_t mnc;      /*!< Mobile Network Code */
    qosa_int32_t  lac_id;   /*!< Location Area Code */
    qosa_int32_t  cell_id;  /*!< Cell ID */
    qosa_int16_t  signal;   /*!< Signal strength */
    qosa_uint16_t tac;      /*!< Tracking Area Code */
    qosa_uint16_t bcch;     /*!< Absolute Radio Frequency Channel Number for BCCH */
    qosa_uint8_t  bsic;     /*!< Base Station Identity Code/Base Station Color Code, for GSM */
    qosa_uint16_t uarfcndl; /*!< Downlink frequency, for WCDMA */
    qosa_uint16_t psc;      /*!< Primary Scrambling Code, for WCDMA */
    qosa_int16_t  rsrq;     /*!< Reference Signal Received Quality, for LTE */
    qosa_uint16_t pci;      /*!< Physical Cell ID, for LTE */
    qosa_uint16_t earfcn;   /*!< E-UTRA Absolute Radio Frequency Channel Number, for LTE */
    qosa_uint16_t reserve;  /*!< Reserved */
} qcm_lbs_cell_info_t;

/**
 * @struct qcm_lbs_wifi_mac_info_t
 * @brief Save WiFi information obtained from LBS positioning
 */
typedef struct
{
    char         wifi_mac[QCM_LBS_MAX_MAC_LENGTH];   /*!< MAC address of nearby Wi-Fi hotspot */
    qosa_int32_t wifi_rssi;                          /*!< Received Signal Strength Indicator */
    char         wifi_ssid[QCM_LBS_MAX_SSID_LENGTH]; /*!< WiFi Service Set Identifier */
} qcm_lbs_wifi_mac_info_t;

/**
 * @struct qcm_lbs_option_t
 * @brief Parameters for LBS request
 */
typedef struct
{
    qosa_uint8_t             pdp_cid;     /*!< PDP Context ID */
    qosa_uint8_t             sim_id;      /*!< SIM card ID used by LBS */
    qosa_int32_t             req_timeout; /*!< Maximum wait time for server data */
    qcm_lbs_basic_info_t    *basic_info;  /*!< Basic information used for LBS positioning */
    qcm_lbs_auth_info_t     *auth_info;   /*!< Authentication information for logging into LBS server */
    qosa_int32_t             cell_num;    /*!< Number of base stations obtained from LBS positioning */
    qcm_lbs_cell_info_t     *cell_info;   /*!< Base station information obtained from LBS positioning */
    qosa_int32_t             wifi_num;    /*!< Number of WiFi obtained from LBS positioning */
    qcm_lbs_wifi_mac_info_t *wifi_info;   /*!< WiFi information obtained from LBS positioning */
} qcm_lbs_option_t;

/**
 * @struct qcm_lbs_postion_info_t
 * @brief Location information obtained by LBS
 */
typedef struct
{
    float         longitude; /*!< Longitude */
    float         latitude;  /*!< Latitude */
    qosa_uint16_t accuracy;  /*!< Accuracy */
    qosa_uint8_t  flag;      /*!< 0 - Normal base station     1 - Invalid base station */
} qcm_lbs_postion_info_t;

/**
 * @enum qcm_lbs_result_code_e
 * @brief Result codes for LBS APP execution
 */
typedef enum
{
    QCM_LBS_SUCCESS = 0,
    QCM_LBS_LOC_FAIL = (QOSA_COMPONENT_LBS << 16) | 10000,                    /* Positioning failed */
    QCM_LBS_IMEI_ILLEGAL = (QOSA_COMPONENT_LBS << 16) | 10001,                /* Illegal IMEI number */
    QCM_LBS_TOKEN_NOT_EXIST = (QOSA_COMPONENT_LBS << 16) | 10002,             /* Token does not exist */
    QCM_LBS_TOKEN_LOC_EXCEED_MAX = (QOSA_COMPONENT_LBS << 16) | 10003,        /* Token positioning count exceeds maximum */
    QCM_LBS_IMEI_LOC_EXCEED_DAY_MAX = (QOSA_COMPONENT_LBS << 16) | 10004,     /* Device daily positioning count exceeds maximum */
    QCM_LBS_IMEI_LOC_VISIT_EXCEED_MAX = (QOSA_COMPONENT_LBS << 16) | 10005,   /* Token connected devices count exceeds maximum */
    QCM_LBS_TOKEN_EXPIRED = (QOSA_COMPONENT_LBS << 16) | 10006,               /* Token expired */
    QCM_LBS_IMEI_NO_AUTHORITY = (QOSA_COMPONENT_LBS << 16) | 10007,           /* This IMEI device cannot access service */
    QCM_LBS_TOKEN_LOC_VISIT_EXCEED_MAX = (QOSA_COMPONENT_LBS << 16) | 10008,  /* Token daily positioning count exceeds maximum */
    QCM_LBS_TOKEN_LOC_EXCEED_PERIOD_MAX = (QOSA_COMPONENT_LBS << 16) | 10009, /* Token positioning count exceeds maximum in period */
    QCM_LBS_DNS_FAIL = (QOSA_COMPONENT_LBS << 16) | 10101,
    QCM_LBS_MD5_FAIL = (QOSA_COMPONENT_LBS << 16) | 10102,
    QCM_LBS_MEMORY_FAIL = (QOSA_COMPONENT_LBS << 16) | 10103,
    QCM_LBS_NET_FAIL = (QOSA_COMPONENT_LBS << 16) | 10104,
    QCM_LBS_PARAM_FORMAT_FAIL = (QOSA_COMPONENT_LBS << 16) | 10105,
    QCM_LBS_TIMEOUT_FAIL = (QOSA_COMPONENT_LBS << 16) | 10106, /* LBS service timeout */
} qcm_lbs_result_code_e;

/**
 * @brief Initialize LBS request and get corresponding LBS operation handle
 *
 * Note: Resources will be automatically released after LBS callback notification, cannot be managed
 *
 * @return qcm_lbs_client_id
 *       - Success returns client id greater than 0
 *       - Failure returns -1
 */
qcm_lbs_client_id qcm_lbs_client_new(void);

/**
 * @brief LBS positioning results, notified to application through this function
 *
 * @param[in] qcm_lbs_client_id client_id
 *          - Corresponding LBS client ID
 *
 * @param[in] qcm_lbs_result_code result
 *          - Corresponding LBS result return code
 *
 * @param[in] qosa_int32_t pos_num
 *          - Number of location information obtained after successful LBS positioning
 *
 * @param[in] qcm_lbs_postion_info_t *pos_info
 *          - Location information obtained by LBS
 *
 * @param[in] char *date
 *          - Time returned by server
 *
 * @param[in] void * arg
 *          - User parameter input when registering callback function
 *
 */
typedef void (*qcm_lbs_response_callback)(
    qcm_lbs_client_id       client_id,
    qcm_lbs_result_code_e   result,
    qosa_int32_t            pos_num,
    qcm_lbs_postion_info_t *pos_info,
    char                   *date,
    void                   *arg
);

/**
 * @brief Query device location information based on base station information/WiFi information
 *
 * @param[in] qcm_lbs_client_id * lbs_cli_id
 *          - Corresponding LBS client ID
 *
 * @param[in] char *host
 *          - Server address
 *
 * @param[in] qcm_lbs_option_t *user_opts
 *          - Parameters for LBS request
 *
 * @param[in] qcm_lbs_response_callback cb
 *          - Callback function for LBS request results
 *
 * @param[in] void *arg
 *          - User-defined parameter
 *
 * @return qcm_lbs_result_code_e
 *       - Returns QCM_LBS_SUCCESS if successful
 *       - Returns other values if failed
 */
qcm_lbs_result_code_e qcm_lbs_get_position(qcm_lbs_client_id lbs_cli_id, char *host, qcm_lbs_option_t *user_opts, qcm_lbs_response_callback cb, void *arg);

#endif /* __QCM_LBS_APP_H__ */
