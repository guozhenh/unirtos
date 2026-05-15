/*****************************************************************/ /**
* @file unirtos_atcmd_aepiot_lwm2m.h
* @brief
* @author larson.li@quectel.com
* @date 2024-03-15
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-03-15 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_ATCMD_AEPIOT_LWM2M_H__
#define __UNIRTOS_ATCMD_AEPIOT_LWM2M_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"

/**
 * @brief This command is used to configure IoT access parameters
 *
 * @note AT+QCFG="LWM2M/Lifetime" configures IoT platform registration lifetime
 * AT+QCFG="LWM2M/BindingMode" configures binding mode when accessing IoT platform
 */
void qstd_exec_aep_lwm2m_qcfg_cmd(qosa_at_cmd_t *cmd);
/**
 * @brief This command is used to configure IoT access parameters
 *
 * @note This command is used to set and query the IP address and port number of the CDP server
 */
void qstd_exec_aep_lwm2m_ncdp_cmd(qosa_at_cmd_t *cmd);
/**
 * @brief AT+QLWSREGIND - Registration Control
 *
 * @note This command is used to initiate registration and deregistration with the IoT platform
 */
void qstd_exec_aep_lwm2m_qlwsregind_cmd(qosa_at_cmd_t *cmd);
/**
 * @brief AT+QLWULDATA - Send Data
 *
 * @note This command is used to send data to the IoT platform using the LwM2M protocol
 */
void qstd_exec_aep_lwm2m_qlwuldata_cmd(qosa_at_cmd_t *cmd);
/**
 * @brief AT+QLWULDATAEX - Send CON/NON Messages
 *
 * @note This command is used to send confirmed (CON) or unconfirmed (NON) messages to the IoT platform using RAI identifier according to the LwM2M protocol
 */
void qstd_exec_aep_lwm2m_qlwuldataex_cmd(qosa_at_cmd_t *cmd);
/**
 * @brief AT+QLWULDATASTATUS - Query CON Message Send Status
 *
 * @note This command is used to query the status of CON data sent to the NB-IoT platform. This command only queries the status of already sent CON data
 */
void qstd_exec_aep_lwm2m_qlwuldatastatus_cmd(qosa_at_cmd_t *cmd);
/**
 * @brief AT+QLWFOTAIND - Set DFOTA Upgrade Mode
 *
 * @note This command is used to set the DFOTA upgrade mode
 */
void qstd_exec_aep_lwm2m_qlwfotaind_cmd(qosa_at_cmd_t *cmd);
/**
 * @brief AT+QREGSWT - Set Registration Mode
 *
 * @note This command is used to set the registration mode after restarting the module
 */
void qstd_exec_aep_lwm2m_qregswt_cmd(qosa_at_cmd_t *cmd);
/**
* @brief AT+NMGS - Send Message
*
 * @note This command is used to send data to the IoT platform
*/
void qstd_exec_aep_lwm2m_nmgs_cmd(qosa_at_cmd_t *cmd);
/**
* @brief AT+NMGR - Receive Message
*
*@note   This command is used to receive messages from the IoT platform via the LwM2M protocol in URC buffer mode, reading reported data from the cache; this command returns the first cached message and deletes it from the cache. If there are no cached messages, the command yields no response.
*/
void qstd_exec_aep_lwm2m_nmgr_cmd(qosa_at_cmd_t *cmd);
/**
* @brief AT+NNMI - Set New Message Indication
*
* @note This command is used to set or get the indication of new messages that have been sent
*/
void qstd_exec_aep_lwm2m_nnmi_cmd(qosa_at_cmd_t *cmd);
/**
* @brief AT+NSMI - Send Message Indication
*
* @note Use this command to set or get the indication for sent messages when sending an uplink message to the IoT platform.
*/
void qstd_exec_aep_lwm2m_nsmi_cmd(qosa_at_cmd_t *cmd);
/**
* @brief AT+NQMGR - Query Received Message Status
*
* @note This command is used to query the status of downlink messages received from the IoT platform
*/
void qstd_exec_aep_lwm2m_nqmgr_cmd(qosa_at_cmd_t *cmd);
/**
* @brief AT+NQMGS - Query Sent Message Status
*
* @note This command is used to query the status of uplink messages sent to the IoT platform.
*/
void qstd_exec_aep_lwm2m_nqmgs_cmd(qosa_at_cmd_t *cmd);
/**
* @brief AT+QLWEVTIND - LwM2M Event Reporting
*
* @note This command is used to configure the module to report LwM2M events to the device.
*/
void qstd_exec_aep_lwm2m_qlwevtind_cmd(qosa_at_cmd_t *cmd);
/**
* @brief AT+NMSTATUS - Message Registration Status
*
* @note This command is used to report the current registration status when the module is connected to the CDP server.
*/
void qstd_exec_aep_lwm2m_nmstatus_cmd(qosa_at_cmd_t *cmd);
/**
* @brief AT+QLWSERVERIP - Set/Remove Bootstrap/LwM2M Server IP
*
* @note This command is used to set and delete the IP addresses of the Bootstrap and IoT servers
*/
void qstd_exec_aep_lwm2m_qlwserverip_cmd(qosa_at_cmd_t *cmd);
/**
* @brief AT+QLWUPDATE - Update Heartbeat or Binding Mode
*
*/
void qstd_exec_aep_lwm2m_qlwaepreset_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief   AT+QLWRESET - Restore Factory Settings
 *
 */

/**
 * @brief lwm2m qlwxxx related AT command initialization
 */
void unir_aepiot_lwm2m_init(void);

#endif /* __UNIRTOS_ATCMD_AEPIOT_LWM2M_H__ */