/*****************************************************************/ /**
* @file qcm_uart_log.c
* @brief
* @author neo.jiang@quectel.com
* @date 2026-04-23
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2026-04-23 <td>1.0 <td>neo.jiang <td> Init
* </table>
**********************************************************************/

#include "qcm_proj_config.h"
#include "qcm_uart_log.h"
#include "qosa_sys.h"
#include "qosa_def.h"
#include "qosa_dev.h"
#include "qosa_uart.h"
#include "qosa_pinctrl.h"
#include "qosa_rtc.h"

static char g_uart_printf_buf[QCM_UART_LOG_PRINTF_MAX_LEN]; // log print buffer

/**
 * @brief UART initialization
 */
void qcm_log_uart_port_init(void)
{
    static qosa_uint8_t uart_log_init_flag = 0;
    qosa_uart_config_t dcb_config = {0};
    //qosa_uart_status_monitor_t monitor = {0};

    if(uart_log_init_flag == 0)
    {
        /* Register UART event callback */
        qosa_uart_register_cb(QCM_UART_LOG_PORT, QOSA_NULL);

        /* Configure UART communication parameters: baud rate, data bits, stop bits, parity bit, flow control */
        dcb_config.baudrate = QOSA_UART_BAUD_115200;
        dcb_config.data_bit = QOSA_UART_DATABIT_8;
        dcb_config.flow_ctrl = QOSA_FC_NONE;
        dcb_config.parity_bit = QOSA_UART_PARITY_NONE;
        dcb_config.stop_bit = QOSA_UART_STOP_1;

        qosa_uart_error_e ctl_ret = qosa_uart_ioctl(QCM_UART_LOG_PORT, QOSA_UART_IOCTL_SET_DCB_CFG, (void *)&dcb_config);
        if(ctl_ret != QOSA_UART_SUCCESS)
        {
            return;
        }
        /* Configure UART port pin functions */

        if(dcb_config.flow_ctrl == QOSA_FC_HW)
        {
            pinctrl_option_t cts_option = {.func = QCM_UART_LOGFLOW_CTRL_PIN_FUNC, .pull = QOSA_PIN_PULL_NONE, .driver = QOSA_PIN_DRIVE_LEVEL_0};
            pinctrl_option_t rts_option = {.func = QCM_UART_LOGFLOW_CTRL_PIN_FUNC, .pull = QOSA_PIN_PULL_NONE, .driver = QOSA_PIN_DRIVE_LEVEL_0};
            qosa_pinctrl_set_option(QCM_UART_LOG_CTS_PIN, cts_option);
            qosa_pinctrl_set_option(QCM_UART_LOG_RTS_PIN, rts_option);
        }
        else if(dcb_config.flow_ctrl == QOSA_FC_HW_RTS)
        {
            pinctrl_option_t rts_option = {.func = QCM_UART_LOGFLOW_CTRL_PIN_FUNC, .pull = QOSA_PIN_PULL_NONE, .driver = QOSA_PIN_DRIVE_LEVEL_0};
            qosa_pinctrl_set_option(QCM_UART_LOG_RTS_PIN, rts_option);
        }
        else if(dcb_config.flow_ctrl == QOSA_FC_HW_CTS)
        {
            pinctrl_option_t cts_option = {.func = QCM_UART_LOGFLOW_CTRL_PIN_FUNC, .pull = QOSA_PIN_PULL_NONE, .driver = QOSA_PIN_DRIVE_LEVEL_0};
            qosa_pinctrl_set_option(QCM_UART_LOG_CTS_PIN, cts_option);
        }

        pinctrl_option_t tx_option = {.func = QCM_UART_LOG_PIN_FUNC, .pull = QOSA_PIN_PULL_UP, .driver = QOSA_PIN_DRIVE_LEVEL_0};
        pinctrl_option_t rx_option = {.func = QCM_UART_LOG_PIN_FUNC, .pull = QOSA_PIN_PULL_UP, .driver = QOSA_PIN_DRIVE_LEVEL_0};
        qosa_pinctrl_set_option(QCM_UART1_TX_PIN, tx_option);
        qosa_pinctrl_set_option(QCM_UART1_RX_PIN, rx_option);
        /* Open UART port */
        qosa_uart_open(QCM_UART_LOG_PORT);
        uart_log_init_flag = 1;
    }
}

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
void qcm_uart_log_printf(int level, int tag, const char* funcname, int line, const char* fmt, ...)
{
    int     len = 0;
    va_list args;

    qosa_time_t      unix_time = 0;
    qosa_rtc_time_t  tm = {0};
    qosa_rtc_get_time(&unix_time);
    qosa_rtc_gmtime_r(&unix_time, &tm);
    qcm_log_uart_port_init();
    qosa_memset(g_uart_printf_buf, 0, QCM_UART_LOG_PRINTF_MAX_LEN);

    len = qosa_snprintf(g_uart_printf_buf, QCM_UART_LOG_PRINTF_MAX_LEN, "[%d-%d:%d:%d:%d]",tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    len += qosa_snprintf(g_uart_printf_buf + len, QCM_UART_LOG_PRINTF_MAX_LEN - len - 1, "[lev:%x,tag:%d,func:%s,ine:%d]", level, tag, funcname, line);

    va_start(args, fmt);
    len += qosa_vsnprintf(g_uart_printf_buf + len, QCM_UART_LOG_PRINTF_MAX_LEN - len - 1, fmt, args);
    va_end(args);
    len += qosa_snprintf(g_uart_printf_buf + len, QCM_UART_LOG_PRINTF_MAX_LEN - len - 1, "\r\n");
    qosa_uart_write(QCM_UART_LOG_PORT, (unsigned char *)g_uart_printf_buf, qosa_strlen(g_uart_printf_buf));
}

