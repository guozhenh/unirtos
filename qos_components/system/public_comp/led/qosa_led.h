/*****************************************************************/ /**
* @file qosa_led.h
* @brief Main interface definitions for the LED module
* @author
* @date 2025-09-18
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description
* <tr><td>2025-09-18 <td>1.0 <td>Initial <td> Init
* </table>
**********************************************************************/

#ifndef __QOSA_LED_H__
#define __QOSA_LED_H__

#include "qosa_def.h"
#include "qosa_led_event.h"

#ifdef CONFIG_QOSA_LED_FUNC
#define QOSA_LED_NOTIFY_SLEEP()  qosa_led_notify_sleep()
#define QOSA_LED_NOTIFY_WAKEUP() qosa_led_notify_wakeup()
#else /* CONFIG_QOSA_LED_FUNC */
#define QOSA_LED_NOTIFY_SLEEP()
#define QOSA_LED_NOTIFY_WAKEUP()
#endif /* CONFIG_QOSA_LED_FUNC */

/**
 * @brief Set LED event
 *
 * @param[in] sim_id
 *          - SIM card ID, used for dual-SIM distinction
 * @param[in] event_id
 *          - Event ID, refer to the qosa_led_event_e enumeration definition
 * @return int
 *          - 0 indicates success, non-zero indicates failure
 */
int qosa_led_set_event(qosa_uint8_t sim_id, qosa_led_event_e event_id);

/**
 * @brief Notify LED to enter sleep state
 *
 *       When the system enters sleep state, call this function to notify the LED module.
 *       The LED module will turn off all LED indicators to save power.
 *       This function notifies the LED task to perform sleep operation by setting a sleep flag.
 *
 * @return int 0 indicates success, non-zero indicates failure
 *         - QOSA_ERROR_OK: Successfully set the sleep flag
 *         - Other values: Failed to set the flag
 */
int qosa_led_notify_sleep(void);

/**
 * @brief Notify LED to wake up from sleep state
 *
 *       When the system wakes up from sleep state, call this function to notify the LED module.
 *       The LED module will re-evaluate the current network status and events, and restore the normal operation of the LED indicator.
 *       This function notifies the LED task to perform the wake-up operation by setting a wake-up flag.
 *
 * @return int 0 indicates success, non-zero indicates failure
 *         - QOSA_ERROR_OK: Successfully set the wake-up flag
 *         - Other values: Failed to set the flag
 */
int qosa_led_notify_wakeup(void);

/**
 * @brief Set NET_STATUS LED mode
 *
 *       Set the operating mode of the NET_STATUS LED. The following modes are supported:
 *       - 0: Conventional module mode
 *       - 1: Same as 2
 *       - 2: MINIPCIE mode
 *
 * @param[in] mode
 *          - LED mode, value range 0-2
 * @return int 0 indicates success, non-zero indicates failure
 */
int qosa_led_net_status_mode_set(qosa_uint8_t mode);

/**
 * @brief Get LED mode
 *
 * @param[out] mode
 *          - Output parameter: current LED mode
 * @return int
 *          - 0 indicates success, non-zero indicates failure
 */
int qosa_led_net_status_mode_get(qosa_uint8_t *mode);

/**
 * @brief Set the (U)SIM card ID pointed to by the LED
 *
 * @param[in] sim_id
 *           - (U)SIM card ID
 *              0: (U)SIM card 1
 *              1: (U)SIM card 2
 *
 * @return int 0 indicates success, non-zero indicates failure
 */
int qosa_led_sim_id_set(qosa_uint8_t sim_id);

/**
 * @brief Get the (U)SIM card ID pointed to by the LED
 *
 * @param[out] sim_id
 *           - Output parameter: The (U)SIM card ID currently pointed to by the LED
 * @return int 0 indicates success, non-zero indicates failure
*/
int qosa_led_sim_id_get(qosa_uint8_t *sim_id);

/**
 * @brief LED module initialization
 *
 * @note This function is called automatically during system startup; the user does not need to call it manually
 */
void qosa_led_init(void);

#endif /* __QOSA_LED_H__ */
