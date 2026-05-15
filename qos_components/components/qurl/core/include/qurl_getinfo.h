/** @file         qurl_getinfo.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2024-01-22 17:08:51
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
*/

#ifndef QURL_GETINFO_H
#define QURL_GETINFO_H

#include "qurl_def_in.h"
#include "qurl_log.h"
#include "qurl_code_in.h"

qurl_ecode_t qurl_getinfo_for_core(qurl_core_ctrl_t *core_ptr, qurl_info_e info, va_list param);

#endif /* Head define end*/
