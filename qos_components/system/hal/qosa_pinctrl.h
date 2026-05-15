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
#ifndef __QOSA_PINCTRLR_H__
#define __QOSA_PINCTRLR_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_gpio.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/

/*===========================================================================
 * Type Definition
 ===========================================================================*/
typedef enum
{
    QOSA_PINCTRL_SUCCESS = 0,                                       /*!< Indicates pinctrl operation successful */
    QOSA_PINCTRL_EXECUTE_ERR = 1 | (QOSA_COMPONENT_BSP_GPIO << 16), /*!< Indicates pinctrl operation execution failed */
    QOSA_PINCTRL_CFG_NOT_SUPPORT_ERR,                               /*!< Indicates pinctrl operation not supported */
    QOSA_PINCTRL_INVALID_PARAM_ERR,                                 /*!< Indicates pinctrl operation parameter invalid */
    QOSA_PINCTRL_OPEN_ERR,                                          /*!< Indicates pinctrl operation open failed */
    QOSA_PINCTRL_CONFIG_ERR,                                        /*!< Indicates pinctrl operation configuration failed */
    QOSA_PINCTRL_CALLBACK_ERR,                                      /*!< Indicates pinctrl operation callback failed */
    QOSA_PINCTRL_LEVEL_TRIGGER_ERR,                                 /*!< Indicates pinctrl operation trigger mode setting failed */
    QOSA_PINCTRL_MEM_NULL_ERR,                                      /*!< Indicates pinctrl operation memory is null */
    QOSA_PINCTRL_SET_DRVING_ERR,                                    /*!< Indicates pinctrl operation driving capability setting failed */
} qosa_pinctrl_error_e;

typedef enum
{
    QOSA_PIN_PULL_NONE       = 0, /*!< No pull */
    QOSA_PIN_PULL_DOWN       = 1, /*!< Enable pull-down */
    QOSA_PIN_PULL_UP         = 2, /*!< Enable pull-up */
} qosa_pin_pull_e;

typedef enum
{
    QOSA_PIN_DRIVE_LEVEL_0     = 0,
    QOSA_PIN_DRIVE_LEVEL_1     = 1,
    QOSA_PIN_DRIVE_LEVEL_2     = 2,
    QOSA_PIN_DRIVE_LEVEL_3     = 3,
    QOSA_PIN_DRIVE_LEVEL_4     = 4,
    QOSA_PIN_DRIVE_LEVEL_5     = 5,
    QOSA_PIN_DRIVE_LEVEL_6     = 6,
    QOSA_PIN_DRIVE_LEVEL_7     = 7,
} qosa_pin_driver_e;

typedef enum
{
    QOSA_PIN_1 = 1,
    QOSA_PIN_2 = 2,
    QOSA_PIN_3 = 3,
    QOSA_PIN_4 = 4,
    QOSA_PIN_5 = 5,
    QOSA_PIN_6 = 6,
    QOSA_PIN_7 = 7,
    QOSA_PIN_8 = 8,
    QOSA_PIN_9 = 9,
    QOSA_PIN_10 = 10,
    QOSA_PIN_11 = 11,
    QOSA_PIN_12 = 12,
    QOSA_PIN_13 = 13,
    QOSA_PIN_14 = 14,
    QOSA_PIN_15 = 15,
    QOSA_PIN_16 = 16,
    QOSA_PIN_17 = 17,
    QOSA_PIN_18 = 18,
    QOSA_PIN_19 = 19,
    QOSA_PIN_20 = 20,
    QOSA_PIN_21 = 21,
    QOSA_PIN_22 = 22,
    QOSA_PIN_23 = 23,
    QOSA_PIN_24 = 24,
    QOSA_PIN_25 = 25,
    QOSA_PIN_26 = 26,
    QOSA_PIN_27 = 27,
    QOSA_PIN_28 = 28,
    QOSA_PIN_29 = 29,
    QOSA_PIN_30 = 30,
    QOSA_PIN_31 = 31,
    QOSA_PIN_32 = 32,
    QOSA_PIN_33 = 33,
    QOSA_PIN_34 = 34,
    QOSA_PIN_35 = 35,
    QOSA_PIN_36 = 36,
    QOSA_PIN_37 = 37,
    QOSA_PIN_38 = 38,
    QOSA_PIN_39 = 39,
    QOSA_PIN_40 = 40,
    QOSA_PIN_41 = 41,
    QOSA_PIN_42 = 42,
    QOSA_PIN_43 = 43,
    QOSA_PIN_44 = 44,
    QOSA_PIN_45 = 45,
    QOSA_PIN_46 = 46,
    QOSA_PIN_47 = 47,
    QOSA_PIN_48 = 48,
    QOSA_PIN_49 = 49,
    QOSA_PIN_50 = 50,
    QOSA_PIN_51 = 51,
    QOSA_PIN_52 = 52,
    QOSA_PIN_53 = 53,
    QOSA_PIN_54 = 54,
    QOSA_PIN_55 = 55,
    QOSA_PIN_56 = 56,
    QOSA_PIN_57 = 57,
    QOSA_PIN_58 = 58,
    QOSA_PIN_59 = 59,
    QOSA_PIN_60 = 60,
    QOSA_PIN_61 = 61,
    QOSA_PIN_62 = 62,
    QOSA_PIN_63 = 63,
    QOSA_PIN_64 = 64,
    QOSA_PIN_65 = 65,
    QOSA_PIN_66 = 66,
    QOSA_PIN_67 = 67,
    QOSA_PIN_68 = 68,
    QOSA_PIN_69 = 69,
    QOSA_PIN_70 = 70,
    QOSA_PIN_71 = 71,
    QOSA_PIN_72 = 72,
    QOSA_PIN_73 = 73,
    QOSA_PIN_74 = 74,
    QOSA_PIN_75 = 75,
    QOSA_PIN_76 = 76,
    QOSA_PIN_77 = 77,
    QOSA_PIN_78 = 78,
    QOSA_PIN_79 = 79,
    QOSA_PIN_80 = 80,
    QOSA_PIN_81 = 81,
    QOSA_PIN_82 = 82,
    QOSA_PIN_83 = 83,
    QOSA_PIN_84 = 84,
    QOSA_PIN_85 = 85,
    QOSA_PIN_86 = 86,
    QOSA_PIN_87 = 87,
    QOSA_PIN_88 = 88,
    QOSA_PIN_89 = 89,
    QOSA_PIN_90 = 90,
    QOSA_PIN_91 = 91,
    QOSA_PIN_92 = 92,
    QOSA_PIN_93 = 93,
    QOSA_PIN_94 = 94,
    QOSA_PIN_95 = 95,
    QOSA_PIN_96 = 96,
    QOSA_PIN_97 = 97,
    QOSA_PIN_98 = 98,
    QOSA_PIN_99 = 99,
    QOSA_PIN_100 = 100,
    QOSA_PIN_101 = 101,
    QOSA_PIN_102 = 102,
    QOSA_PIN_103 = 103,
    QOSA_PIN_104 = 104,
    QOSA_PIN_105 = 105,
    QOSA_PINCTRL_MAX = 0,
} qosa_pin_num_e;

typedef struct pinctrl_option
{
    union
    {
        qosa_uint32_t option;
        struct
        {
            /* bit0~bit3, function select */
            unsigned int func : 4;
            /* bit4~bit5, pull up/down */
            unsigned int pull : 2;
            /* bit6~bit8, driver select */
            unsigned int driver : 3;
            /* bit9, smit */
            unsigned int smit : 1;
            /* bit10~bit31, reserved */
            unsigned int reserved : 22;
        };
    };
} pinctrl_option_t;

/*===========================================================================
 * Enum
 ===========================================================================*/
/**
 * @brief Set pin configuration options
 *
 * This function configures the specified pin's function selection, pull-up/down, and drive strength
 *
 * @param[in] pin_num
 *           - Pin number representing the physical pin to configure
 * @param[in] option
 *           - Pin configuration option structure containing:
 *             * func (bit0~bit3): Function selection, valid range 0-15
 *             * pull (bit4~bit5): Pull-up/down configuration, 0=none, 1=pull-down, 2=pull-up
 *             * driver (bit5~bit7): Drive strength, valid range 0-7
 *             * smit (bit8): Smit trigger configuration, 0=disable, 1=enable
 *             * reserved (bit9~bit31): Reserved bits, must be set to 0
 *
 * @return qosa_pinctrl_error_e
 *         - QOSA_PINCTRL_SUCCESS: Configuration successful
 *         - QOSA_PINCTRL_INVALID_PARAM_ERR: Invalid parameter (pin not found or configuration value out of range)
 *
 * @note
 *         - Pin number must be a valid pin in the configuration mapping table
 *         - func valid range: 0-15
 *         - pull valid range: 0-2 (0=none, 1=pull-down, 2=pull-up)
 *         - driver valid range: 0-7 (0-7 drive strength levels)
 *         - smit valid range: 0-1 (0=disable, 1=enable)
 *
 * @par Example:
 * @code
 *     // Configure pin as GPIO with pull-up and high drive strength
 *     pinctrl_option_t option;
 *     option.func = 0;                          // GPIO function
 *     option.pull = QOSA_PIN_PULL_UP;           // Pull-up
 *     option.driver = QOSA_PIN_DRIVE_LEVEL_7;   // High drive strength
 *     option.smit = 0;                          // Disable smit
 *     option.reserved = 0;
 *
 *     qosa_pinctrl_error_e ret = qosa_pinctrl_set_option(10, option);
 *     if (ret != QOSA_PINCTRL_SUCCESS)
 *     {
 *         // Handle configuration failure
 *     }
 * @endcode
 */
qosa_pinctrl_error_e qosa_pinctrl_set_option(qosa_pin_num_e pin_num, pinctrl_option_t option);

/**
 * @brief Used to configure the function of the corresponding module pin
 *
 * @param[in] qosa_uint8_t pin_num
 *          - Configure the pin number to operate
 *
 * @param[in] qosa_uint8_t func_sel
 *          - Configure the function selection of the pin
 *
 * @return qosa_pinctrl_error_e
 *       - If operation is successful, return QOSA_PINCTRL_SUCCESS
 *       - Return corresponding error code if failed
 */
qosa_pinctrl_error_e qosa_pin_set_func(qosa_pin_num_e pin_num, qosa_uint8_t func_sel);

/**
 * @brief Used to get the function of the corresponding module pin
 *
 * @param[in] qosa_uint8_t pin_num
 *          - Configure the pin number to operate
 *
 * @param[out] qosa_uint8_t * func_sel
 *          - Get the function selection of the pin
 *
 * @return qosa_pinctrl_error_e
 *       - If operation is successful, return QOSA_PINCTRL_SUCCESS
 *       - Return corresponding error code if failed
 */
qosa_pinctrl_error_e qosa_pin_get_func(qosa_pin_num_e pin_num, qosa_uint8_t *func_sel);

#endif /* __QOSA_PINCTRLR_H__ */