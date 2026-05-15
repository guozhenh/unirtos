/*****************************************************************/ /**
* @file qcm_web_http.h
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
#ifndef __QCM_WEB_HTTP_H__
#define __QCM_WEB_HTTP_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qcm_websocket.h"
#include "qcm_web_internal.h"

/**
 * @brief Send an HTTP request to the server
 *
 * @param msg pointer to the structure containing the request information
 * @return qcm_web_err_e returns the status of the requested operation, indicating the reason for success or failure.
 */
qcm_web_err_e qcm_ws_http_request(qcm_web_msg_t *msg);

/**
 * @brief Process the HTTP response returned by the server
 *
 * @param msg pointer to the structure containing the response information
 * @return qcm_web_err_e returns the status of the processing operation, indicating the reason for success or failure.
 */
qcm_web_err_e qcm_ws_http_response(qcm_web_msg_t *msg);

/**
 * @brief Generate a random number of specified length and store it in the buffer
 *
 * @param buf pointer to the buffer for storing random numbers
 * @param len The length of the random number to generate
 * @return int returns the actual length of the generated random number, which may be less than the requested length due to system errors
 */
int qcm_ws_get_random(void *buf, int len);

#endif /* __QCM_WEB_HTTP_H__ */
