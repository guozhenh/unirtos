/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/


/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/
#ifndef QL_UART_H
#define QL_UART_H


/*===========================================================================
 * include files
 ===========================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ql_api_common.h"
#include "ccio_base.h"


#if defined(QL_UART0_AT_SUPPORT)
void ql_uart0_mode_set(uint32_t diagport_mode);
CcioDevWorkState_e ql_uart0_mode_get(void);
int ql_uart0_mode_read_config_nv(uint8 *diagport_mode);
#endif /* QL_UART0_AT_SUPPORT */

#endif /* QL_UART_H */

