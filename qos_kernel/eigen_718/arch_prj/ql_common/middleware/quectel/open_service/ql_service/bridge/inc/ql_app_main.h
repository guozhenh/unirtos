/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
#ifndef __QL_APP_MAIN_H__
#define __QL_APP_MAIN_H__

#include "ql_type.h"

typedef void (*ql_app_t)(void *argv);

#define _appRegTable_attr_ volatile const __attribute__((__section__(".LOAD_APP_TABLE")))

typedef struct
{
	ql_app_t  app_entry;
	char     *app_name;
	UINT32  stack_size_byte;
	UINT8   startup_prio;
	UINT32  *arvg;
} appRegItem_t;

#define application_init(app_entry, app_name, stack_size_kib, startup_prio, arvg) _appRegTable_attr_  appRegItem_t _regAppItem_##app_entry  = {app_entry, app_name, stack_size_kib*1024, startup_prio, arvg}

#ifdef UNIRTOS_OPEN_MODE_SUPPORT

typedef int (*_ql_trace_t)(const char* fmt, ... );
typedef int (*_ql_printf_t)(const char* fmt, ... );
typedef int (*_get_api_ptr_t)(char* func_name );

extern _ql_trace_t ql_trace;
extern _get_api_ptr_t m_get_api_ptr;
#define SDK_API_DEBUG_NOTSUP() ql_trace("ERROR:%s NOT SUPPORT",__FUNCTION__)
#define SDK_API_DEBUG_FUNC_PTR(func_ptr) ql_trace("DEBUG:%s function pointer address: 0x%x",__FUNCTION__, (void*)(func_ptr))
#define SDK_API_DEBUG_FUNC_PTR_MSG(msg, func_ptr) ql_trace("DEBUG:%s " msg " function pointer address: 0x%x",__FUNCTION__, (void*)(func_ptr))
#else

#include "ql_debug.h"

#endif

#endif
