/*****************************************************************/ /**
* @file qosa_led_rule.h
* @brief LED Rule Definitions and Interfaces
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

#ifndef __QOSA_LED_RULE_H__
#define __QOSA_LED_RULE_H__

#include "qosa_def.h"
#include "qosa_led_event.h"

#define QOSA_LED_COND_BIT_0       (0)
#define QOSA_LED_COND_BIT_1       (1)
#define QOSA_LED_COND_BIT_2       (2)
#define QOSA_LED_COND_BIT_3       (3)
#define QOSA_LED_COND_BIT_4       (4)
#define QOSA_LED_COND_BIT_5       (5)

#define QOSA_LED_COND_BIT_NOT_REG (1 << QOSA_LED_COND_BIT_1)
#define QOSA_LED_COND_BIT_REG_2G  (1 << QOSA_LED_COND_BIT_2)
#define QOSA_LED_COND_BIT_REG_3G  (1 << QOSA_LED_COND_BIT_3)
#define QOSA_LED_COND_BIT_REG_4G  (1 << QOSA_LED_COND_BIT_4)
#define QOSA_LED_COND_BIT_REG_5G  (1 << QOSA_LED_COND_BIT_5)

#define QOSA_LED_COND_BIT_ALL     (0x3e)

/*
 * LED Rule Construction Macro
 * Parameter Description:
 * - event_id: Event ID (use qosa_led_event_e enum, highest bit reserved for indicating inverse matching)
 * - high: High-level duration, unit ms (0~65535, 0 means always off)
 * - period: Period duration, unit ms (1~65535)
 * - driver_id: Driver ID (use numeric value directly)
 * - condition: Condition field (lower 8 bits for network type: 0=NOT_REGISTERED, 1=4G)
 *
 * Note: The priority parameter has been removed and is fixed at 0
 */
#define QOSA_LED_RULE(_event_id, _high, _period, _driver_id, _priority, _condition)                                                                            \
    {                                                                                                                                                          \
        .rule_id = 0, .event_id = (_event_id), .reserved = 0, .driver_id = (_driver_id), .priority = (_priority), .condition = (_condition), .high = (_high),  \
        .period = (_period)                                                                                                                                    \
    }

/**
 * @brief Network Type Definitions
 */
typedef enum
{
    QOSA_LED_NET_TYPE_NOT_REGISTERED = 0, /*!< Unregistered network */
    QOSA_LED_NET_TYPE_4G,                 /*!< 4G network */
    QOSA_LED_NET_TYPE_MAX
} qosa_led_net_type_e;

/**
 * @brief LED Rule Structure
 *
 */
typedef struct
{
    qosa_uint8_t rule_id;         /*!< Rule ID, 0~255 */
    qosa_uint8_t event_id;        /*!< Event ID, 0~255 */

    qosa_uint8_t reserved;        /*!< Reserved, set to 0 */
    qosa_uint8_t driver_id;       /*!< Driver group, high 3 bits represent gpio num id, next 3 bits represent pwm num id, low 2 bits reserved */

    qosa_uint32_t priority  : 16; /*!< Priority */
    qosa_uint32_t reserved2 : 16;

    qosa_uint32_t condition; /*!< Status bit combination */

    qosa_uint16_t high;      /*!< ON state duration, 0~65535, unit is ms, 0 means always off, maximum high-level time is 65 seconds */
    qosa_uint16_t period;    /*!< Period, 1~65535, unit is ms, maximum period is 65 seconds */
} qosa_led_rule_t;

/**
 * @brief Predefined LED rule template data
 *
 * These template data are imported into the runtime templates during LED service initialization
 * Each rule corresponds to an LED behavior configuration for a specific event and network type
 */
extern const qosa_led_rule_t qosa_led_pre_rule_template_table[];

qosa_uint8_t qosa_led_rule_get_template_count(void);

#endif /* __QOSA_LED_RULE_H__ */
