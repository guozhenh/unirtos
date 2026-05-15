/*****************************************************************/ /**
* @file unirtos_atcmd_tcpip.h
* @brief
* @author larson.li@quectel.com
* @date 2023-10-14
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2023-10-14 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_TCPIP_H__
#define __UNIRTOS_ATCMD_TCPIP_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"

void qstd_exec_tcpip_qicfg_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_tcpip_qiopen_cmd(qosa_at_cmd_t *cmd);

void qstd_exec_tcpip_qird_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_tcpip_qiclose_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_tcpip_qisend_cmd(qosa_at_cmd_t *cmd);

void qstd_exec_tcpip_qisendex_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_tcpip_qisendhex_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_tcpip_qistate_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_tcpip_qiswtmd_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_tcpip_qisde_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_tcpip_qigeterror_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_tcpip_qiaccept_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_tcpip_qicsgp_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_tcpip_qiact_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_tcpip_qiactex_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_tcpip_qideact_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_tcpip_qideactex_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_tcpip_qidnscfg_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_tcpip_qidnsgip_cmd(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_TCPIP_H__ */
