/*****************************************************************/ /**
* @file qosa_gpio.h
* @brief GPIO operation interface
* @author larson.li@quectel.com
* @date 2025-01-09
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2025-01-09 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_GPIO_H__
#define __QOSA_GPIO_H__

#include "qosa_def.h"
#include "qosa_sys.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/

/*===========================================================================
 * Enum
 ===========================================================================*/

/**
 * @enum qosa_gpio_error_e
 * @brief GPIO error codes
 */
typedef enum
{
    QOSA_GPIO_SUCCESS = 0,                                       /*!< Indicates GPIO operation successful */
    QOSA_GPIO_EXECUTE_ERR = 1 | (QOSA_COMPONENT_BSP_GPIO << 16), /*!< Indicates GPIO operation execution failed */
    QOSA_GPIO_CFG_NOT_SUPPORT_ERR,                               /*!< Indicates GPIO operation not supported */
    QOSA_GPIO_INVALID_PARAM_ERR,                                 /*!< Indicates GPIO operation parameter invalid */
    QOSA_GPIO_OPEN_ERR,                                          /*!< Indicates GPIO operation open failed */
    QOSA_GPIO_CONFIG_ERR,                                        /*!< Indicates GPIO operation configuration failed */
    QOSA_GPIO_CALLBACK_ERR,                                      /*!< Indicates GPIO operation callback failed */
    QOSA_GPIO_LEVEL_TRIGGER_ERR,                                 /*!< Indicates GPIO operation trigger mode setting failed */
    QOSA_GPIO_MEM_NULL_ERR,                                      /*!< Indicates GPIO operation memory is null */
    QOSA_GPIO_SET_DRVING_ERR,                                    /*!< Indicates GPIO operation driving capability setting failed */
} qosa_gpio_error_e;

/**
 * @enum qosa_gpio_num_e
 * @brief GPIO number
 */
typedef enum
{
    QOSA_GPIO_0 = 0,
    QOSA_GPIO_1,
    QOSA_GPIO_2,
    QOSA_GPIO_3,
    QOSA_GPIO_4,
    QOSA_GPIO_5,
    QOSA_GPIO_6,
    QOSA_GPIO_7,
    QOSA_GPIO_8,
    QOSA_GPIO_9,
    QOSA_GPIO_10,
    QOSA_GPIO_11,
    QOSA_GPIO_12,
    QOSA_GPIO_13,
    QOSA_GPIO_14,
    QOSA_GPIO_15,
    QOSA_GPIO_16,
    QOSA_GPIO_17,
    QOSA_GPIO_18,
    QOSA_GPIO_19,
    QOSA_GPIO_20,
    QOSA_GPIO_21,
    QOSA_GPIO_22,
    QOSA_GPIO_23,
    QOSA_GPIO_24,
    QOSA_GPIO_25,
    QOSA_GPIO_26,
    QOSA_GPIO_27,
    QOSA_GPIO_28,
    QOSA_GPIO_29,
    QOSA_GPIO_30,
    QOSA_GPIO_31,
    QOSA_GPIO_32,
    QOSA_GPIO_33,
    QOSA_GPIO_34,
    QOSA_GPIO_35,
    QOSA_GPIO_36,
    QOSA_GPIO_37,
    QOSA_GPIO_38,
    QOSA_GPIO_39,
    QOSA_GPIO_40,
    QOSA_GPIO_41,
    QOSA_GPIO_42,
    QOSA_GPIO_43,
    QOSA_GPIO_44,
    QOSA_GPIO_45,
    QOSA_GPIO_46,
    QOSA_GPIO_47,
    QOSA_GPIO_48,
    QOSA_GPIO_49,
    QOSA_GPIO_50,
    QOSA_GPIO_51,
    QOSA_GPIO_52,
    QOSA_GPIO_53,
    QOSA_GPIO_54,
    QOSA_GPIO_55,
    QOSA_GPIO_56,
    QOSA_GPIO_57,
    QOSA_GPIO_58,
    QOSA_GPIO_59,
    QOSA_GPIO_60,
    QOSA_GPIO_61,
    QOSA_GPIO_62,
    QOSA_GPIO_63,
    QOSA_GPIO_64,
    QOSA_GPIO_65,
    QOSA_GPIO_66,
    QOSA_GPIO_67,
    /*ASR1903SR support
[*/
    QOSA_GPIO_68,
    QOSA_GPIO_69,
    QOSA_GPIO_70,
    QOSA_GPIO_71,
    QOSA_GPIO_72,
    QOSA_GPIO_73,
    QOSA_GPIO_74,
    QOSA_GPIO_75,
    QOSA_GPIO_76,
    QOSA_GPIO_77,
    QOSA_GPIO_78,
    QOSA_GPIO_79,
    QOSA_GPIO_80,
    QOSA_GPIO_81,
    QOSA_GPIO_82,
    QOSA_GPIO_83,
    QOSA_GPIO_84,
    QOSA_GPIO_85,
    QOSA_GPIO_86,
    QOSA_GPIO_87,
    QOSA_GPIO_88,
    QOSA_GPIO_89,
    QOSA_GPIO_90,
    QOSA_GPIO_91,
    QOSA_GPIO_92,
    QOSA_GPIO_93,
    QOSA_GPIO_94,
    QOSA_GPIO_95,
    QOSA_GPIO_96,
    QOSA_GPIO_97,
    QOSA_GPIO_98,
    QOSA_GPIO_99,
    QOSA_GPIO_100,
    QOSA_GPIO_101,
    QOSA_GPIO_102,
    QOSA_GPIO_103,
    QOSA_GPIO_104,
    QOSA_GPIO_105,
    QOSA_GPIO_106,
    QOSA_GPIO_107,
    QOSA_GPIO_108,
    QOSA_GPIO_109,
    QOSA_GPIO_110,
    QOSA_GPIO_111,
    QOSA_GPIO_112,
    QOSA_GPIO_113,
    QOSA_GPIO_114,
    QOSA_GPIO_115,
    QOSA_GPIO_116,
    QOSA_GPIO_117,
    QOSA_GPIO_118,
    QOSA_GPIO_119,
    QOSA_GPIO_120,
    QOSA_GPIO_121,
    QOSA_GPIO_122,
    QOSA_GPIO_123,
    QOSA_GPIO_124,
    QOSA_GPIO_125,
    QOSA_GPIO_126,
    QOSA_GPIO_127,
    /*              ]*/
    QOSA_GPIO_MAX
} qosa_gpio_num_e;

/**
 * @enum qosa_gpio_direction_e
 * @brief GPIO pin direction
 */
typedef enum
{
    QOSA_GPIO_DIRECTION_INPUT, /*!< GPIO PIN input */
    QOSA_GPIO_DIRECTION_OUTPUT /*!< GPIO PIN output */
} qosa_gpio_direction_e;

/**
 * @enum qosa_gpio_level_e
 * @brief GPIO pin output level
 */
typedef enum
{
    QOSA_GPIO_LEVEL_LOW, /*!< GPIO PIN output low level */
    QOSA_GPIO_LEVEL_HIGH /*!< GPIO PIN output high level */
} qosa_gpio_level_e;

/**
 * @enum qosa_gpio_pull_e
 * @brief GPIO input pull mode
 */
typedef enum
{
    QOSA_GPIO_PULL_NONE, /*!< GPIO PIN input pull none */
    QOSA_GPIO_PULL_DOWN, /*!< GPIO PIN input pull down */
    QOSA_GPIO_PULL_UP    /*!< GPIO PIN input pull up */
} qosa_gpio_pull_e;

/**
 * @enum qosa_gpio_trigger_e
 * @brief GPIO interrupt trigger mode
 */
typedef enum
{
    QOSA_GPIO_TRIGGER_DISABLED,     /*!< Disable interrupt */
    QOSA_GPIO_TRIGGER_LOW_LEVEL,    /*!< Low-level interrupt */
    QOSA_GPIO_TRIGGER_HIGH_LEVEL,   /*!< High-level interrupt */
    QOSA_GPIO_TRIGGER_FALLING_EDGE, /*!< Falling edge interrupt */
    QOSA_GPIO_TRIGGER_RISING_EDGE,  /*!< Rising edge interrupt */
    QOSA_GPIO_TRIGGER_BOTH_EDGE     /*!< Falling and rising edge interrupt */
} qosa_gpio_trigger_e;

/**
 * @enum qosa_gpio_debounce_e
 * @brief GPIO interrupt debounce
 */
typedef enum
{
    QOSA_GPIO_DEBOUNCE_DIS, /*!< GPIO PIN debounce disable */
    QOSA_GPIO_DEBOUNCE_EN   /*!< GPIO PIN debounce enable */
} qosa_gpio_debounce_e;

/*===========================================================================
 * callback
===========================================================================*/
// Corresponding interrupt callback function pointer type definition. Callback function parameter, corresponds to the user_ctx parameter during interrupt registration
typedef void (*gpio_interrupt_cb)(void *argv);

/*===========================================================================
 *  Struct
===========================================================================*/
/**
 * @enum qosa_int_cfg_t
 * @brief GPIO interrupt configuration structure
 */
typedef struct
{
    qosa_gpio_num_e      gpio_num;      /*!< GPIO NUM number to operate */
    qosa_gpio_debounce_e gpio_debounce; /*!< Hardware debounce switch */
    qosa_gpio_pull_e     gpio_pull;     /*!< Configure GPIO pull-up or pull-down mode */
    gpio_interrupt_cb    interrupt_cb;  /*!< GPIO interrupt callback function pointer */
    qosa_uint32_t        options;       /*!< Whether to enable single or multiple interrupt mode--bit0 */
    void                *user_ctx;      /*!< User-defined context data, used to pass parameters in interrupt callback function */
} qosa_int_cfg_t;

/**
 * @enum qosa_pin_cfg_t
 * @brief GPIO pin configuration structure
 */
typedef struct
{
    qosa_uint8_t          pin_num;      /*!< PIN NUM number to operate */
    qosa_uint8_t          default_func; /*!< Default fun of PIN NUM to operate */
    qosa_uint8_t          gpio_func;    /*!< GPIO fun corresponding to PIN NUM to operate */
    qosa_gpio_num_e       gpio_num;     /*!< GPIO NUM corresponding to PIN NUM to operate */
    qosa_gpio_direction_e gpio_dir;     /*!< Direction of PIN NUM to operate, input or output */
    qosa_gpio_pull_e      gpio_pull;    /*!< Default pull-up/pull-down of PIN NUM to operate */
    qosa_gpio_level_e     gpio_lvl;     /*!< Default output level of PIN NUM to operate */
} qosa_pin_cfg_t;

/**
 * @enum qosa_gpio_basic_isr_info_t
 * @brief GPIO interrupt callback information structure
 */
typedef struct
{
    qosa_gpio_num_e   gpio_num;      /*!< GPIO NUM that generated interrupt callback */
    qosa_gpio_level_e trigger_level; /*!< Level of corresponding GPIO */
} qosa_gpio_basic_isr_info_t;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/

/**
 * @brief Configure GPIO NUM initialization settings, including direction, pull-up/pull-down and output level.
 *
 * @param[in] qosa_gpio_num_e gpio_num
 *          - Configure the GPIO NUM number to operate
 *
 * @param[in] qosa_gpio_direction_e gpio_direction
 *          - Configure GPIO direction, input or output
 *
 * @param[in] qosa_gpio_pull_e gpio_pull
 *          - Configure GPIO pull-up or pull-down mode
 *
 * @param[in] qosa_gpio_level_e gpio_level
 *          - Configure GPIO output level, only valid when direction is output
 *
 * @return qosa_gpio_error_e
 *       - If operation is successful, return QOSA_GPIO_SUCCESS
 *       - Return corresponding error code if failed
 */
qosa_gpio_error_e qosa_gpio_init(qosa_gpio_num_e gpio_num, qosa_gpio_direction_e gpio_direction, qosa_gpio_pull_e gpio_pull, qosa_gpio_level_e gpio_level);

/**
 * @brief Deinitialize GPIO NUM configuration
 *
 * @param[in] qosa_gpio_num_e gpio_num
 *          - Configure the GPIO NUM number to operate
 *
 * @return qosa_gpio_error_e
 *       - If operation is successful, return QOSA_GPIO_SUCCESS
 *       - Return corresponding error code if failed
 */
qosa_gpio_error_e qosa_gpio_deinit(qosa_gpio_num_e gpio_num);

/**
 * @brief Used to set the pin level of the specified GPIO, only valid for output pins
 *
 * @param[in] qosa_gpio_num_e gpio_num
 *           - Configure the GPIO NUM number to operate
 *
 * @param[in] qosa_gpio_level_e gpio_level
 *          - Configure GPIO output level, only valid when direction is output
 *
 * @return qosa_gpio_error_e
 *       - If operation is successful, return QOSA_GPIO_SUCCESS
 *       - Return corresponding error code if failed
 */
qosa_gpio_error_e qosa_gpio_set_level(qosa_gpio_num_e gpio_num, qosa_gpio_level_e gpio_level);

/**
 * @brief Used to get the pin level of the specified GPIO
 *
 * @param[in] qosa_gpio_num_e gpio_num
 *          - Configure the GPIO NUM number to operate
 *
 * @param[out] qosa_gpio_level_e * gpio_level
 *          - Get GPIO output level
 *
 * @return qosa_gpio_error_e
 *       - If operation is successful, return QOSA_GPIO_SUCCESS
 *       - Return corresponding error code if failed
 */
qosa_gpio_error_e qosa_gpio_get_level(qosa_gpio_num_e gpio_num, qosa_gpio_level_e *gpio_level);

/**
 * @brief Used to set GPIO pin input/output direction
 *
 * @param[in] qosa_gpio_num_e gpio_num
 *          - Configure the GPIO NUM number to operate
 *
 * @param[in] qosa_gpio_direction_e gpio_direction
 *          - Configure GPIO direction, input or output
 *
 * @return qosa_gpio_error_e
 *       - If operation is successful, return QOSA_GPIO_SUCCESS
 *       - Return corresponding error code if failed
 */
qosa_gpio_error_e qosa_gpio_set_direction(qosa_gpio_num_e gpio_num, qosa_gpio_direction_e gpio_direction);

/**
 * @brief Used to get the pin input/output direction of the specified GPIO
 *
 * @param[in] qosa_gpio_num_e gpio_num
 *           - Configure the GPIO NUM number to operate
 *
 * @param[out] qosa_gpio_direction_e * gpio_direction
 *          - Get GPIO input/output direction
 *
 * @return qosa_gpio_error_e
 *       - If operation is successful, return QOSA_GPIO_SUCCESS
 *       - Return corresponding error code if failed
 */
qosa_gpio_error_e qosa_gpio_get_direction(qosa_gpio_num_e gpio_num, qosa_gpio_direction_e *gpio_direction);

/**
 * @brief Used to configure the pull-up/pull-down setting of the specified GPIO, only valid for input pins
 *
 * @param[in] qosa_gpio_num_e gpio_num
 *          - Configure the GPIO NUM number to operate
 *
 * @param[in] qosa_gpio_pull_e gpio_pull
 *          - Configure GPIO pull-up/pull-down mode, only valid when direction is input
 *
 * @return qosa_gpio_error_e
 *       - If operation is successful, return QOSA_GPIO_SUCCESS
 *       - Return corresponding error code if failed
 */
qosa_gpio_error_e qosa_gpio_set_pull(qosa_gpio_num_e gpio_num, qosa_gpio_pull_e gpio_pull);

/**
 * @brief GPIO get pull
 * @param gpio_num  : GPIO number
 * @param gpio_pull : GPIO get input pull
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_CONFIG_ERR
 *       QL_GPIO_SUCCESS
 */
/**
 * @brief Used to get the pull-up/pull-down attribute of the specified GPIO
 *
 * @param[in] qosa_gpio_num_e gpio_num
 *          - Configure the GPIO NUM number to operate
 *
 * @param[out] qosa_gpio_pull_e * gpio_pull
 *          - Get GPIO pull-up/pull-down mode
 *
 * @return qosa_gpio_error_e
 *       - If operation is successful, return QOSA_GPIO_SUCCESS
 *       - Return corresponding error code if failed
 */
qosa_gpio_error_e qosa_gpio_get_pull(qosa_gpio_num_e gpio_num, qosa_gpio_pull_e *gpio_pull);

/**
 * @brief Register GPIO interrupt callback function
 *
 * This function registers interrupt configuration for a specified GPIO pin,
 * including interrupt trigger mode and interrupt callback function.
 * After registration, when the GPIO pin generates an interrupt, the system
 * will call the corresponding callback function for processing.
 *
 * @param[in] int_cfg Pointer to the interrupt configuration structure, containing the following members:
 *                    - gpio_num: GPIO pin number
 *                    - interrupt_cb: Interrupt callback function pointer
 *                    - gpio_debounce: Configure whether to enable debouncing (hardware debouncing setting)
 *                    - gpio_pull: GPIO pull-up/pull-down configuration
 *                    - options: Enable single or multiple interrupt modes,
 *                               if 0, it represents single interrupt mode,
 *                               if 1, it represents multiple interrupt mode
 *                    - user_ctx: User-defined context pointer
 *
 * @return QOSA_GPIO_SUCCESS Successfully registered interrupt
 * @return QOSA_GPIO_INVALID_PARAM_ERR Invalid parameter (e.g., GPIO number out of range)
 * @return QOSA_GPIO_CONFIG_ERR GPIO configuration error (e.g., GPIO index check failed)
 *
 * @note This function initializes the GPIO to input mode and sets the pull-up/pull-down resistors according to configuration
 * @note After registering the interrupt, the corresponding interrupt enable function must be called to actually trigger the interrupt
 * @note This function should be called before qosa_interrupt_enable() to complete interrupt configuration
 */
qosa_gpio_error_e qosa_interrupt_register(qosa_int_cfg_t *int_cfg);

/**
 * @brief Used to unregister interrupt callback function, only valid for input pins.
 *
 * @param[in] qosa_gpio_num_e gpio_num
 *          - Configure the GPIO NUM number to operate
 *
 * @return qosa_gpio_error_e
 *       - If operation is successful, return QOSA_GPIO_SUCCESS
 *       - Return corresponding error code if failed
 */
qosa_gpio_error_e qosa_interrupt_unregister(qosa_gpio_num_e gpio_num);

/**
 * @brief Used to enable interrupt callback function.
 *
 * Must be registered with qosa_interrupt_register before calling this function.
 *
 * @param[in] qosa_gpio_num_e gpio_num
 *          - Configure the GPIO NUM number to operate
 *
 * @param[in] qosa_edge_mode_e pin_edge
 *          - Update configuration interrupt trigger edge
 *
 * @return qosa_gpio_error_e
 *       - If operation is successful, return QOSA_GPIO_SUCCESS
 *       - Return corresponding error code if failed
 */
qosa_gpio_error_e qosa_interrupt_enable(qosa_gpio_num_e gpio_num, qosa_gpio_trigger_e pin_edge);

/**
 * @brief Used to disable interrupt callback function.
 *
 * @param[in] qosa_gpio_num_e gpio_num
 *          - Configure the GPIO NUM number to operate
 *
 * @return qosa_gpio_error_e
 *       - If operation is successful, return QOSA_GPIO_SUCCESS
 *       - Return corresponding error code if failed
 */
qosa_gpio_error_e qosa_interrupt_disable(qosa_gpio_num_e gpio_num);

/**
 * @brief Used to get the default configuration of the corresponding pin.
 *
 * @param[in] qosa_uint8_t pin_num
 *          - Pin number to get configuration
 *
 * @param[out] qosa_pin_cfg_t *pin_cfg
 *          - Memory address to store the obtained configuration
 *
 * @return qosa_gpio_error_e
 *       - If operation is successful, return QOSA_GPIO_SUCCESS
 *       - Return corresponding error code if failed
 */
qosa_gpio_error_e qosa_get_pin_default_cfg(qosa_uint8_t pin_num, qosa_pin_cfg_t *pin_cfg);
#endif /* __QOSA_GPIO_H__ */
