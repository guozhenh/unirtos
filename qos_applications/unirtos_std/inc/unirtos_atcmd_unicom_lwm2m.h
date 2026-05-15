/*****************************************************************/ /**
* @file unirtos_atcmd_unicom_lwm2m.h
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
#ifndef __UNIRTOS_ATCMD_UNICOM_LWM2M_H__
#define __UNIRTOS_ATCMD_UNICOM_LWM2M_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"

void qstd_exec_unicom_lwm2m_at_unilwcfg_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_unicom_lwm2m_at_unilwreg_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_unicom_lwm2m_at_unilwupdate_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_unicom_lwm2m_at_unilwunreg_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_unicom_lwm2m_at_unilwsend_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_unicom_lwm2m_at_unilwstate_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_unicom_lwm2m_at_unilwver_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_unicom_lwm2m_at_universion_cmd(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_UNICOM_LWM2M_H__ */
