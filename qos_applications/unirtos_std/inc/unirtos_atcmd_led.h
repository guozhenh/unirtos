/*****************************************************************/ /**
* @file unirtos_atcmd_led.h
* @brief Declarations of AT command handling functions related to LED
* @author
* @date 2025-09-26
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2025-09-26 <td>1.0 <td>Initial <td> Init
* </table>
**********************************************************************/

#ifndef __UNIRTOS_ATCMD_LED_H__
#define __UNIRTOS_ATCMD_LED_H__

#include "qosa_def.h"
#include "qosa_at_cmd.h"

void qstd_exec_qledstat_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_qledcfg_cmd(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_LED_H__ */
