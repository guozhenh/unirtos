/*****************************************************************/ /**
* @file unirtos_atcmd_ftp.h
* @brief
* @author larson.li@quectel.com
* @date 2024-04-08
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-04-08 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_FTP_H__
#define __UNIRTOS_ATCMD_FTP_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"

void qstd_exec_ftp_qftpcfg_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftpopen_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftpclose_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftpput_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftpputex_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftpget_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftpsize_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftpdel_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftpcwd_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftppwd_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftpmkdir_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftprmdir_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftprename_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftpmdtm_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftplist_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftpnlst_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftpmlsd_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftpstat_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftplen_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_ftp_qftpdbg_cmd_func(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_FTP_H__ */
