/*****************************************************************/ /**
* @file unirtos_atcmd_ftm.h
* @brief
* @author joe.tu@quectel.com
* @date 2025-09-02
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description
* <tr><td>2025-09-02 <td>1.0 <td>joe.tu <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_FTM_H__
#define __UNIRTOS_ATCMD_FTM_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"

void qstd_exec_qrxftm_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_qrftest_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_qrftestmode_cmd(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_FTM_H__ */
