
/**  @file
  ql_config.h

  @brief
  TODO

*/

/*================================================================
  Copyright (c) 2024 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/

// ��ȡQL_WAKEUP_HOTPLUG_IRQ_NUM��ֵ
int get_ql_wakeup_hotplug_irq_num(void);

// ��ȡQL_WAKEUP_HOTPLUG_ID��ֵ
int get_ql_wakeup_hotplug_id(void);


// ��ȡQL_WAKEUP_DTR_IRQ_NUM��ֵ��UNIRTOS_COMMON_MODIFY����ʱ��
int get_ql_wakeup_dtr_irq_num(void);

// ��ȡQL_WAKEUP_DTR_ID��ֵ��UNIRTOS_COMMON_MODIFY����ʱ��
int get_ql_wakeup_dtr_id(void);
