/*****************************************************************/ /**
* @file unirtos_atcmd_lpm.h
* @brief
* @author harry.li@quectel.com
* @date 2025-05-14
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2025-05-14 <td>1.0 <td>harry.Li <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_LPM_H__
#define __UNIRTOS_ATCMD_LPM_H__

#include "qosa_at_param.h"

/*========================================================================
*  Type Definition
*========================================================================*/

void qstd_exec_qsclk_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_qsclkex_cmd(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_LPM_H__ */
