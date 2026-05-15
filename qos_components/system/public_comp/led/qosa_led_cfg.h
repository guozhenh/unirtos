/*****************************************************************/ /**
* @file qosa_led_cfg.h
* @brief LED Module Configuration Interface
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

#ifndef __QOSA_LED_CFG_H__
#define __QOSA_LED_CFG_H__

#include "qosa_def.h"
#include "qosa_led_rule.h"

/** Maximum number of LED configuration rules */
#define QOSA_LED_CONFIG_MAX_RULES 64

/**
 * @brief Add custom LED rule to configuration
 *
 * @param[in] rule
 *          - LED rule structure
 * @return int
 *          - 0 indicates success, non-zero indicates failure
 */
int qosa_led_config_add_rule(const qosa_led_rule_t *rule);

/**
 * @brief Get all rules in the configuration
 *
 * @param[out] rules
 *          - Rule array
 * @param[in] max_count
 *          - Maximum capacity of the array
 * @return int
 *          - The actual number of rules obtained; returns -1 on failure
 */
int qosa_led_config_get_rules(qosa_led_rule_t *rules, qosa_uint8_t max_count);

#endif /* __QOSA_LED_CFG_H__ */
