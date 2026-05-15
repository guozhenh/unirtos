/*****************************************************************/ /**
* @file unirtos_atcmd_flash.h
* @brief
* @author felix.huang@quectel.com
* @date 2025-09-16
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2025-09-16 <td>1.0 <td>felix.huang <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_FLASH_H__
#define __UNIRTOS_ATCMD_FLASH_H__

#include "qosa_at_param.h"

/**
 * @brief QFTEST AT command processing function
 *
 * This function is used to parse and execute AT commands related to flash stress testing
 *
 * @param[in] cmd Pointer to the AT command structure, containing command parameters and context information
 *
 * @return None
 *
 * @see qosa_flash_stress_info_t
 */
void unir_exec_qftest_cmd(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_FILE_H__ */
