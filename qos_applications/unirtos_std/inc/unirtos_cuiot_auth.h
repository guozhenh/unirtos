/*****************************************************************/ /**
* @file unirtos_cuiot_auth.h
* @brief
* @author lawrence.liu@quectel.com
* @date 2023-11-11
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description
* <tr><td>2023-11-11 <td>1.0 <td>lawrence.liu <td> Init
* </table>
**********************************************************************/
#ifndef __UNIRTOS_UNIFY_AUTH_H__
#define __UNIRTOS_UNIFY_AUTH_H__

#include "qcm_mqtt.h"
#include "unirtos_unify_common.h"
#include "qosa_log.h"

/**
 * @brief Represents different connection methods.
 */
/** Use one-device-one-secret authentication method: login. */
#define OSA_CUIOT_ONE_MACHINE_ONE_SECRE       0
/** Use one-type-one-secret authentication pre-registration method: activation. */
#define OSA_CUIOT_ONE_TYPE_ONE_DEN            1
/** Activate using the one-type-one-secret authentication method without pre-registration. */
#define OSA_CUIOT_ONE_TYPE_ONE_DEN_FREE       2
/** Use one-type-one-secret authentication without pre-registration method: login. */
#define OSA_CUIOT_ONE_TYPE_ONE_DEN_FREE_LOGIN -2
/** Use one-key-per-model login method. */
#define OSA_CUIOT_ONE_MODULE_ONE_DEN          3

typedef struct
{
    char clientid_str[OSA_UNIFY_CLIENTID_LEN_MAX];
    char username[OSA_UNIFY_USERNAME_LEN_MAX];
    char password[OSA_UNIFY_PASSWORD_LEN_MAX];
} unirtos_cuiot_mqtt_info;

typedef struct
{
    qosa_uint8_t             client_id;  //mqtt context id
    qosa_bool_t              is_tls_enable;
    int                      error_code;
    int                      _operator_t;
    unir_unify_auth_type_e   authType;
    int                      sim_id;
    int                      pdp_id;
    char                     Terminal_PK[OSA_UNIFY_PK_LEN_MAX];
    unir_unify_key_info_t    auth_param;
    char                    *topic[1];
    unirtos_cuiot_mqtt_info *mqtt_ptr;
} unir_cuiot_auth_info;

unirtos_cuiot_mqtt_info *qstd_cuiot_auth_get_secret(unir_cuiot_auth_info *auth_ptr);

#endif /* __UNIRTOS_UNIFY_AUTH_H__ */
