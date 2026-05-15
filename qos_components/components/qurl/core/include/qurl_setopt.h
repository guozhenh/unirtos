/** @file         qurl_setopt.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-06 17:08:51
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
*/

#ifndef QURL_SETOPT_H
#define QURL_SETOPT_H

#include "qurl_def_in.h"
#include "qurl_log.h"
#include "qurl_code_in.h"

qurl_ecode_t qurl_setopt_reset_for_core(qurl_core_ctrl_t *core_ptr);

qurl_ecode_t qurl_setopt_for_core(qurl_core_ctrl_t *core_ptr, qurl_opt_e opt, va_list param);

#endif /* Head define end*/
