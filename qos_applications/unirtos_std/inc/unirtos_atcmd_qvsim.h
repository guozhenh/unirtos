/*****************************************************************/ /**
* @file unirtos_atcmd_qvsim.h
* @brief
* @author larson.li@quectel.com
* @date 2023-05-25
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2023-05-25 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_QVSIM_H__
#define __UNIRTOS_ATCMD_QVSIM_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"
#include "unirtos_atcmd_general.h"
/*========================================================================
*  Type Definition
*========================================================================*/

void qstd_exec_qvsim_cmd(qosa_at_cmd_t *cmd);
void qstd_exec_qvsimw_cmd(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_QVSIM_H__ */
