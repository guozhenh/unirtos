/*****************************************************************/ /**
* @file qcm_app_entry.h
* @brief
* @author mahat.xu@quectel.com
* @date 2026-05-08
*
* @copyright Copyright (c) 2026 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2026-05-08 <td>1.0 <td>Mahat.Xu <td> Init
* </table>
**********************************************************************/


#ifndef __QCM_APP_ENTRY_H__
#define __QCM_APP_ENTRY_H__

#include "qosa_def.h"

typedef int (*osa_trace_func_t)(char* fmt, ... );
typedef int (*_get_api_ptr_t)(char* func_name );


extern osa_trace_func_t osa_trace_debug;
extern unsigned int  func_table_size;
extern _get_api_ptr_t m_get_api_ptr;

#define user_boot_init(user_boot_entry, startup_prio) userBootItem_t _userBootItem_##user_boot_entry _userBoot_attr_ = {user_boot_entry, startup_prio}

#define application_init(app_entry, app_name, stack_size_kib, startup_prio,arvg) \
appRegItem_t _regAppItem_##app_entry _appRegTable_attr_ = {app_entry, app_name, stack_size_kib*1024, startup_prio,arvg}


typedef void (*userBoot_t)(void * argv);
typedef struct
{
	userBoot_t user_boot_entry;
	unsigned char  startup_prio;
} userBootItem_t;
#define _userBoot_attr_ __attribute__((unused, section(".userBootTable")))


typedef void (*userKernel_t)(void * argv);
typedef struct
{
	userKernel_t user_kernel_entry;
	char *user_kernel_name;
	unsigned int  stack_size_byte;
	unsigned char  startup_prio;
    unsigned int   *arvg;
} userKernelItem_t;

#define KERNEL_TASK_STACK_SIZE 2048
#define KERNEL_TASK_NAME "kernel"
#define _userKernel_attr_ __attribute__((unused, section(".userKernelTable")))

typedef void (*app_t)(void * argv);
typedef struct
{
	app_t app_entry;
	char *app_name;
	unsigned int  stack_size_byte;
	unsigned char  startup_prio;
    unsigned int   *arvg;
} appRegItem_t;
#define _appRegTable_attr_ __attribute__((unused, section(".appRegTable")))

typedef struct app_boot_para
{
    void *static_maping;
    void *kernel_maping;
	void *reserver_maping;
}qosa_app_boot_para_t;

typedef struct func_map
{
    unsigned int*   table_size;
    void  			*get_fun_ptr_dep_name;
	//add system info here
}qosa_func_mapping_t;


#endif /* __QCM_APP_ENTRY_H__ */


