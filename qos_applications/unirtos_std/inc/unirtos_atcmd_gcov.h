/*****************************************************************/ /**
* @file unirtos_atcmd_gcov.h
* @brief
* @author John.ou@quectel.com
* @date 2026-04-07
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2026-04-07 <td>1.0 <td>John.ou <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_GCOV_H__
#define __UNIRTOS_ATCMD_GCOV_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_cmd.h"
#include "qcm_file_api.h"

void qstd_exec_qgcov_cmd(qosa_at_cmd_t *cmd);

#endif
