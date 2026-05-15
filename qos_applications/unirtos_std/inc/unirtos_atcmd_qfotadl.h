/*****************************************************************/ /**
* @file unirtos_atcmd_qfotadl.h
* @brief
* @author larson.li@quectel.com
* @date 2024-06-19
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-06-19 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/

#ifndef UNIR_ATCMD_QFOTADL_H
#define UNIR_ATCMD_QFOTADL_H

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_datacall.h"
#include "qosa_at_param.h"

// Define the timeout parameter range and default values for HTTP connections.
#define UNIR_FOTA_HTTP_TIMEOUT_MIN           1    // Minimum HTTP timeout value (seconds)
#define UNIR_FOTA_HTTP_TIMEOUT_MAX           120  // Maximum HTTP timeout value (seconds)
#define UNIR_FOTA_HTTP_TIMEOUT_DEFAULT       60   // Default value for HTTP timeout (seconds)

// Define the parameter ranges and default values for HTTP retry count and interval.
#define UNIR_FOTA_HTTP_RETRYCOUNT_MIN        1    // Minimum number of HTTP request retries
#define UNIR_FOTA_HTTP_RETRYCOUNT_MAX        24   // Maximum number of HTTP request retries
#define UNIR_FOTA_HTTP_RETRYCOUNT_DEFAULT    6    // Default number of HTTP request retries

#define UNIR_FOTA_HTTP_RETRYINTERVAL_MIN     1    // Minimum interval for HTTP request retries (seconds)
#define UNIR_FOTA_HTTP_RETRYINTERVAL_MAX     120  // Maximum interval for HTTP request retries (seconds)
#define UNIR_FOTA_HTTP_RETRYINTERVAL_DEFAULT 6    // Default value for HTTP request retry interval (seconds)

/**
 * @brief Enumeration defining FOTA configuration options
 *
 * This enumeration type is used to specify different options for FOTA (Firmware Over-The-Air) configuration, including:
 * - FOTA_CFG_OPT_CID: Customer identifier for the FOTA session
 * - FOTA_CFG_OPT_HTTP_TIMES: Number of HTTP retry attempts
 * - FOTA_CFG_OPT_MAX: Maximum value of the enumeration, used as a boundary condition
 */
typedef enum
{
    FOTA_CFG_OPT_CID,         // Client identifier for the FOTA session
    FOTA_CFG_OPT_HTTP_TIMES,  // HTTP retry attempts
    FOTA_CFG_OPT_MAX          // Maximum value of the enumeration type
} fota_cfg_opt_e;

/**
 * @brief Defines a structure type named unirtos_fota_cfg_t for configuring HTTP connection properties and retry mechanisms for FOTA (Firmware Over-The-Air).
 */
typedef struct
{
    qosa_uint32_t fota_http_connect_timeout; /*!< HTTP connection timeout, in milliseconds */
    qosa_uint32_t fota_http_retry_count;     /*!< Maximum number of retries after an HTTP connection failure */
    qosa_uint32_t fota_http_retry_interval;  /*!< Interval time between HTTP connection retries, in milliseconds */
    qosa_uint32_t context_id;                /*!< Context ID, used to identify a specific FOTA update session */
} unirtos_fota_cfg_t;

/*========================================================================

FUNCTION: qcfg_fota_cate_get

! @brief
<Function Definition Description>
    Returns the corresponding enumeration based on the AT+QCFG command parameter.

@param[in][out] <Parameter Name> <Parameter Description>
   char *name  String type, the first parameter of the AT command AT+QCFG.

@return
<Return Value Number or Enumeration> -- <Return Value Description>
    Success:
    FOTA_CFG_OPT_CID            Indicates the parameter is CID-related.
    FOTA_CFG_OPT_TIMES          Indicates the parameter is HTTP connection timeout-related.
    Failure:
    UNIR_GENERAL_ERROR          Indicates the parameter is invalid data.
    .
@dependencies
<Dependencies for calling this function and parameter validity.>
    This function is called within the AT+QCFG command execution functions
    unirtos_exec_qcfg_fota_cid_cmd or unirtos_exec_qcfg_fota_times_cmd.
=======================================================================*/
int qcfg_fota_cate_get(char *name);

/*========================================================================
FUNCTION: at_fota_config_get_opt

! @brief
<Function Definition Description>
    Retrieves the content of the FOTA-related configuration information `unir_fota_cfg_param` based on the enumeration `fota_cfg_opt_e`.

@param[in][out] <Parameter Name> <Parameter Description>
   fota_cfg_opt_e fota_cfg_opt          Enumeration corresponding to the first parameter of the AT+QCGF command.
   void * option                        Used to obtain the content of the FOTA-related configuration information `unir_fota_cfg_param`.

@return
<Return Value Number or Enumeration> -- <Return Value Description>
    Success:
    UNIR_NO_ERROR               Indicates success.
    Failure:
    UNIR_GENERAL_ERROR          Indicates a parameter error.
    .
@dependencies
<Dependencies for calling this function and parameter validity.>
    This function is called within the AT+QCFG command execution functions `unirtos_exec_qcfg_fota_cid_cmd`
    or `unirtos_exec_qcfg_fota_times_cmd`.
=======================================================================*/
int at_fota_config_get_opt(fota_cfg_opt_e fota_cfg_opt, void *option);

/*========================================================================
FUNCTION: at_fota_config_set_opt

! @brief
<Function Definition Description>
    Sets the content of the FOTA-related configuration information `unir_fota_cfg_param` according to the enumeration `fota_cfg_opt_e`.

@param[in][out] <Parameter Name> <Parameter Description>
   fota_cfg_opt_e fota_cfg_opt          The enumeration corresponding to the first parameter of the AT+QCGF command.
   void * option                        The content of the FOTA-related configuration information `unir_fota_cfg_param` stored for the second parameter of the AT+QCGF command.

@return
<Return Value Number or Enumeration> -- <Return Value Description>
    Success:
    UNIR_NO_ERROR               Indicates success.
    Failure:
    UNIR_GENERAL_ERROR          Indicates parameter failure.
    .
@dependencies
<Dependencies for calling this function and parameter validity.>
    This function is called within the AT+QCFG command execution functions `unirtos_exec_qcfg_fota_cid_cmd`
    or `unirtos_exec_qcfg_fota_times_cmd`.
=======================================================================*/
int at_fota_config_set_opt(fota_cfg_opt_e fota_cfg_opt, void *option);

/**
 * @brief <AT Command Format Description>
 *
 * AT+QFOTADL=<url>,[<upgrade_mode>,<download_URC_max>,<update_URC_max>]
 *
 * url: String type. The URL of the target firmware package stored on an FTP server, HTTP server, or PC. Maximum length: 255 bytes.
 *
 * upgrade_mode: Integer type. 0 Update after reboot; 1 Update immediately after the target firmware package is successfully downloaded; 2 File protocol FOTA.
 *
 * download_URC_max:
 *                  Integer type. Download progress URC; the final URC indicates download completion.
 *                  Range: (0,5-100).
 *                  0        Disable reporting download progress URC.
 *                  5?100    Maximum number of download progress URC reports.
 *
 *  update_URC_max:
 *                  Integer type. Upgrade progress URC; the final URC indicates upgrade completion.
 *                  Range: (0,5-100).
 *                  0        Disable reporting upgrade progress URC.
 *                  5?100    Maximum number of upgrade progress URC reports.
 *
 * @dependencies
 *   When using the FILE download method, the AT+QFUPL command must be sent first to upload the FOTA upgrade package file, before the FOTA upgrade command can be sent.
 *
 */
void qstd_exec_qfotadl_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief Get the FOTA upgrade status after power-on reboot
 *
 * @return qosa_uint8_t
 *       - Returns 1: FOTA upgrade successful
 *       - Returns 0: FOTA upgrade failed
 */
qosa_uint8_t unirtos_get_fota_result(void);

/**
 * @brief After the FOTA upgrade is successfully completed, the FOTA files in the current partition should be deleted.
 *
 */
void unirtos_fota_file_reset(void);

typedef void (*external_fota_cb)(int status, int ret);

/**
 * @brief Start FOTA (Firmware Over-The-Air)
 *
 * This function is used to initialize and start the firmware upgrade process, configuring the upgrade environment and selecting the upgrade path via specified parameters.
 * It is a key interface in the firmware upgrade module, connecting external callers with internal upgrade logic. Upgrade progress is broadcast via URC by default.
 * Does not support file name or file transfer upgrade methods.
 *
 * @param char *url
 *           - Pointer to the data required for firmware upgrade, including firmware metadata and the actual firmware package, etc.
 * @param qosa_uint8_t sim_id
 *           - SIM card identifier, used to specify the SIM card to be used for the firmware upgrade, especially in multi-SIM environments.
 * @param qosa_uint8_t pdp_id
 *           - PDP context identifier, used to define the network connection for data transmission, particularly important in mobile networks.
 * @param qosa_uint8_t upgrade_flag
 *           - Upgrade flag, indicating the type or mode of this firmware upgrade, which may affect the specific behavior of the upgrade process.
 * @param qosa_uint8_t download_urc_num
 *           - Download URC (Unsolicited Result Code) number, indicating the expected number of URC reports to be received during the firmware download phase.
 * @param qosa_uint8_t update_urc_num
 *           - Update URC number, indicating the expected number of URC reports to be received during the firmware update phase.
 *
 * @return int
 *       - Returns 0 on success, other values indicate failure.
 */
int unirtos_fota_external_fota_start(char *url, int sim_id, int pdp_id, int upgrade_flag, int download_urc_num, int update_urc_num, external_fota_cb cb);

#endif /* UNIR_ATCMD_QFOTADL_H */
