/*****************************************************************/ /**
* @file unirtos_atcmd_qindcfg.h
* @brief
* @author joe.tu@quectel.com
* @date 2024-07-23
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-07-23 <td>1.0 <td>joe.tu <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_QINTCFG_H__
#define __UNIRTOS_ATCMD_QINTCFG_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "unirtos_atcmd_cfg.h"
#include "qosa_at_param.h"
/*========================================================================
*  Type Definition
*========================================================================*/

typedef enum
{
    OSA_QINDCFG_URC_ALL_IND = 0,      //use to contol all urc report set true as default
    OSA_QINDCFG_URC_CSQ_IND,          // use to contol CSQ urc report
    OSA_QINDCFG_URC_ACT_IND,          //use to contol network mode change urc report
    OSA_QINDCFG_URC_SMSFULL_IND,      //use to control smsfull urc report
    OSA_QINDCFG_URC_SMSINCOMING_IND,  //use to control sms incoming urc report
    OSA_QINDCFG_URC_RING_IND,
    OSA_QINDCFG_URC_PSM_IND,          //use to psm sleep/wakeup urc report
#ifdef CONFIG_QAPP_UNIRTOS_AT_QIND_DATASTATUS_CMD
    OSA_QINDCFG_URC_DATASTATUS_IND,   /*!< use to control datastatus urc report, old feature */
#endif                                /* CONFIG_QAPP_UNIRTOS_AT_QIND_DATASTATUS_CMD */
#ifdef CONFIG_QAPP_UNIRTOS_AT_QIND_MODE_CMD
    OSA_QINDCFG_URC_MODE_IND,         //use to control network mode URC report, old feature
#endif                                /* CONFIG_QAPP_UNIRTOS_AT_QIND_MODE_CMD */
    OSA_QINDCFG_URC_MAX_IND,
} qingcfg_urc_ind_type_t;

void qstd_exec_qindcfg_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief Read URC report switch status
 * @note When reading parameters of a type other than OSA_QINDCFG_URC_ALL_IND, the returned result will be evaluated together with the status of OSA_QINDCFG_URC_ALL_IND
 *
 * @param[in] flag
 *          - Specified URC report type
 * @return qosa_bool_t Returns OSA_TRUE if reporting is allowed, returns OSA_FALSE if reporting is prohibited
 */
qosa_bool_t qosa_read_urc_ind_flag(qingcfg_urc_ind_type_t flag);

#endif /* __UNIRTOS_ATCMD_QINTCFG_H__ */
