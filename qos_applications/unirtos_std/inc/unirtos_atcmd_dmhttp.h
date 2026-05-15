/*****************************************************************/ /**
* @file unirtos_atcmd_dmhttp.h
* @brief
* @author kruskal.zhu@quectel.com
* @date 2025-08
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2025-08 <td>1.0 <td>kruskal.zhu <td> Init
* </table>
**********************************************************************/

#ifndef __UNIRTOS_ATCMD_DMHTTP_H
#define __UNIRTOS_ATCMD_DMHTTP_H

#include "qosa_sys.h"
#include "qosa_at_param.h"

void qstd_exec_dmhttpcfg_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_dmhttpinfo_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_dmhttpreg_cmd_func(qosa_at_cmd_t *cmd);

/**
 * @brief  dmhttp power-on self-registration function
 *
 */
void unir_dmhttp_at_auto_reg(void);

#endif /* __UNIRTOS_ATCMD_DMHTTP_H */
