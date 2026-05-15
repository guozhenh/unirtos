/*****************************************************************/ /**
* @file unirtos_at_nvm_cfg.h
* @brief
* @author larson.li@quectel.com
* @date 2024-09-02
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-09-02 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_AT_NVM_CFG_H__
#define __UNIRTOS_AT_NVM_CFG_H__

#include "qosa_def.h"
#include "qosa_sys.h"

#include "qosa_nvitem.h"

// clang-format off

//json nv file name
#define UNIRTOS_AT_JSON_FILE             "atcmd_cfg.json"

//json config root node
#define UNIRTOS_CFG_JSON_ROOT_NODE_NAME  UNIRTOS_AT_JSON_FILE"/atcmd_config"

// AT command lbs+wifiscan config
#define UNIRTOS_AT_LBS_CFG               UNIRTOS_CFG_JSON_ROOT_NODE_NAME"/lbs_cfg"
#define UNIRTOS_AT_QINDCFG_CFG           UNIRTOS_CFG_JSON_ROOT_NODE_NAME"/qindcfg_at_cfg"

// AT command ntp config
#define UNIRTOS_AT_NTP_CFG               UNIRTOS_CFG_JSON_ROOT_NODE_NAME"/ntp_cfg"


/** Stores the information of the LianTong YanFei device. */
/** Store China Unicom Yanfei device information. */
#define UNIRTOS_AT_UNIFY_CFG             UNIRTOS_CFG_JSON_ROOT_NODE_NAME"/unify_cfg"
/** Stores the Unicom Yanfei One-Module-One-Secret information. */
/** Store China Unicom Yanfei one-module-one-secret information. */
#define UNIRTOS_AT_UNIFY_MODULE_CFG      UNIRTOS_CFG_JSON_ROOT_NODE_NAME"/unify_module_cfg"
/** Stores the device shadow information for Lianyanfei self-registration. */
/** Store China Unicom Yanfei self-registration device shadow information. */
#define UNIRTOS_AT_UNIFY_DEVICESHADOW_CFG    UNIRTOS_CFG_JSON_ROOT_NODE_NAME"/unify_deviceshadow_cfg"

// AT command qdbgcfg config
#define UNIRTOS_AT_QDBGCFG_CFG           UNIRTOS_CFG_JSON_ROOT_NODE_NAME"/qdbgcfg_cfg"

// AT command qsvn config
#define UNIRTOS_AT_QSVN_CFG              UNIRTOS_CFG_JSON_ROOT_NODE_NAME"/qsvn_cfg"

// clang-format on

#endif /* __UNIRTOS_AT_NVM_CFG_H__ */
