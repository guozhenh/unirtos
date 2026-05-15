/*****************************************************************/ /**
* @file unirtos_atcmd_websocket.h
* @brief
* @author harry.li@quectel.com
* @date 2024-04-25
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-04-25 <td>1.0 <td>harry.Li <td> Init
* </table>
**********************************************************************/

#ifndef __UNIRTOS_ATCMD_WEBSOCKET_H
#define __UNIRTOS_ATCMD_WEBSOCKET_H

#include "qosa_sys.h"
#include "qosa_at_param.h"

void qstd_exec_web_qwsopen_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_web_qwsclose_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_web_qwsread_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_web_qwswrite_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_web_qwscfg_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_web_qwstate_cmd_func(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_WEBSOCKET_H */
