/*********************************************************************
 * @file   qcm_uart_log.h
 * @brief
 * @author neo.jiang@quectel.com
 * @date   2026-04-23
 *
 * @copyright  Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
 * All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
 *
 * @par EDIT HISTORY FOR MODULE
 * <table>
 * <tr><th>Date       <th>Version    <th>Author          <th>Description"
 * <tr><td>2026-04-23 <td>1.0        <td>neo.jiang       <td> Init
 * </table>
 **********************************************************************/

#ifndef ___QCM_UART_LOG_H__
#define ___QCM_UART_LOG_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_gpio.h"

#define QCM_UART_LOG_PORT                       QOSA_UART_PORT_1
#define QCM_UART_LOG_TX_PIN                     QCM_UART1_TX_PIN
#define QCM_UART_LOG_RX_PIN                     QCM_UART1_RX_PIN
#define QCM_UART_LOG_CTS_PIN                    QCM_UART1_CTS_PIN
#define QCM_UART_LOG_RTS_PIN                    QCM_UART1_RTS_PIN
#define QCM_UART_LOG_PIN_FUNC                   QCM_UART1_PIN_FUNC
#define QCM_UART_LOGFLOW_CTRL_PIN_FUNC          QCM_UART1_FLOW_CTRL_FUNC

#define QCM_UART1_TX_PIN                        (18)
#define QCM_UART1_RX_PIN                        (17)
#define QCM_UART1_PIN_FUNC                      (1)
#define QCM_UART1_CTS_PIN                       (16)
#define QCM_UART1_RTS_PIN                       (17)
#define QCM_UART1_FLOW_CTRL_FUNC                (3)

#define QCM_UART_LOG_PRINTF_MAX_LEN             256

/**
 * @brief Print log using UART
 *
 * @param[in] level
 *          - log level
 * @param[in] tag
 *          - log tag
 * @param[in] funcname
 *          - Application layer SIM ID, always 0 in dual-SIM single standby mode
 * @param[in] line
 *          - the line number of the file containing the log
 * @param[in] fmt
 *          - fmort control string
 */
 void qcm_uart_log_printf(int level, int tag, const char* funcname, int line, const char* fmt, ...);

#endif /* ___QCM_UART_LOG_H__ */
