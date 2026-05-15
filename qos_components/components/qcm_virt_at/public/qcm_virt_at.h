/*****************************************************************/ /**
* @file qcm_virt_at.h
* @brief
* @author kruskal.zhu@quectel.com
* @date   2025-09
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date    <th>Version <th>Author       <th>Description"
* <tr><td>2025-09 <td>1.0     <td>kruskal.zhu  <td> Init
* </table>
**********************************************************************/

#ifndef __QCM_VIRT_AT_H__
#define __QCM_VIRT_AT_H__

/**
 * @brief qcm_virt_at_err_e Enumeration, which defines the error type that VIRT_AT function returns
 *
 */
typedef enum
{
    QCM_VIRT_AT_ERR_OK = 0,         /*!< The operation was successful */
    QCM_VIRT_AT_ERR_UNKNOW = 1,     /*!< Unknown error */
    QCM_VIRT_AT_ERR_INVAL_PARM = 2, /*!< Invalid parameter error */
} qcm_virt_at_err_e;

/**
 * @brief  User-defined callback functions
 *         This callback function is used to return the virtual AT execution response and the corresponding URC notification
 *
 * @param  data  The virtual AT executes the response along with the corresponding URC notification, which is returned via a string
 * @param  size  The length of the response result string
 *
 */
typedef void (*qcm_virt_at_result_cb)(const char *data, qosa_uint32_t size);

/**
 * @brief  Initialize the virtual AT and set up the user's callback function
 *
 * @param  virt_at_cb  User-defined callback functions
 *
 * @return Function returns an error code indicating success or failure of operation. (Returns QCM_VIRT_AT_ERR_OK on success, other error codes on failure).
 *
 */
qcm_virt_at_err_e qcm_virt_at_init(qcm_virt_at_result_cb virt_at_cb);

/**
 * @brief  Used to send virtual AT instructions
 *
 * @param  data      Virtual AT instruction string
 * @param  data_len  Virtual AT instruction string length
 *
 * @return Function returns an error code indicating success or failure of operation. (Returns QCM_VIRT_AT_ERR_OK on success, other error codes on failure).
 *
 */
qcm_virt_at_err_e qcm_virt_at_send(char *data, int data_len);

#endif /* __QCM_VIRT_AT_H__ */
