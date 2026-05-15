/*****************************************************************/ /**
* @file qosa_dev_eigen.h
* @brief RTC operation interface
* @author larson.li@quectel.com
* @date 2025-12-10
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2023-08-15 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_DEV_EIGEN_H__
#define __QOSA_DEV_EIGEN_H__

#include "qosa_system_config.h"
#if defined (CONFIG_QOSA_EIGEN718_PLATFORM_FUNC) || defined (CONFIG_QOSA_QCX216_PLATFORM_FUNC)
#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_gpio.h"
#include "qosa_sim.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
typedef struct qosa_tim_ic_handle qosa_tim_ic_handle_t;
/*===========================================================================
 * Enum
 ===========================================================================*/
typedef enum
{
    VOL_1_80V,
    VOL_3_30V
}qosa_gpio_vol_e;

typedef enum
{
    POWER_OFF,
    POWER_ON
}qosa_gpio_power_control_e;

typedef enum
{
    ROOT_CLK_26M,
    ROOT_CLK_612M
} qosa_FracDivRootClk_e;

typedef enum
{
    QOSA_TIMER_CAPTURE_RISING_EDGE      = 0U,  /**< Caputure Rising edge */
    QOSA_TIMER_CAPTURE_FALLING_EDGE     = 1U,  /**< Caputure falling edge */
    QOSA_TIMER_CAPTURE_BOTH_EDGE        = 2U,  /**< Caputure both edges */
} qosa_timerCaptureEdge_e;
typedef enum 
{
    QOSA_TIMER_OK = 0,
    QOSA_TIMER_INVALID_PARAM_ERR = -1,
    QOSA_TIMER_SET_CLK_ERR= -2,
} qosa_timer_error_e;
typedef enum
{
    QOSA_TIMER_CLK_INVALID = -1,        /*!< Invalid clock frequency configuration */
    QOSA_TIMER_CLK_40KHZ = 40000,       /*!< Clock frequency: 40K */
    QOSA_TIMER_CLK_26MHZ = 26000000,    /*!< Clock frequency: 26M */
} qosa_timer_clk_e;

/*===========================================================================
 * callback
===========================================================================*/
typedef void (*qosa_capture_callback_cb)(qosa_tim_ic_handle_t *htim );
/*===========================================================================
 *  Struct
===========================================================================*/
typedef struct {
    qosa_uint32_t           timer_id;              /**< Timer instance ID */
    qosa_uint32_t           clock_freq_hz;         /**< Timer clock frequency in Hz */
    qosa_uint16_t           prescaler;             /**< Clock prescaler value */
} qosa_tim_base_t;
typedef struct {
    qosa_uint8_t                pin_num;            /**< GPIO pin number for input capture */
    qosa_uint32_t               polarity;           /**< Capture polarity (reference qosa_timerCaptureEdge_e) */
    qosa_capture_callback_cb    capture_cb;         /**< Callback function invoked on capture event */
} qosa_tim_ic_config_t;
struct qosa_tim_ic_handle{
    qosa_tim_base_t         base;               /**< Timer base configuration */
    qosa_tim_ic_config_t    config;             /**< Input capture channel configuration */
    qosa_uint32_t           captured_value;     /**< Last captured value */
};
/*===========================================================================
 * Functions declaration
 ===========================================================================*/
/**
 * @brief Set the voltage level for GPIO operations.
 *
 * @param[in] gpio_vol
 *          - Voltage level to be set for GPIO
 *
 * @return qosa_gpio_error_e
 *       - If operation is successful, return QOSA_GPIO_SUCCESS
 *       - Return corresponding error code if failed
 */
qosa_gpio_error_e qosa_gpio_set_voltage(qosa_gpio_vol_e gpio_vol);

/**
 * @brief Turn on/off the AON GPIO power.
 *
 * @return qosa_gpio_error_e
 *       - If operation is successful, return QOSA_GPIO_SUCCESS
 *       - Return corresponding error code if failed
 */
qosa_gpio_error_e qosa_aon_gpio_power_control(qosa_gpio_power_control_e power_control);

/**
 * @brief Get the GNSS img address
 *
 * @return qosa_uint32_t
 *       - Returns 0 if not present
 *       - Returns firmware address if present
 */
qosa_uint32_t qosa_gnss_img_addr(void);

/**
 * @brief Enable 32k clock setting
 *
 * @param[in] enable
 *          - Enable 32k oscillating clock
 */
void qosa_32k_clock_enable(qosa_bool_t enable);

/**
 * @brief Synchronously get SIM power save status
 *
 * @param[in] simid
 *          - Application layer SIM ID, identifying the SIM card to read
 *          - Always 0 in dual card single standby mode
 * @param[out] status
 *          - SIM power save status
 *          - QOSA_FALSE: disabled
 *          - QOSA_TRUE: enabled
 * @return qosa_sim_err_e
 *          - QOSA_SIM_ERR_OK: Operation successful
 *          - QOSA_SIM_ERR_NOT_INSERTED: SIM card not inserted
 *          - QOSA_SIM_ERR_FAILURE: Read failed
 *          - QOSA_SIM_ERR_BUSY: SIM card busy
 *          - For other error codes, please refer to qosa_sim_err_e enumeration definition
 */
qosa_sim_err_e qosa_sim_get_sim_power_save(qosa_uint8_t simid, qosa_bool_t *status);

/**
 * @brief Synchronously set SIM power save status
 *
 * @param[in] simid
 *          - Application layer SIM ID, identifying the SIM card to read
 *          - Always 0 in dual card single standby mode
 * @param[in] status
 *          - SIM power save status
 *          - QOSA_FALSE: disable
 *          - QOSA_TRUE: enable
 * @return qosa_sim_err_e
 *          - QOSA_SIM_ERR_OK: Operation successful
 *          - QOSA_SIM_ERR_NOT_INSERTED: SIM card not inserted
 *          - QOSA_SIM_ERR_FAILURE: Read failed
 *          - QOSA_SIM_ERR_BUSY: SIM card busy
 *          - For other error codes, please refer to qosa_sim_err_e enumeration definition
 */
qosa_sim_err_e qosa_sim_set_sim_power_save(qosa_uint8_t simid, qosa_bool_t status);

/**
 * @brief Used to configure the mclk pin to output the desired clock frequency.
 *
 * @param[in] qosa_uint32_t pin_num
 *          - Pin number to get configuration
 * 
 * @param[in] qosa_uint32_t func_sel
 *          - Configure the function selection of the pin
 *
 * @param[in] qosa_uint32_t freq
 *          - Expected output clock frequency (Hz)
 *
 * @param[in] qosa_FracDivRootClk_e clkSrc
 *          - Root clock source selection, refer to the qosa_FracDivRootClk_e enumeration definition.
 * 
 * @return qosa_gpio_error_e
 *       - If operation is successful, return QOSA_GPIO_SUCCESS
 *       - Return corresponding error code if failed
 */
qosa_gpio_error_e qosa_pin_config_out_clk(qosa_uint32_t pin_num, qosa_uint32_t func_sel, qosa_uint32_t freq, qosa_FracDivRootClk_e clkSrc);

/**
 * @brief Used to Configure SWD function
 *
 * @note This function uses FUNC to configure the EC platform to enter J-Link mode.
 */
void qosa_set_swd_func(void);

/**
 * @brief Initialize the timer base clock for input capture.
 *
 * @param[in] htim
 *          - Pointer to the input capture handle
 *
 * @return qosa_int32_t
 *       - QOSA_TIMER_OK if initialization is successful
 *       - Return corresponding error code if failed
 */
qosa_int32_t qosa_tim_ic_init(qosa_tim_ic_handle_t *htim);
/**
 * @brief Deinitialize the timer and disable the clock.
 *
 * @param[in] htim
 *          - Pointer to the input capture handle
 *
 * @return qosa_int32_t
 *       - QOSA_TIMER_OK if deinitialization is successful
 *       - Return corresponding error code if failed
 */
qosa_int32_t qosa_tim_ic_deinit(qosa_tim_ic_handle_t *htim);
/**
 * @brief Configure a single input capture channel.
 *
 * @param[in] htim
 *          - Pointer to the input capture handle
 * @param[in] ch_config
 *          - Pointer to the channel configuration structure
 *
 * @return qosa_int32_t
 *       - QOSA_TIMER_OK if configuration is successful
 *       - Return corresponding error code if failed
 */
qosa_int32_t qosa_tim_ic_config_channel(qosa_tim_ic_handle_t *htim, 
                                       const qosa_tim_ic_config_t *ch_config);
/**
 * @brief Start input capture and enable the capture interrupt.
 *
 * @param[in] htim
 *          - Pointer to the input capture handle
 *
 * @return qosa_int32_t
 *       - QOSA_TIMER_OK if started successfully
 *       - Return corresponding error code if failed
 */
qosa_int32_t qosa_tim_ic_start_it(qosa_tim_ic_handle_t *htim);
/**
 * @brief Stop the input capture interrupt mode.
 *
 * @param[in] htim
 *          - Pointer to the input capture handle
 *
 * @return qosa_int32_t
 *       - QOSA_TIMER_OK if stopped successfully
 *       - Return corresponding error code if failed
 */
qosa_int32_t qosa_tim_ic_stop_it(qosa_tim_ic_handle_t *htim);
/**
 * @brief Get the most recently captured counter value
 *
 * @param[in] htim
 *          - Pointer to the input capture handle
 *
 * @return qosa_int32_t
 *       - The captured counter value
 *       - Return corresponding error code if failed
 */
qosa_int32_t qosa_tim_ic_get_captured_value(qosa_tim_ic_handle_t *htim);
/**
 * @brief Get the current timer counter value
 *
 * @param[in] htim
 *          - Pointer to the input capture handle
 *
 * @return qosa_int32_t
 *       - The current counter value
 *       - Return corresponding error code if failed
 */
qosa_int32_t qosa_tim_ic_get_counter(qosa_tim_ic_handle_t *htim);
#endif /* CONFIG_QOSA_EIGEN718_PLATFORM_FUNC */
#endif /* __QOSA_DEV_EIGEN_H__ */
