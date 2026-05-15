/*****************************************************************/ /**
* @file qcm_utf8.h
* @brief
* @author larson.li@quectel.com
* @date 2023-06-11
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2023-06-11 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_UTF8_H__
#define __QOSA_UTF8_H__

#include "qosa_def.h"
#include "qosa_sys.h"

/**
 * @brief Validate a null-terminated string has only UTF-8 characters
 *
 * @param const char *string
 *           - the string to check for valid UTF-8 characters
 *
 * @return int
 *          - 1 (true) if the string has only UTF-8 characters, 0 (false) otherwise
 */
extern int qcm_utf8_validate_string(const char *string);

#endif /* __QOSA_UTF8_H__ */
