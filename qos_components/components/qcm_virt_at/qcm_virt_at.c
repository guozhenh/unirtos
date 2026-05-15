/*****************************************************************/ /**
* @file   qcm_virt_at.c
* @brief  Virtual AT component layer function implementation
* @author kruskal.zhu@quectel.com
* @date   2025-09
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date    <th>Version <th>Author      <th>Description"
* <tr><td>2025-09 <td>1.0     <td>kruskal.zhu <td> Init
* </table>
**********************************************************************/

#include "qosa_sys.h"
#include "qosa_sys_registers.h"
#include "qosa_at_sio.h"

#include "qcm_virt_at.h"

qcm_virt_at_err_e qcm_virt_at_init(qcm_virt_at_result_cb virt_at_cb)
{
    if (QOSA_NULL == virt_at_cb)
    {
        return QCM_VIRT_AT_ERR_INVAL_PARM;
    }

    //Register the callback function for reporting virtual AT execution results, located at the system layer, used to adapt to the actual callback registration on different platforms.
    qosa_regedit_qcm_virt_at_result_func(virt_at_cb);

    return QCM_VIRT_AT_ERR_OK;
}

qcm_virt_at_err_e qcm_virt_at_send(char *data, int data_len)
{
    int ret = 0;

    //Virtual AT send function, located at the system layer, used to adapt the actual AT send functions across different platforms.
    ret = qosa_virt_at_send(data, data_len);
    if (ret != 0)
    {
        return QCM_VIRT_AT_ERR_UNKNOW;
    }

    return QCM_VIRT_AT_ERR_OK;
}
