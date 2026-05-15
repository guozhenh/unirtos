/*****************************************************************/ /**
* @file unirtos_atcmd_ntp.h
* @brief
* @author larson.li@quectel.com
* @date 2024-04-22
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-04-22 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_NTP_H__
#define __UNIRTOS_ATCMD_NTP_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"

void qstd_exec_tcpip_qntp_cmd(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_NTP_H__ */
