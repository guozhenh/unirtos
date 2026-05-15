/*****************************************************************/ /**
* @file qcm_dmhttp.h
* @brief
* @author kruskal.zhu@quectel.com
* @date   2025-10
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date    <th>Version <th>Author       <th>Description"
* <tr><td>2025-10 <td>1.0     <td>kruskal.zhu  <td> Init
* </table>
**********************************************************************/

#ifndef __QCM_DMHTTP_H__
#define __QCM_DMHTTP_H__

#include "qosa_def.h"
#include "qosa_sys.h"

#include "qosa_queue_list.h"

// DMHTTP operation default URLs, if user doesn't specify specific URL, use corresponding URL below based on operation type
#define QCM_DMHTTP_URL_GET_INFO            "https://a.fxltsbl.com/dm-multi-accept/multi/getMultiTerminalConfig"
#define QCM_DMHTTP_URL_POST_REG            "https://a.fxltsbl.com/dm-multi-accept/multi/acceptMultiTerminalRegInfo"
#define QCM_DMHTTP_URL_HEART_BEAT          "https://a.fxltsbl.com/dm-multi-accept/multi/acceptMultiTerminalHeartbeatInfo"
#define QCM_DMHTTP_URL_HEART_BEAT_TEMP     "https://b.fxltsbl.com/dm-multi-accept/multi/acceptMultiTerminalHeartbeatInfo"
#define QCM_DMHTTP_URL_COMMERCIAL          "https://a.fxltsbl.com"

// qcm_dmhttp_cfg_set configuration QCM_DMHTTP_CFG_FIELDCFG, delete/add operations
#define QCM_DMHTTP_CFG_FIELDCFG_DEL        0
#define QCM_DMHTTP_CFG_FIELDCFG_ADD        1

// qcm_dmhttp_cfg_set configuration heartbeat QCM_DMHTTP_CFG_HEART_BEAT
// Whether to enable heartbeat function
#define QCM_DMHTTP_CFG_HB_DISABLE          0
#define QCM_DMHTTP_CFG_HB_ENABLE           1
// Heartbeat cycle range (set to 0 to disable heartbeat)
#define QCM_DMHTTP_CFG_HB_TIME_MIN         0
#define QCM_DMHTTP_CFG_HB_TIME_MAX         86400
// Heartbeat count (set to 0 to disable heartbeat)
#define QCM_DMHTTP_CFG_HB_CNT_MIN          0
#define QCM_DMHTTP_CFG_HB_CNT_MAX          86400

// Whether to enable power-off save/auto-registration on boot function (when enabled, configuration parameters are saved in UFS, including DMHTTP named file directory)
#define QCM_DMHTTP_CFG_RESUME_SAVE_DISABLE 0
#define QCM_DMHTTP_CFG_RESUME_SAVE_ENABLE  1

/**
 * @brief qcm_dmhttp_err_e enum, defines dmhttp API function return error types
 *
 */
typedef enum
{
    QCM_DMHTTP_ERR_OK = 0,         /*!< Operation successful */
    QCM_DMHTTP_ERR_UNKNOW = 1,     /*!< Unknown error */
    QCM_DMHTTP_ERR_INVAL_PARM = 2, /*!< Invalid parameter error */
    QCM_DMHTTP_ERR_NOMEM = 3,      /*!< Insufficient memory error */
    QCM_DMHTTP_ERR_BUSY = 4,       /*!< Busy error, only one DMHTTP service can execute at a time */
} qcm_dmhttp_err_e;

/**
 * @brief qcm_dmhttp_cfg_e enum, defines qcm_dmhttp_cfg_set/get function operation types
 *
 */
typedef enum
{
    QCM_DMHTTP_CFG_EP_INFO = 0,     /*!< Query/registration request endpoint configuration */
    QCM_DMHTTP_CFG_FIELDCFG = 1,    /*!< Registration request fieldConfig configuration */
    QCM_DMHTTP_CFG_URL = 2,         /*!< Connection URL */
    QCM_DMHTTP_CFG_SIM = 3,         /*!< Connection SIMCID */
    QCM_DMHTTP_CFG_CID = 4,         /*!< Connection PDPCID */
    QCM_DMHTTP_CFG_KEY = 5,         /*!< Device information encryption key for reporting */
    QCM_DMHTTP_CFG_HEART_BEAT = 6,  /*!< Heartbeat configuration */
    QCM_DMHTTP_CFG_RESUME_SAVE = 7, /*!< Power-off save/auto-registration on boot */
    QCM_DMHTTP_CFG_MAX,
} qcm_dmhttp_cfg_e;

/**
 * @brief qcm_dmhttp_type_e enum, three processing types for request/response in dmhttp service
 *
 */
typedef enum
{
    QCM_DMHTTP_GET_INFO = 0,   /*!< Registration form query */
    QCM_DMHTTP_POST_REG = 1,   /*!< Self-registration */
    QCM_DMHTTP_HEART_BEAT = 2, /*!< Self-registration heartbeat */
} qcm_dmhttp_type_e;

/**
 * @brief DMHTTP callback notification function pointer.
 *        Pass callback function instance through qcm_dmhttp_set_notify_cb, and callback input parameter cb_data
 *
 * @param notify_type   Notification triggered by qcm_dmhttp_type_e type
 * @param result        DMHTTP specific execution result (QURL execution result, non-zero indicates execution exception)
 * @param http_code     HTTP specific response code returned by server (if any)
 * @param hb_remain_cnt Remaining heartbeat count
 * @param cb_data       cb_data input parameter set by qcm_dmhttp_set_notify_cb
 *
 */
typedef void (*qcm_dmhttp_notify_cb_ptr)(qcm_dmhttp_type_e notify_type, int result, int http_code, qosa_uint32_t hb_remain_cnt, void *cb_data);

/**
 * @brief  Set DMHTTP service configuration items, variadic function
 *
 * @param  opt_tag  Option tag of qcm_dmhttp_cfg_e type configuration item, used to specify configuration attribute to modify
 *
 * @return Function returns an error code indicating success or failure of operation. (Returns QCM_DMHTTP_ERR_OK on success, other error codes on failure)
 *
 */
qcm_dmhttp_err_e qcm_dmhttp_cfg_set(qcm_dmhttp_cfg_e opt_tag, ...);

/**
 * @brief  Get DMHTTP configuration information, variadic function
 *
 * @param  opt_tag  Option tag of qcm_dmhttp_cfg_e type configuration item, used to specify configuration information to get
 *
 * @return Function returns an error code indicating success or failure of operation. (Returns QCM_DMHTTP_ERR_OK on success, other error codes on failure)
 *
 */
qcm_dmhttp_err_e qcm_dmhttp_cfg_get(qcm_dmhttp_cfg_e opt_tag, ...);

/**
 * @brief  Registration form query request message preview
 *          Stitch ep_info information configured in QCM_DMHTTP_CFG_EP_INFO configuration item into complete message
 *
 * @return Returns string pointer of stitched ep_info.
 *         Returned pointer needs to be manually released, otherwise memory leak will occur.
 *
 */
char *qcm_dmhttp_ep_info_stitch_func(void);

/**
 * @brief  Self-registration request message preview.
 *          Stitch endpoint and fieldConfig information configured in QCM_DMHTTP_CFG_EP_INFO and QCM_DMHTTP_CFG_FIELDCFG configuration items into complete message
 *
 * @param  encode_enable Whether to display fieldConfig information in encrypted way
 *         QOSA_TRUE     Display fieldConfig configuration encrypted
 *         QOSA_FALSE    Display fieldConfig configuration in plain text
 *
 * @return Returns string pointer of stitched endpoint and fieldConfig.
 *         Returned pointer needs to be manually released, otherwise memory leak will occur.
 *
 */
char *qcm_dmhttp_fieldcfg_stitch_func(qosa_bool_t encode_enable);

/**
 * @brief  Get last response message from DMHTTP server for corresponding request type
 *         qcm_dmhttp_get_download_data only makes sense after qcm_dmhttp_perform execution is completed
 *
 * @param  type Set qcm_dmhttp_type_e corresponding type, function gets server response message after qcm_dmhttp_perform execution of corresponding type
 *
 * @return Returns response message string pointer from DMHTTP server, returns QOSA_NULL if not obtained
 *         This pointer does not need to be released
 *
 */
char *qcm_dmhttp_get_download_data(qcm_dmhttp_type_e type);

/**
 * @brief  Set DMHTTP callback notification function pointer, and callback function corresponding input parameter cb_data
 *
 * @param  cb       qcm_dmhttp_notify_cb_ptr function pointer type DMHTTP callback notification function pointer
 * @param  cb_data  Callback input parameter passed to cb, user decides, can be set to QOSA_NULL if no parameters to pass
 *
 * @return Function returns an error code indicating success or failure of operation. (Returns QCM_DMHTTP_ERR_OK on success, other error codes on failure).
 *
 */
qcm_dmhttp_err_e qcm_dmhttp_set_notify_cb(qcm_dmhttp_notify_cb_ptr cb, void *cb_data);

/**
 * @brief  Actual execution of dmhttp service
 *
 * @param  type  Set qcm_dmhttp_type_e corresponding type, initiate actual business of corresponding type
 *         Only allows users to initiate QCM_DMHTTP_GET_INFO and QCM_DMHTTP_POST_REG requests.
 *         If heartbeat function is configured, heartbeat request will be initiated after QCM_DMHTTP_POST_REG request is completed.
 *
 * @return Function returns an error code indicating success or failure of operation. (Returns QCM_DMHTTP_ERR_OK on success, other error codes on failure).
 *
 */
qcm_dmhttp_err_e qcm_dmhttp_perform(qcm_dmhttp_type_e type);

/**
 * @brief  Stop the current heartbeat and network registration timers, and change the current service status to IDLE.
 */
void qcm_dmhttp_stop_service(void);

#endif /* __QCM_DMHTTP_H__ */
