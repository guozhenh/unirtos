/*****************************************************************/ /**
* @file unirtos_atcmd_coap.h
* @brief
* @author larson.li@quectel.com
* @date 2023-10-14
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2023-10-14 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/

#ifndef __UNIRTOS_ATCMD_COAP_H__
#define __UNIRTOS_ATCMD_COAP_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_at_param.h"

#include "coap_13.h"

#define QCOAP_PACKET_STRING_OPTION_TO_URC(number, field)                                                                                                       \
    if (IS_OPTION(coap_pkt, number))                                                                                                                           \
    {                                                                                                                                                          \
        if (coap_pkt->field##_len > 0)                                                                                                                         \
        {                                                                                                                                                      \
            rc = qosa_snprintf(buffer + length, buffer_len - length, ",%d,\"%s\"", number, coap_pkt->field);                                                   \
        }                                                                                                                                                      \
        else                                                                                                                                                   \
        {                                                                                                                                                      \
            rc = qosa_snprintf(buffer + length, buffer_len - length, ",%d,\"\"", number);                                                                      \
        }                                                                                                                                                      \
        if (-1 == rc)                                                                                                                                          \
        {                                                                                                                                                      \
            return 0;                                                                                                                                          \
        }                                                                                                                                                      \
        length += rc;                                                                                                                                          \
        (*opt_cnt)++;                                                                                                                                          \
    }

#define QCOAP_PACKET_BYTE_OPTION_TO_URC(number, field)                                                                                                         \
    if (IS_OPTION(coap_pkt, number))                                                                                                                           \
    {                                                                                                                                                          \
        if (coap_pkt->field##_len > 0)                                                                                                                         \
        {                                                                                                                                                      \
            int index = 0;                                                                                                                                     \
            rc = qosa_snprintf(buffer + length, buffer_len - length, ",%d,\"", number);                                                                        \
            if (-1 == rc)                                                                                                                                      \
            {                                                                                                                                                  \
                return 0;                                                                                                                                      \
            }                                                                                                                                                  \
            length += rc;                                                                                                                                      \
            for (index = 0; index < coap_pkt->field##_len; index++)                                                                                            \
            {                                                                                                                                                  \
                rc = qosa_snprintf(buffer + length, buffer_len - length, "%02X", coap_pkt->field[index]);                                                      \
                if (-1 == rc)                                                                                                                                  \
                {                                                                                                                                              \
                    return 0;                                                                                                                                  \
                }                                                                                                                                              \
                length += rc;                                                                                                                                  \
            }                                                                                                                                                  \
            rc = qosa_snprintf(buffer + length, buffer_len - length, "\"");                                                                                    \
        }                                                                                                                                                      \
        else                                                                                                                                                   \
        {                                                                                                                                                      \
            rc = qosa_snprintf(buffer + length, buffer_len - length, ",%d,\"\"", number);                                                                      \
        }                                                                                                                                                      \
        if (-1 == rc)                                                                                                                                          \
        {                                                                                                                                                      \
            return 0;                                                                                                                                          \
        }                                                                                                                                                      \
        length += rc;                                                                                                                                          \
        (*opt_cnt)++;                                                                                                                                          \
    }

#define QCOAP_PACKET_INT_OPTION_TO_URC(number, field)                                                                                                          \
    if (IS_OPTION(coap_pkt, number))                                                                                                                           \
    {                                                                                                                                                          \
        rc = qosa_snprintf(buffer + length, buffer_len - length, ",%d,\"%d\"", number, coap_pkt->field);                                                       \
        if (-1 == rc)                                                                                                                                          \
        {                                                                                                                                                      \
            return 0;                                                                                                                                          \
        }                                                                                                                                                      \
        length += rc;                                                                                                                                          \
        (*opt_cnt)++;                                                                                                                                          \
    }

#define QCOAP_PACKET_ACCEPT_OPTION_TO_URC(number, field)                                                                                                       \
    if (IS_OPTION(coap_pkt, number))                                                                                                                           \
    {                                                                                                                                                          \
        qosa_uint8_t index = 0;                                                                                                                                \
        for (index = 0; index < coap_pkt->field##_num; index++)                                                                                                \
        {                                                                                                                                                      \
            rc = qosa_snprintf(buffer + length, buffer_len - length, ",%d,\"%d\"", number, coap_pkt->field[index]);                                            \
            if (-1 == rc)                                                                                                                                      \
            {                                                                                                                                                  \
                return 0;                                                                                                                                      \
            }                                                                                                                                                  \
            length += rc;                                                                                                                                      \
            (*opt_cnt)++;                                                                                                                                      \
        }                                                                                                                                                      \
    }

#define QCOAP_PACKET_MULTI_OPTION_TO_URC(number, field)                                                                                                        \
    if (IS_OPTION(coap_pkt, number))                                                                                                                           \
    {                                                                                                                                                          \
        multi_option_t *optP;                                                                                                                                  \
        for (optP = coap_pkt->field; optP != QOSA_NULL; optP = optP->next)                                                                                     \
        {                                                                                                                                                      \
            if (optP->len > 0)                                                                                                                                 \
            {                                                                                                                                                  \
                char *temp = qosa_malloc(optP->len + 1);                                                                                                       \
                qosa_memset(temp, 0x00, optP->len + 1);                                                                                                        \
                if (QOSA_NULL == temp)                                                                                                                         \
                {                                                                                                                                              \
                    return 0;                                                                                                                                  \
                }                                                                                                                                              \
                qosa_memcpy(temp, optP->data, optP->len);                                                                                                      \
                rc = qosa_snprintf(buffer + length, buffer_len - length, ",%d,\"%s\"", number, temp);                                                          \
                qosa_free(temp);                                                                                                                               \
            }                                                                                                                                                  \
            else                                                                                                                                               \
            {                                                                                                                                                  \
                rc = qosa_snprintf(buffer + length, buffer_len - length, ",%d,\"\"", number);                                                                  \
            }                                                                                                                                                  \
            if (-1 == rc)                                                                                                                                      \
            {                                                                                                                                                  \
                return 0;                                                                                                                                      \
            }                                                                                                                                                  \
            length += rc;                                                                                                                                      \
            (*opt_cnt)++;                                                                                                                                      \
        }                                                                                                                                                      \
    }

#define QCOAP_PACKET_BLOCK_OPTION_TO_URC(number, field)                                                                                                        \
    if (IS_OPTION(coap_pkt, number))                                                                                                                           \
    {                                                                                                                                                          \
        qosa_uint32_t block = coap_pkt->field##_num << 4;                                                                                                      \
        if (coap_pkt->field##_more)                                                                                                                            \
        {                                                                                                                                                      \
            block |= 0x8;                                                                                                                                      \
        }                                                                                                                                                      \
        block |= 0xF & qcm_coap_log_2(coap_pkt->field##_size / 16);                                                                                            \
        rc = qosa_snprintf(buffer + length, buffer_len - length, ",%d,\"%d\"", number, block);                                                                 \
        if (-1 == rc)                                                                                                                                          \
        {                                                                                                                                                      \
            return 0;                                                                                                                                          \
        }                                                                                                                                                      \
        length += rc;                                                                                                                                          \
        (*opt_cnt)++;                                                                                                                                          \
    }

void qstd_exec_coap_qcoapcfg_cmd(qosa_at_cmd_t *cmd);

void qstd_exec_coap_qcoapopen_cmd(qosa_at_cmd_t *cmd);

void qstd_exec_coap_qcoapclose_cmd(qosa_at_cmd_t *cmd);

void qstd_exec_coap_qcoapoption_cmd(qosa_at_cmd_t *cmd);

void qstd_exec_coap_qcoaphead_cmd(qosa_at_cmd_t *cmd);

void qstd_exec_coap_qcoapsend_cmd(qosa_at_cmd_t *cmd);

#endif
