
/*****************************************************************/ /**
* @file unirtos_atcmd_mms.h
* @brief
* @author lawrence.liu@quectel.com
* @date 2024-03-06
*
* @copyright Copyright (c) 2024 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-03-06 <td>1.0 <td>lawrence.liu <td> Init
* </table>
**********************************************************************/
#ifndef _UNIRTOS_ATCMD_MMS_H
#define _UNIRTOS_ATCMD_MMS_H

#include "qosa_at_cmd.h"
#include "qcm_mms.h"

typedef enum
{
    UNIR_MMS_RESULT_OK = 0,
    UNIR_MMS_RESULT_UNKNOW_ERROR = 751,
    UNIR_MMS_RESULT_ERROR_URL_LENGTHERROR = 752,
    UNIR_MMS_RESULT_ERROR_URL_ERROR = 753,           /*failed to parse domain name*/
    UNIR_MMS_RESULT_ERROR_PROXYTYPE_NOSUPPORT = 754, /*failed to establish socket or the network is deactivated*/
    UNIR_MMS_RESULT_ERROR_PROXYADDR_ERROR = 755,
    UNIR_MMS_RESULT_ERROR_PARAM_ERROR = 756,
    UNIR_MMS_RESULT_ERROR_PARAM_TOADDR_FULL = 757,
    UNIR_MMS_RESULT_ERROR_PARAM_CCADDR_FULL = 758,
    UNIR_MMS_RESULT_ERROR_PARAM_BCCADDR_FULL = 759,
    UNIR_MMS_RESULT_ERROR_PARAM_BODYFILE_FULL = 760,
    UNIR_MMS_RESULT_ERROR_FILE = 761,
    UNIR_MMS_RESULT_ERROR_PARAM_EMTPY_TOADDR = 762,
    UNIR_MMS_RESULT_ERROR_FILE_NOT_FOUND = 763,
    UNIR_MMS_RESULT_ERROR_MMS_BUSY = 764,
    UNIR_MMS_RESULT_ERROR_HTTP_RESPONSE_FAILED = 765,
    UNIR_MMS_RESULT_ERROR_MMS_POST_RESPONS_INVALIDMSG = 766,
    UNIR_MMS_RESULT_ERROR_MMS_POST_RESPONSE_REPORTERROR = 767,
    UNIR_MMS_RESULT_ERROR_NETWORK_OPENFAILED = 768,
    UNIR_MMS_RESULT_ERROR_NETWORK_ERROR = 769,
    UNIR_MMS_RESULT_ERROR_SOC_CREATEFAILED = 770,
    UNIR_MMS_RESULT_ERROR_SOC_CONNECTFAILED = 771,
    UNIR_MMS_RESULT_ERROR_SOC_READFAILED = 772,
    UNIR_MMS_RESULT_ERROR_SOC_WRITEFAILED = 773,
    UNIR_MMS_RESULT_ERROR_SOC_CLOSE = 774,
    UNIR_MMS_RESULT_ERROR_TIMEOUT = 775,
    UNIR_MMS_RESULT_ERROR_ENCODE_DATAFAILED = 776,
    UNIR_MMS_RESULT_ERROR_HTTP_DECODEERROR = 777,
    /*
    UNIR_MMS_RESULT_ERROR_DECODE_DATAFAILED             = 778,
    UNIR_MMS_RESULT_ERROR_PDU_INPUTALIGNERROR           = 779,
    UNIR_MMS_RESULT_ERROR_PDU_INPUTCHARERROR            = 780,
    UNIR_MMS_RESULT_ERROR_MMS_NOTEXIST                  = 781,
    UNIR_MMS_RESULT_ERROR_INVALIDADDRESS                = 782,
    UNIR_MMS_RESULT_ERROR_VOICEBUSY                     = 783,
    UNIR_MMS_RESULT_ERROR_ALLOC_MEMORY                  = 784,
    UNIR_MMS_RESULT_ERROR_ENCODE_GETMMSLENTHFAILED      = 785,
    UNIR_MMS_RESULT_ERROR_SMS_DECODEERROR               = 786,
    UNIR_MMS_RESULT_ERROR_MMS_RECEIVEFULL               = 787,
    */
} unir_mms_error_code_e;

typedef unir_mms_error_code_e (*at_cfg_handle)(qosa_at_cmd_t *cmd);

typedef enum
{
    MMS_CFG_PDP_CID = 0,
    MMS_CFG_MMSC,
    MMS_CFG_PROXY,
    MMS_CFG_CHARSET,
    MMS_CFG_SEND_PARAM,
    MMS_CFG_SUPPORT_FIELD,
    MMS_CFG_CONNECT_TIMEOUT,
    MMS_CFG_MAX_PACKET_LEN,
    MMS_CFG_MAX
} unir_mms_cfg_e;

typedef enum
{
    DS_MMS_NONE_CMD = 0,
    DS_MMS_CFG_CMD,
    DS_MMS_EDIT_CMD,
    DS_MMS_SEND_CMD,
} unir_mms_cmd_e;

typedef struct
{
    char         *name;
    at_cfg_handle mms_config_func;
} unir_mms_config_t;

typedef struct
{
    qcm_mms_eercode_e     core_code;
    unir_mms_error_code_e app_code;
} unir_mms_code_table_t;

typedef struct
{
    qosa_at_dev_type_e dev_port;
    qcm_mms_common_cfg mms_user_config;
} unir_mms_mgr_t;

/**
 * @brief MMS CFG AT command, configures MMS parameters.
 *
 * @param[in] cmd
 */
void qstd_exec_mms_qmmscfg_cmd_func(qosa_at_cmd_t *cmd);

/**
 * @brief AT command for editing MMS information.
 *
 * @param[in] cmd
 */
void qstd_exec_mms_qmmsedit_cmd_func(qosa_at_cmd_t *cmd);

/**
 * @brief AT function for sending MMS messages.
 *
 * @param[in] cmd
 */
void qstd_exec_mms_qmmsend_cmd_func(qosa_at_cmd_t *cmd);

/**
 * @brief Initialization of MMS AT default parameters and MMS event callback functions.
 */
void unir_mms_at_init(void);

#endif /* _UNIRTOS_ATCMD_MMS_H */
