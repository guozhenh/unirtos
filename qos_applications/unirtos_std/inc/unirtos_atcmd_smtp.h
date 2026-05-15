
/**  @file
  unirtos_atcmd_smtp.h

  @brief
  TODO

*/

/*================================================================
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/

#ifndef __UNIRTOS_ATCMD_SMTP_H
#define __UNIRTOS_ATCMD_SMTP_H
#include "qosa_sys.h"

#include "qosa_at_param.h"

void qstd_exec_smtp_app_init(void);

void qstd_exec_smtp_qsmtpcfg_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_smtp_qsmtpdst_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_smtp_qsmtpsub_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_smtp_qsmtpbody_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_smtp_qsmtpatt_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_smtp_qsmtpclr_cmd_func(qosa_at_cmd_t *cmd);
void qstd_exec_smtp_qsmtpput_cmd_func(qosa_at_cmd_t *cmd);

#endif /* __UNIRTOS_ATCMD_SMTP_H */
