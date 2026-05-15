/*****************************************************************/ /**
* @file unirtos_atcmd_mipl_lwm2m.h
* @brief
* @author larson.li@quectel.com
* @date 2024-03-02
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-03-02 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_MIPL_LWM2M_H__
#define __UNIRTOS_ATCMD_MIPL_LWM2M_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"

/**
 * @brief AT+MIPLCONFIG OneNET Access Configuration
 *
 * @note This command is used to configure the bootstrap mode, bootstrap server address, access server address, automatic subscription response, authentication function, and write operation output format.
 *
 */
void qstd_exec_lwm2m_miplconfig_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief AT + MIPLCREATE Create a OneNET communication suite instance
 *
 * @note This command is used to create a OneNET communication suite instance (ONENET SDK)
 *
 * Internally, it does nothing but returns the globally customized clientid.
 */
void qstd_exec_lwm2m_miplcreate_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief AT+MIPLDELETE Delete OneNET Communication Suite Instance
 *
 * @note This command is used to delete a OneNET communication suite instance. (ONENET SDK)
 *
 */
void qstd_exec_lwm2m_mipldelete_cmd(qosa_at_cmd_t *cmd);

/**
* @brief  AT+MIPLVER Query OneNET SDK version
*
* @note
* Current OneNET SDK version, tentative format: V1.0.0
*/
void qstd_exec_lwm2m_miplver_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief  AT+MIPLADDOBJ Add LwM2M Object
 *
* @note This command is used to add an LwM2M object
*
 */
void qstd_exec_lwm2m_mipladdobj_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief  AT+MIPLDELOBJ Delete LwM2M Object
 *
 * @note This command is used to delete an LwM2M object
 *
 */
void qstd_exec_lwm2m_mipldelobj_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief  AT+MIPLOPEN Send registration request
 *
 * @note This command is used to send a registration request to the OneNET platform
 *
 */
void qstd_exec_lwm2m_miplopen_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief  AT+MIPLCLOSE Send deregistration request
 *
 * @note This command is used to send a deregistration request to the OneNET platform. The deregistration request is not retransmitted, meaning the platform's response does not affect the deregistration operation on the module side.
 *
 */
void qstd_exec_lwm2m_miplclose_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief  AT+MIPLOBSERVERSP
 */
void qstd_exec_lwm2m_mipldiscoverrsp_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief  AT+MIPLOBSERVERSP Response to subscription request
 *
 * @note   When the automatic response to subscription requests is disabled, this command is used to respond to subscription (Observe) requests from the OneNET platform or application server
 *
 */
void qstd_exec_lwm2m_miplobserversp_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief  AT+MIPLOBSERVERSP Response to subscription request
 *
 * @note  When the automatic response to subscription request feature is disabled, this command is used to respond to subscription (Observe) requests from the OneNET platform or application server
 *
 */
void qstd_exec_lwm2m_miplreadrsp_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief  AT+MIPLWRITERSP Response to Write Request
 *
 * @note This command is used to respond to a write request from the OneNET platform or application server
 *
 */
void qstd_exec_lwm2m_miplwritersp_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief  AT+MIPLEXECUTERSP Execute Request Response
 *
 * @note This command is used to respond to an Execute request from the OneNET platform or application server.
 *
 */
void qstd_exec_lwm2m_miplexecutersp_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief  AT+MIPLPARAMETERRSP Response to Write Attribute Request
 *
 * @note This command is used to respond to Write-Attributes requests from the OneNET platform or application server
 *
 */
void qstd_exec_lwm2m_miplparameterrsp_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief  AT+MIPLNOTIFY Report Data
 *
 * @note This command is used to report data to the OneNET platform or application server.
 *
 */
void qstd_exec_lwm2m_miplnotify_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief  AT+MIPLUPDATE Send Update Request
 *
 * @note This command is used to send an update request to update the device lifecycle and object list
 *
 */
void qstd_exec_lwm2m_miplupdate_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief   AT+MIPLRD Read cached data
 *
 * @note This command is used to read cached downlink data
 *
 */
void qstd_exec_lwm2m_miplrd_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief   AT+MIPLRESET Restore factory settings
 *
 */
void qstd_exec_lwm2m_miplreset_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief Initialization of AT commands related to lwm2m miplxxx
 */
void unir_onenet_miplwm2m_init(void);

#endif /* __UNIRTOS_ATCMD_MIPL_LWM2M_H__ */
