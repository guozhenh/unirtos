/*****************************************************************/ /**
* @file unirtos_atcmd_LT.c
* @brief
* @author david.deng@quectel.com
* @date 2026-1-28
*
* @copyright Copyright (c) 2025 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2026-1-28 <td>1.0 <td>david.deng <td> Init
* </table>
**********************************************************************/
#ifndef __UNIR_ATCMD_LT_H__
#define __UNIR_ATCMD_LT_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"

#ifdef CONFIG_QAPP_OEM_LT_AT_FUNC

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define UNIR_AVGRSSI_TIMEOUT    (1000*60) /*1 minute*/

/*========================================================================
*  Enumeration Definition
*========================================================================*/
typedef enum
{
    QNWSCAN_SET_BAND = 0,
    QNWSCAN_GET_OPERATOR = 1,
    QNWSCAN_RESTORE_BAND,
    QNWSCAN_DEINIT,
} qnwscan_task_cmd_e;

typedef enum
{
    SET_BAND_RSP = 0,
    GET_OPERATOR_RSP = 1,
    RESTORE_BAND_RSP = 2,
} qstd_qnwscan_resp_cmd_e;


/*========================================================================
*  Structure Definition
*========================================================================*/
typedef struct
{
    qosa_uint8_t simid;
    qosa_uint32_t sum_csq;
    qosa_uint32_t count;
    qosa_timer_t timer;
} unir_avgrssi_ctx_t;

typedef struct
{
    qosa_nw_band_t       band_cfg;
} unir_nw_qnwscan_reset_params_t;

typedef struct
{
    qnwscan_task_cmd_e    msgid; /*!< qnwscan message type */
    qosa_uint8_t            param1; /*! param1 */
    void                    *argv;  /*!< qnwscan message argument */
} qnwscan_cmd_t;


typedef struct
{
    qosa_uint32_t cmd_id; /*!< qstd_qnwscan_resp_cmd_e  */
    qosa_uint8_t simid; /*!< sim id */
    qosa_uint32_t err_code; /*!< error code, success is OSA_OK, failure is others, when this value indicates failure, application layer is not allowed to parse other parameters */
}qstd_qnwscan_at_msg;

typedef struct
{
    qosa_uint32_t err_code;
    qosa_uint8_t lte_band;
    qosa_at_dev_type_e dev_port;             /*!< AT channel */
    qosa_uint8_t        oper_num;                    /*!< Operator list count */
    qosa_nw_oper_list_t list[QOSA_NW_SUPP_OPER_NUM]; /*!< Operator list */
}qstd_qnwscan_ctx;

/*========================================================================
*  Global Variable
*========================================================================*/

/*========================================================================
*  Func Definition
*========================================================================*/
void unir_network_LT_init();

void unir_exec_qnwscan_cmd(qosa_at_cmd_t *cmd);
void unir_exec_qavgrssi_cmd(qosa_at_cmd_t *cmd);
#endif /* CONFIG_QAPP_OEM_LT_AT_FUNC */


#endif /* __UNIR_ATCMD_LT_H__ */
