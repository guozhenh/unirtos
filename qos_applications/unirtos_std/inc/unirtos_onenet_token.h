/*****************************************************************/ /**
* @file unirtos_onenet_token.h
* @brief
* @author lawrence.liu@om
* @date 2023-12-11
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description
* <tr><td>2023-12-11 <td>1.0 <td>lawrence.liu <td> Init
* </table>
**********************************************************************/
#ifndef _UNIRTOS_ONENET_TOKEN_H
#define _UNIRTOS_ONENET_TOKEN_H

char *
unir_onenet_generate_auth_token(signed long long expire_time, const char *pid, const char *device_name, const char *version, const char *device_access_key);

#endif /* _UNIRTOS_ONENET_TOKEN_H */
