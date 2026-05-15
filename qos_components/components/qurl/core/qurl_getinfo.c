/** @file         qurl_getinfo.c
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2024-01-22 17:07:28
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#include "qurl_getinfo.h"

#include "qurl_http_def.h"
#include "qurl_http.h"

qurl_ecode_t qurl_getinfo_for_core(qurl_core_ctrl_t *core_ptr, qurl_info_e info, va_list param)
{
    qurl_ecode_t ret = QURL_OK;

    switch (info)
    {
        case QURL_INFO_RESP_CODE: {
            long *arg_str = va_arg(param, long *);

            *arg_str = (long)core_ptr->info.resp_code;
            break;
        }
        case QURL_INFO_RESP_CONTENT_LENGTH: {
            long *arg_str = va_arg(param, long *);

            *arg_str = (long)core_ptr->info.content_length;
            break;
        }
        case QURL_INFO_FILETIME: {
            long *arg_str = va_arg(param, long *);

            if (core_ptr->info.filetime > LONG_MAX)
            {
                *arg_str = LONG_MAX;
            }
            else
            {
                *arg_str = (long)core_ptr->info.filetime;
            }
            break;
        }
        case QURL_INFO_RESP_DATE: {
            char **arg_str = va_arg(param, char **);

            *arg_str = core_ptr->info.resp_date ? qurl_strdup(core_ptr->info.resp_date) : QOSA_NULL;
            break;
        }
        case QURL_INFO_START_POS: {
            long *arg_str = va_arg(param, long *);

            *arg_str = (long)core_ptr->info.start_pos;
            break;
        }
        default:
            ret = QURL_ECODE_PARAM_INVALID;
            break;
    }

    qurl_logd("info:[%x],ret:[%x]\r\n", info, ret);
    return ret;
}
