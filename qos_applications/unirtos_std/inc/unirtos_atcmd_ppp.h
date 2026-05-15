/*****************************************************************/ /**
* @file unirtos_atcmd_ppp.h
* @brief PPP AT commands implementation
* @author joe.tu@quectel.com
* @date 2025-01-13
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description
* <tr><td>2025-01-13 <td>1.0 <td>joe.tu <td> Init
* </table>
**********************************************************************/
#ifndef UNIRTOS_ATCMD_PPP_H
#define UNIRTOS_ATCMD_PPP_H

#include "qosa_at_cmd.h"

/**
 * @brief AT+QPPPDROP command handler (qstd_exec prefix)
 * @param cmd AT command structure
 * @return void
 */
void qstd_exec_qpppdrop_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief Initialize PPP AT command module
 * @return void
 */
void unirtos_ppp_at_init(void);

#endif /* UNIRTOS_ATCMD_PPP_H */
