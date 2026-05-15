/*****************************************************************/ /**
* @file unirtos_atcmd_wifiscan.h
* @brief
* @author David.Deng@quectel.com
* @date 2025-07-08
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description
* <tr><td>2025-07-08 <td>1.0 <td>David.Deng <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_WIFISCAN_H__
#define __UNIRTOS_ATCMD_WIFISCAN_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"
#include "qosa_at_config.h"

void qstd_exec_qwifiscan_cmd(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_WIFISCAN_H__ */
