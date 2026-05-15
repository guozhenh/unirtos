/*****************************************************************/ /**
* @file qosa_conv.h
* @brief
* @author joe.tu@quectel.com
* @date 2025-06-30
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2025-06-30 <td>1.0 <td>Joe.tu <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_CONV_H__
#define __QOSA_CONV_H__

#include "qosa_def.h"
#include "qosa_sys.h"

int qosa_conv(qosa_uint8_t from_chset, qosa_uint8_t *inbuf, qosa_size_t inbytes, qosa_uint8_t to_chset, qosa_uint8_t *outbuf, qosa_size_t *outbytes);
int qosa_conv_utf8_to_ucs2_character(const char* utf8_txt, char* ucs2_character, qosa_size_t buffer_size);

#endif /* __QOSA_CONV_H__ */
