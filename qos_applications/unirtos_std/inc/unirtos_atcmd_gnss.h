/*****************************************************************//**
* @file unirtos_atcmd_gnss.h
* @brief 
* @author larson.li@quectel.com
* @date 2025-12-18
* 
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd. 
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
* 
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2025-12-18 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_GNSS_H__
#define __UNIRTOS_ATCMD_GNSS_H__

#include "qosa_def.h"
#include "qosa_sys.h"


void qstd_exec_qgpscfg_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_qgpsdel_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_qgps_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_qgpsend_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_qgpsloc_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_qgpscmdsend_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_qagps_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_qagpscfg_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_qgpsgnmea_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_qgpsvbckp_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_qgpsinfo_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_qgpsdebug_cmd_func(qosa_at_cmd_t *cmd);

/**
 * @brief  GNSS power-on self-start function
 *
 */
void qstd_gnss_at_autogps(void);

#endif /* __UNIRTOS_ATCMD_GNSS_H__ */
