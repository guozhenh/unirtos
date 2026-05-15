/*****************************************************************/ /**
 * @file   qosa_hw_timer.h
 * @brief  Hardware timer interface for UniRTOS
 * @author Nike.Bu@quectel.com
 * @date   2026-04-22
 *
 * @copyright  Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
 * All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
 **********************************************************************/
#ifndef __QOSA_HW_TIMER_H__
#define __QOSA_HW_TIMER_H__

#include "qosa_def.h"


/**
 * @brief Hardware timer handle type (opaque pointer)
 */
typedef void* qosa_hw_timer_t;

/**
 * @brief Hardware timer callback function type
 * @param[in] ctx  User-defined context parameter
 */
typedef void (*qosa_hw_timer_cb)(void *ctx);

/**
 * @brief Hardware timer configuration structure
 */
typedef struct
{
    qosa_hw_timer_cb callback;   /*!< Timer callback function */
    void             *param;     /*!< Callback parameter (passed to callback) */
} qosa_hw_timer_cfg_t;

/**
 * @brief Create a hardware microsecond-level timer
 *
 * The system automatically allocates an available hardware timer instance (TIMER0~TIMER5).
 * If no timer instance is available, returns QOSA_ERROR_TIMER_CREATE_ERR.
 *
 * @param[out] timerRef
 *           - Pointer to receive the created timer handle
 *
 * @param[in] config
 *           - Pointer to timer configuration (callback and parameter)
 *
 * @return int
 *        - QOSA_ERROR_OK: Success
 *        - QOSA_ERROR_TIMER_INVALID_ERR: Invalid parameter
 *        - QOSA_ERROR_TIMER_CREATE_ERR: No available timer instance or creation failed
 *
 * @note The callback executes in interrupt context; avoid blocking operations.
 * @note Maximum 6 independent hardware timers can exist simultaneously.
 */
int qosa_hw_timer_create(qosa_hw_timer_t *timerRef, qosa_hw_timer_cfg_t *config);

/**
 * @brief Start a hardware microsecond-level timer
 *
 * @param[in] timerRef
 *           - Timer handle returned by qosa_hw_timer_create()
 *
 * @param[in] time_us
 *           - Timer period in microseconds
 *           - Minimum precision: 1us (26MHz clock source)
 *
 * @param[in] cyclicalEn
 *           - QOSA_TRUE:  Periodic timer (auto-reload)
 *           - QOSA_FALSE: One-shot timer (single execution)
 *
 * @return int
 *        - QOSA_ERROR_OK: Success
 *        - QOSA_ERROR_TIMER_INVALID_ERR: Invalid timer handle
 *        - QOSA_ERROR_TIMER_START_ERR: Failed to start timer
 */
int qosa_hw_timer_start(qosa_hw_timer_t timerRef, qosa_uint32_t time_us, qosa_bool_t cyclicalEn);

/**
 * @brief Stop a hardware microsecond-level timer
 *
 * @param[in] timerRef
 *           - Timer handle returned by qosa_hw_timer_create()
 *
 * @return int
 *        - QOSA_ERROR_OK: Success
 *        - QOSA_ERROR_TIMER_INVALID_ERR: Invalid timer handle
 *        - QOSA_ERROR_TIMER_STOP_ERR: Failed to stop timer
 */
int qosa_hw_timer_stop(qosa_hw_timer_t timerRef);

/**
 * @brief Delete a hardware microsecond-level timer and release resources
 *
 * @param[in] timerRef
 *           - Timer handle returned by qosa_hw_timer_create()
 *
 * @return int
 *        - QOSA_ERROR_OK: Success
 *        - QOSA_ERROR_TIMER_INVALID_ERR: Invalid timer handle
 *        - QOSA_ERROR_TIMER_DELETE_ERR: Failed to delete timer
 */
int qosa_hw_timer_delete(qosa_hw_timer_t timerRef);

/**
 * @brief Get current count value of a hardware timer
 *
 * @param[in] timerRef
 *           - Timer handle returned by qosa_hw_timer_create()
 *
 * @return int
 *        - Current count value in microseconds
 *        - 0 if invalid handle or timer not running
 */
int qosa_hw_timer_get_count(qosa_hw_timer_t timerRef);

#endif /* __QOSA_HW_TIMER_H__ */
