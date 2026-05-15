/*****************************************************************/ /**
* @file qcm_web_tcp.h
* @brief
* @author harry.li@quectel.com
* @date 2024-04-25
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-04-25 <td>1.0 <td>harry.Li <td> Init
* </table>
**********************************************************************/

#ifndef __QCM_WEB_TCP_H__
#define __QCM_WEB_TCP_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qcm_proj_config.h"
/**
 * @brief Open the WebSocket TCP connection.
 *
 * @param msg points to a pointer to a message structure containing connection information.
 * @return qcm_web_err_e returns the result status code of the operation, indicating success or the type of error.
 */
qcm_web_err_e qcm_ws_tcp_open(qcm_web_msg_t *msg);

/**
 * @brief Close the WebSocket TCP connection.
 *
 * @param msg points to a pointer to a message structure containing connection information.
 * @return qcm_web_err_e returns the result status code of the operation, indicating success or error type.
 */
qcm_web_err_e qcm_ws_tcp_close(qcm_web_msg_t *msg);

/**
 * @brief Confirmation function for closing WebSocket TCP connections.
 *
 * @param msg points to a pointer to a message structure containing connection information.
 */
void qcm_ws_tcp_close_cnf(qcm_web_msg_t *msg);

/**
 * @brief Send data via WebSocket.
 *
 * @param client_id Client ID, used to identify the target client for sending data.
 * @return int Returns the result status of the send operation, success or error code.
 */
int qcm_ws_socket_send(int client_id);

/**
 * Read data from the network and cache it into the watermark.
 *
 * @param client_id Client ID, used to identify a specific WebSocket client connection.
 *
 * This function is used to read data from the specified WebSocket client and perform corresponding processing based on the type of data received,
 * Including handling text, binary data, and responding to PING requests.
 */
void qcm_ws_socket_read(int client_id);

/**
 * @brief Handles WebSocket socket events.
 *
 * @param msg points to the pointer of the message structure containing socket event information.
 * @return qcm_web_err_e returns the result status code of the event processing, indicating success or the type of error.
 */
qcm_web_err_e qcm_ws_socket_evt(qcm_web_msg_t *msg);

/**
 * @brief WebSocket TCP open confirmation function.
 *
 * @param msg points to the pointer of the message structure containing connection information.
 */
void qcm_ws_tcp_open_cnf(qcm_web_msg_t *msg);

#ifdef CONFIG_QCM_VTLS_FUNC
/**
 * @brief Set up SSL session
 *
 * This function is used to initialize and set up an SSL session. It configures the relevant parameters of the SSL session based on the incoming message structure.
 *
 * @param msg points to a pointer of the qcm_web_msg_t structure, which contains message data related to SSL session setup.
 *            The specific data included depends on the application's requirements and implementation.
 *
 * Note: This function has no return value, but may affect subsequent operations by modifying the passed message structure.
 */
void qcm_ws_ssl_session_setup(qcm_web_msg_t *msg);
#endif

#endif /* __QCM_WEB_TCP_H__ */
