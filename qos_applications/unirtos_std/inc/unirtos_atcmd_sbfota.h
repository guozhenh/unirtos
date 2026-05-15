/*****************************************************************/ /**
* @file unirtos_atcmd_sbfota.h
* @brief
* @author Elmo.huang@quectel.com
* @date 2024-10-08
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description
* <tr><td>2024-10-08 <td>1.0 <td>Elmo.huang <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_SBFOTA_H__
#define __UNIRTOS_ATCMD_SBFOTA_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"

/**
 * @enum unir_sbfota_err_code_e
 * @brief Error codes generated when executing SBFOTA AT-side functions.
 */
typedef enum
{
    UNIR_SBFOTA_OK = 0,                  /*!< Success */
    UNIR_SBFOTA_ERROR_MEMORY = 1,        /*!< Memory allocation failed. */
    UNIR_SBFOTA_ERROR_PARAM_INVALID = 2, /*!< Parameter error. */
    UNIR_SBFOTA_ERROR_PARAM_IS_NULL = 3, /*!< Parameter is empty. */
} unir_sbfota_err_code_e;

/*AutoFOTA upgrade delay time*/
#define UNIR_SBFOTA_MIN_PROTECT_TIME 0
#define UNIR_SBFOTA_MAX_PROTECT_TIME 60

/*Cycle time for the loop check version*/
#define UNIR_SBFOTA_MIN_POLLING_TIME 10
#define UNIR_SBFOTA_MAX_POLLING_TIME 1209600

void qstd_exec_qsbfota_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_qsbfota_cfg_cmd(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_SBFOTA_H__ */
