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
#ifndef __UNIRTOS_ATCMD_FILE_H__
#define __UNIRTOS_ATCMD_FILE_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"

void qstd_exec_file_qfupl_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_file_qfdwl_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_file_qfmkdir_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_file_qfrmdir_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_file_qfopen_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_file_qflst_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_file_qfdel_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_file_qfclose_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_file_qfread_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_file_qflds_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_file_qfwrite_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_file_qfseek_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_file_qfposition_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_file_qftucat_cmd(qosa_at_cmd_t *cmd);
#ifdef CONFIG_QAPP_UNIRTOS_AT_QFMD5_CMD
void qstd_exec_file_qfmd5_cmd(qosa_at_cmd_t *cmd);
#endif /* CONFIG_QAPP_UNIRTOS_AT_QFMD5_CMD */
#endif /* __UNIRTOS_ATCMD_FILE_H__ */
