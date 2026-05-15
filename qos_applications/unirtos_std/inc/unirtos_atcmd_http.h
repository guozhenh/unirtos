/*****************************************************************/ /**
* @file unirtos_atcmd_http.h
* @brief
* @author larson.li@quectel.com
* @date 2024-04-05
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-04-05 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_HTTP_H__
#define __UNIRTOS_ATCMD_HTTP_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"

void qstd_exec_http_qhttpcfg_cmd_func(qosa_at_cmd_t *cmd);
/* unir_http_at AT command function */
void qstd_exec_http_qhttpurl_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_http_qhttpget_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_http_qhttpgetex_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_http_qhttppost_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_http_qhttpptfile_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_http_qhttpput_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_http_qhttppatch_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_http_qhttpread_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_http_qhttpreadfile_cmd_func(qosa_at_cmd_t *cmd);

void qstd_exec_http_qhttpstop_cmd_func(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_HTTP_H__ */
