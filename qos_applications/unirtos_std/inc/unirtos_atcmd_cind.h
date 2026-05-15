/*****************************************************************/ /**
* @file unirtos_atcmd_cind.h
* @brief
* @author liaz.liao@quectel.com
* @date 2025-01-05
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2025-01-05 <td>1.0 <td>Liaz.Liao <td> Init
* </table>
**********************************************************************/
#ifndef __QUECOS_ATCMD_CIND_H__
#define __QUECOS_ATCMD_CIND_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"
#include "qosa_system_config.h"

/**
 * @struct qosa_cind_sim_service_t
 * @brief Structure containing the current values of CIND indicators for a single SIM.
 */
typedef struct
{
    qosa_uint8_t signal_value;    /**< Current signal strength (0-5) */
    qosa_uint8_t service_value;   /**< Current service availability (0 or 1) */
    qosa_uint8_t message_value;   /**< Current message indicator (0 or 1) */
    qosa_uint8_t call_value;      /**< Current call activity (0 or 1) */
    qosa_uint8_t roam_value;      /**< Current roaming status (0 or 1) */
    qosa_uint8_t smsfull_value;   /**< Current SMS storage full status (0 or 1) */
} qosa_cind_sim_service_t;

/**
 * @struct qosa_cind_setting_t
 * @brief Complete CIND configuration structure containing both reporting settings and current values.
 */
typedef struct {
    qosa_bool_t battchg_present;      /**< Enable/disable battery charge level reporting */
    qosa_bool_t signal_present;       /**< Enable/disable signal quality reporting */
    qosa_bool_t service_present;      /**< Enable/disable service availability reporting */
    qosa_bool_t sounder_present;      /**< Enable/disable sounder activity reporting */
    qosa_bool_t message_present;      /**< Enable/disable message received reporting */
    qosa_bool_t call_present;         /**< Enable/disable call in progress reporting */
    qosa_bool_t vox_present;          /**< Enable/disable voice activity transmit reporting */
    qosa_bool_t roam_present;         /**< Enable/disable roaming indicator reporting */
    qosa_bool_t smsfull_present;      /**< Enable/disable SMS storage status reporting */
    qosa_bool_t inputstatus_present;  /**< Enable/disable keypad/touch screen status reporting */
    qosa_uint8_t battchg_value;       /**< Current battery charge level value (0-5) */
    qosa_uint8_t sounder_value;       /**< Current sounder activity value (0 or 1) */
    qosa_uint8_t vox_value;           /**< Current transmit activated by voice activity value (0 or 1) */
    qosa_uint8_t inputstatus_value;   /**< Current keypad/touch screen status value (0 or 1) */
    qosa_cind_sim_service_t sim_service_info[2];  /**< Array of SIM-specific CIND values for SIM IDs 0 and 1 */
} qosa_cind_setting_t;

/**
 * @enum qosa_cind_server_type_e
 * @brief Bitmask enumeration for enabling/disabling specific CIND indicator reporting.
 *
 * @note Each type represents a bit position in a bitmask, allowing multiple indicators
 *       to be configured simultaneously using bitwise OR operations.
 */
typedef enum {
    QOSA_CIND_BATTCHG_TYPE       = (1 << 0),   /**< 0000 0001 - Battery charging level */
    QOSA_CIND_SIGNAL_TYPE        = (1 << 1),   /**< 0000 0010 - Signal quality */
    QOSA_CIND_SERVICE_TYPE       = (1 << 2),   /**< 0000 0100 - Service availability */
    QOSA_CIND_SOUNDER_TYPE       = (1 << 3),   /**< 0000 0100 - Sounder activity */
    QOSA_CIND_MESSAGE_TYPE       = (1 << 4),   /**< 0000 0100 - Message received */
    QOSA_CIND_CALL_TYPE          = (1 << 5),   /**< 0000 0100 - Call in progress */
    QOSA_CIND_VOX_TYPE           = (1 << 6),   /**< 0000 0100 - Transmit activated by voice activity */
    QOSA_CIND_ROAM_TYPE          = (1 << 7),   /**< 0000 0100 - Roaming indicator */
    QOSA_CIND_SMSFULL_TYPE       = (1 << 8),   /**< 0000 0100 - SMS storage status */
    QOSA_CIND_INPUTSTATUS_TYPE   = (1 << 9),   /**< 0000 0100 - Keypad/touch screen status */
    QOSA_CIND_MAX_TYPE           = 0xFFFF      /**< All types */
} qosa_cind_server_type_e;

void unir_exec_cind_cmd(qosa_at_cmd_t *cmd);

#endif /* __QUECOS_ATCMD_CIND_H__ */
