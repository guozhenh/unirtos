/*****************************************************************/ /**
* @file unirtos_atcmd_version.h
* @brief
* @author harry.li@quectel.com
* @date 2025-11-21
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2025-11-21 <td>1.0 <td>harry.Li <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_VERSION_H__
#define __UNIRTOS_ATCMD_VERSION_H__

#include "qosa_at_param.h"
#include "unirtos_atcmd_cfg.h"

/*========================================================================
*  Type Definition
*========================================================================*/

#ifdef CONFIG_QAPP_UNIRTOS_AT_QAPPVERSION_CMD
void qstd_exec_qappverinfo_cmd(qosa_at_cmd_t *cmd);
#endif /* CONFIG_QAPP_UNIRTOS_AT_QAPPVERSION_CMD */

#endif /* __UNIRTOS_ATCMD_VERSION_H__ */
