/*****************************************************************/ /**
* @file unirtos_app_app.h
* @brief
* @author harry.li@quectel.com
* @date 2024-5-10
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-5-10 <td>1.0 <td>harry.Li <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_PDP_APP_H__
#define __UNIRTOS_PDP_APP_H__

#include "qosa_def.h"
#include "qosa_sys.h"

#include "qosa_datacall.h"
#include "qosa_asyn_dns.h"

#include "qosa_at_sio.h"

/*
 * Enumeration type definition: qstd_pdp_msg_e
 * Used to define the type of PDP (Packet Data Protocol) messages.
 */
typedef enum
{
    /* PDP deactivation type: indicates the initiation of the PDP deactivation process */
    OSA_PDP_DEATTACH_TYPE = 0,
    /* PDP deactivation type: indicates initiating the PDP deactivation process */
    OSA_PDP_DEACTIVE_TYPE = 1,
} qstd_pdp_msg_e;

/**
 * @brief Defines a structure type qstd_pdp_add_t for PDP addition operations.
 *
 * The structure contains the following members:
 * - list: A linked list node used to add the PDP context to OSA's linked list for management.
 * - sim_id: SIM card identifier used to distinguish between different SIM cards.
 * - cid: Context ID
 * - dev_port: Port type, such as uart1 or usb.
 */
typedef struct
{
    qosa_q_link_type_t list;      // Linked list node
    qosa_uint8_t       sim_id;    // SIM card identifier
    qosa_uint8_t       cid;       // Connection identifier
    qosa_at_dev_type_e dev_port;  // Device type
} qstd_pdp_add_t;

/**
 * @brief Add an active PDP connection.
 *
 * This function is used to add a specified active PDP connection to the system.
 *
 * @param sim_id SIM card identifier, used to distinguish different SIM cards.
 * @param cid Connection identifier, used to distinguish different network connections.
 * @param dev_port Device type, indicating which type of device this PDN connection is for.
 */
void unirtos_pdp_add_active_pdp(qosa_uint8_t sim_id, qosa_uint8_t cid, qosa_at_dev_type_e dev_port);

/**
 * @brief Delete an active PDN connection
 *
 * This function is used to delete an active PDN connection based on the specified SIM identifier and call identifier.
 * The deletion operation is based on the network context, and it will disconnect all IP sessions associated with this PDN connection.
 *
 * @param sim_id SIM card identifier, used to identify a specific SIM card.
 * @param cid Call identifier, used to identify a specific PDN connection.
 */
void unirtos_pdp_delete_active_pdp(qosa_uint8_t sim_id, qosa_uint8_t cid);

#endif /* __UNIRTOS_PDP_APP_H__ */
