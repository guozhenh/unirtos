/*****************************************************************/ /**
* @file unirtos_modem_at.h
* @brief
* @author Joe.tu@quectel.com
* @date 2024-03-18
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description
* <tr><td>2024-04-16 <td>1.0 <td>Joe.tu <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_MODEM_AT_H__
#define __UNIRTOS_MODEM_AT_H__

#include "qosa_def.h"
#include "qosa_at_sio.h"
#include "qosa_at_cmd.h"
#include "qosa_at_param.h"
#include "unirtos_atcmd_cfg.h"

// attention!!!
// This header file will redefine the qstd_exec_at_resp series macros
#ifdef CONFIG_QAPP_UNIRTOS_MODEM_AT_BYPASS_FUNC
#ifdef qosa_at_resp_error
#undef qosa_at_resp_error
#endif

#ifdef qosa_at_resp_ok
#undef qosa_at_resp_ok
#endif

#ifdef qosa_at_resp_cms_error
#undef qosa_at_resp_cms_error
#endif

#ifdef qosa_at_resp_cme_error
#undef qosa_at_resp_cme_error
#endif

#ifdef qosa_at_resp_text
#undef qosa_at_resp_text
#endif

#ifdef qosa_at_resp_ntext
#undef qosa_at_resp_ntext
#endif

#define qosa_at_resp_error(dev_port)                     qstd_modem_at_resp_cmd(dev_port, QOSA_ATCI_RESULT_CODE_ERROR, QOSA_CMD_RC_ERROR, QOSA_NULL, 1);
#define qosa_at_resp_ok(dev_port)                        qstd_modem_at_resp_cmd(dev_port, QOSA_ATCI_RESULT_CODE_OK, QOSA_CMD_RC_OK, QOSA_NULL, 1);
#define qosa_at_resp_cms_error(dev_port, errcode)        qstd_modem_at_resp_cmd(dev_port, QOSA_ATCI_RESULT_CODE_CMS_ERROR, errcode, QOSA_NULL, 1);
#define qosa_at_resp_cme_error(dev_port, errcode)        qstd_modem_at_resp_cmd(dev_port, QOSA_ATCI_RESULT_CODE_CME_ERROR, errcode, QOSA_NULL, 1);
#define qosa_at_resp_text(dev_port, text)                qstd_modem_at_resp_cmd(dev_port, QOSA_ATCI_RESULT_CODE_OK, QOSA_CMD_RC_OK, text, 1);
#define qosa_at_resp_ntext(dev_port, text, len)          qstd_modem_at_resp_text(dev_port, text, len, 1)
#define qosa_at_resp_ntext_directly(dev_port, text, len) qosa_at_resp_cmd_info_text(dev_port, text, len, 1)
#define qosa_at_resp_ok_directly(dev_port)               qosa_at_resp_cmd(dev_port, QOSA_ATCI_RESULT_CODE_OK, QOSA_CMD_RC_OK, QOSA_NULL, 0);

void qstd_modem_at_resp_cmd(qosa_at_dev_type_e dev_port, qosa_atci_result_code_e resultCode, qosa_uint32_t report_code, char *rsp_buffer, qosa_uint8_t padding);
void qstd_modem_at_resp_text(qosa_at_dev_type_e dev_port, const char *text, qosa_size_t length, qosa_uint8_t padding);
void _qstd_modem_at_push(qosa_at_cmd_t *cmd, void (*func)(struct _qosa_at_cmd *));
void mdoem_at_cfg_common_handler(qosa_at_cmd_t *cmd, void *_cmd_lsit);
void qstd_modem_at_bypass_init(void);
#endif

#if defined(CONFIG_QAPP_UNIRTOS_MODEM_AT_BYPASS_FUNC) && !defined(CONFIG_QAPP_UNIRTOS_MODEM_AT_BYPASS_ERRCODE_ONLY_FUNC)
#define qstd_modem_at_push(cmd, func) _qstd_modem_at_push(cmd, func)
#else
#define qstd_modem_at_push(cmd, func) func(cmd)
#endif

#endif /* __UNIRTOS_MODEM_AT_H__ */
