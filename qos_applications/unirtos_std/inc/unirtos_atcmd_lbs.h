/*****************************************************************/ /**
* @file unirtos_atcmd_lbs.h
* @brief
* @author lambert.Zhao@quectel.com
* @date 2024-07-10
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-07-10 <td>1.0 <td>Lambert.Zhao <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_LBS_H__
#define __UNIRTOS_ATCMD_LBS_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"

void qsd_exec_lbs_qlbscfg_cmd_func(qosa_at_cmd_t *cmd);

void qsd_exec_lbs_qlbs_cmd_func(qosa_at_cmd_t *cmd);

void qsd_exec_lbs_qlbs_ext_input_cmd_func(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_LBS_H__ */
