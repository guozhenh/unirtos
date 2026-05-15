/*****************************************************************/ /**
* @file unirtos_atcmd_dev.h
* @brief
* @author Joe.tu@quectel.com
* @date 2024-03-18
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description
* <tr><td>2024-04-16 <td>1.0 <td>Joe.tu <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_DEV_H__
#define __UNIRTOS_ATCMD_DEV_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"
#include "qosa_at_config.h"

void qstd_exec_cfun_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_cclk_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_qcfg_airplanecontrol_cmd(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_DEV_H__ */
