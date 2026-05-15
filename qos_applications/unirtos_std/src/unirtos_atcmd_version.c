/*****************************************************************/ /**
* @file unirtos_atcmd_tcpip.c
* @brief
* @author larson.li@quectel.com
* @date 2023-10-14
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description
* <tr><td>2023-10-14 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_sio.h"
#include "qosa_log.h"
#include "qosa_at_cmd.h"
#include "unirtos_atcmd_cfg.h"
#ifdef CONFIG_QAPP_UNIRTOS_AT_QAPPVERSION_CMD
#include "unirtos_version.h"
#endif /* CONFIG_QAPP_UNIRTOS_AT_QAPPVERSION_CMD */

#define QOS_LOG_TAG LOG_TAG_AT

#ifdef CONFIG_QAPP_UNIRTOS_AT_QAPPVERSION_CMD
extern const char unirtos_mob_sw_rev[];
extern const char unirtos_ver_date[];
extern const char unirtos_ver_time[];
extern const char unirtos_authors[];
#endif /* CONFIG_QAPP_UNIRTOS_AT_QAPPVERSION_CMD */

#ifdef CONFIG_QAPP_UNIRTOS_AT_QAPPVERSION_CMD
void qstd_exec_qappverinfo_cmd(qosa_at_cmd_t *cmd)
{
    char res_buf[256] = {0};

    if (cmd->type == QOSA_AT_CMD_EXE)
    {
        qosa_snprintf(res_buf, 256, "QAPPVERSION: %s\r\n%s %s\r\n%s", unirtos_mob_sw_rev, unirtos_ver_date, unirtos_ver_time, unirtos_authors);
        qosa_at_resp_cmd(cmd->dev_port, QOSA_ATCI_RESULT_CODE_OK, QOSA_CMD_RC_OK, res_buf, 1);
    }
    else if (cmd->type == QOSA_AT_CMD_TEST)
    {
        QOSA_RETURN_OK(cmd->dev_port);
    }
    else
    {
        QOSA_RETURN_ERROR(cmd->dev_port);
    }
}
#endif /* CONFIG_QAPP_UNIRTOS_AT_QAPPVERSION_CMD */
