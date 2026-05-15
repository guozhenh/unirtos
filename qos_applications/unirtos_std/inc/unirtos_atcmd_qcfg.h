/*****************************************************************/ /**
* @file unirtos_atcmd_qcfg.h
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
#ifndef __UNIRTOS_ATCMD_QCFG_H__
#define __UNIRTOS_ATCMD_QCFG_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"

/*========================================================================
*  Type Definition
*========================================================================*/

typedef void (*unirtos_qcfg_process_func)(qosa_at_cmd_t *);

typedef struct
{
    char                     *pCmdName;
    unirtos_qcfg_process_func pCmdHandler;
    char                     *test_info;  //set test info as "", it will ouput at the cmd handler; set it as null, it will be ignored.
} unirtos_qcfg_cmd_list_t;

typedef unirtos_qcfg_cmd_list_t unir_cfg_cmdlist_s;

void qstd_exec_qcfg_cmd(qosa_at_cmd_t *cmd);

void unirtos_cfg_common_handler(qosa_at_cmd_t *cmd, unirtos_qcfg_cmd_list_t *cmdlist);
void qosa_at_qcfg_add_cust_at(const unirtos_qcfg_cmd_list_t *desc, qosa_uint32_t list_len);

#endif /* __UNIRTOS_ATCMD_QCFG_H__ */
