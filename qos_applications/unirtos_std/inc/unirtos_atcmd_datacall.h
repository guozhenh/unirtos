/*****************************************************************/ /**
* @file unir_atcmd_datacall.h
* @brief
* @author Joe.tu@quectel.com
* @date 2024-05-16
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
#ifndef __UNIRTOS_ATCMD_DATACALL_H__
#define __UNIRTOS_ATCMD_DATACALL_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"

void qstd_exec_cgdcont_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_cgauth_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_cgatt_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_cgact_cmd(qosa_at_cmd_t *cmd);
// void qstd_exec_cgdata_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_cgpaddr_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_cgqreq_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_qgdcnt_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_qpdpgdcnt_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_qaugdcnt_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_cgcontrdp_cmd(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_DATACALL_H__ */
