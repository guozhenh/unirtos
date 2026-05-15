/*****************************************************************/ /**
* @file unirtos_atcmd_file.h
* @brief
* @author larson.li@quectel.com
* @date 2023-06-13
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2023-06-13 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_UIS8852_H__
#define __UNIRTOS_ATCMD_UIS8852_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"

void qstd_exec_platcfg_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_prodtest_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_amtphy_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_cmepcg_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_ftm_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_qreset_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_flistfile_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_flistpart_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_amtnv_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_cmset_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_reboot_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_slogctl_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_calibinfo_cmd(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_FILE_H__ */
