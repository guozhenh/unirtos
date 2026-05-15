/*****************************************************************/ /**
* @file unirtos_atcmd_ping.h
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
#ifndef __UNIRTOS_ATCMD_PING_H__
#define __UNIRTOS_ATCMD_PING_H__

#include "qosa_at_param.h"

void qstd_exec_tcpip_qping_cmd(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_PING_H__ */
