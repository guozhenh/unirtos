/*****************************************************************/ /**
* @file unirtos_atcmd_lwm2m.h
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
#ifndef __UNIRTOS_ATCMD_LWM2M_H__
#define __UNIRTOS_ATCMD_LWM2M_H__

#include "qosa_def.h"
#include "qosa_sys.h"

#include "qosa_at_param.h"

void qstd_exec_lwm2m_qlwcfg_cmd(qosa_at_cmd_t *cmd);
//AT+QLWCONFIG=<BS_enabled>,<serverIP>,<port>,<endpoint_name>,<lifetime>,<security_mode>[,<PSK_ID>,<PSK>]
void qstd_exec_lwm2m_qlwconfig_cmd(qosa_at_cmd_t *cmd);
//AT+QLWREG
void qstd_exec_lwm2m_qlwreg_cmd(qosa_at_cmd_t *cmd);
//AT+QLWUPDATE=<mode>,<lifetime/binding_mode>
void qstd_exec_lwm2m_qlwupdate_cmd(qosa_at_cmd_t *cmd);
//AT+QLWDEREG
void qstd_exec_lwm2m_qlwdereg_cmd(qosa_at_cmd_t *cmd);
//AT+QLWADDOBJ=<objectID>,<instanceID>,<res_num>,<resourceID>
void qstd_exec_lwm2m_qlwaddobj_cmd(qosa_at_cmd_t *cmd);
//AT+QLWDELOBJ=<objectID>
void qstd_exec_lwm2m_qlwdelobj_cmd(qosa_at_cmd_t *cmd);
//AT+QLWWRRSP=<messageID>,<result>
//+QLWURC: "write",36560,9,0,0,2,7,"5155454354454C",0  //Receive the write request from server
//AT+QLWWRRSP=36560,2
//OK
//
//+QLWWRRSP: 0
void qstd_exec_lwm2m_qlwwrrsp_cmd(qosa_at_cmd_t *cmd);
//AT+QLWRDRSP=<messageID>,<result>,<objectID>,<instanceID>,<resourceID>,<value_type>,<len>,<value>,<index>
void qstd_exec_lwm2m_qlwrdrsp_cmd(qosa_at_cmd_t *cmd);
//AT+QLWEXERSP=<messageID>,<result>
void qstd_exec_lwm2m_qlwexersp_cmd(qosa_at_cmd_t *cmd);
//AT+QLWOBSRSP=<messageID>,<result>,<objectID>,<instanceID>,<resourceID>,<value_type>,<len>,<value>,<index>
void qstd_exec_lwm2m_qlwobsrsp_cmd(qosa_at_cmd_t *cmd);
//AT+QLWNOTIFY=<objectID>,<instanceID>,<resourceID>,<value_type>,<len>,<value>,<index>[,<ack>[,<rai_flag>]]
void qstd_exec_lwm2m_qlwnotify_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_lwm2m_qlwrd_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_lwm2m_qlwstatus_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_lwm2m_qlwrecover_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_lwm2m_qlwreset_cmd(qosa_at_cmd_t *cmd);

void unir_lwm2m_at_init(void);

#endif /* __UNIRTOS_ATCMD_LWM2M_H__ */
