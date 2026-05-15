/*****************************************************************/ /**
* @file qosa_defer_time.h
* @brief Deferred timer interface for UniRTOS
* @author joe.tu@quectel.com
* @date 2026-03-11
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2026-03-11 <td>1.0 <td>joe.tu <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_DEFER_TIME_H__
#define __QOSA_DEFER_TIME_H__

#include "qosa_sys.h"

/**
 * @brief create a defer timer
 * 
 * @note This timer is specifically designed for low-power scenarios where strict timing precision is not critical. 
 * It will NOT wake up the system from sleep modes,  maximizing power efficiency.
 * 
 * @param[out] timerRef
 *          - Pointer to the timer handle to be created, which will be used for subsequent operations on this timer
 * @param[in] callBack
 *          - Pointer to the callback function to be executed when the timer expires
 * @param[in] argv
 *          - Argument to be passed to the callback function
 * @return int 
 */
int qosa_defer_timer_create(qosa_timer_t* timerRef, void (*callBack)(void*), void* argv);

/**
 * @brief start a defer timer
 * 
 * @param[in] timerRef
 *          - Timer handle pointer returned by qosa_defer_timer_create
 * @param[in] set_ms
 *          - Delay time in milliseconds
 * @param[in] cyclicalEn
 *          - Flag indicating whether the timer is cyclic
 * @return int 
 */
int qosa_defer_timer_start(qosa_timer_t timerRef, qosa_uint32_t set_Time, qosa_bool_t cyclicalEn);

/**
 * @brief stop a defer timer
 * 
 * @param[in] timerRef
 *          - Timer handle pointer returned by qosa_defer_timer_create
 * @return int 
 */
int qosa_defer_timer_stop(qosa_timer_t timerRef);

/**
 * @brief check if a defer timer is running
 * 
 * @param[in] timerRef
 *          - Timer handle pointer returned by qosa_defer_timer_create
 * @return qosa_bool_t 
 */
qosa_bool_t qosa_defer_timer_is_running(qosa_timer_t timerRef);

/**
 * @brief delete a defer timer
 * 
 * @param[in] timerRef
 *          - Timer handle pointer returned by qosa_defer_timer_create
 * @return int 
 */
int qosa_defer_timer_delete(qosa_timer_t timerRef);

/**
 * @brief Stop all running defer timers during low power mode (LPM)
 * 
 * @return int
 *        - Returns QOSA_ERROR_OK on success
 */
int qosa_defer_timer_suspend_for_lpm(void);

/**
 * @brief resume all defer timers that were stopped during low power mode (LPM)
 * 
 * @return int 
 *        - Returns QOSA_ERROR_OK on success
 */
int qosa_defer_timer_resume_from_lpm(void);

#endif // !__QOSA_DEFER_TIME_H__