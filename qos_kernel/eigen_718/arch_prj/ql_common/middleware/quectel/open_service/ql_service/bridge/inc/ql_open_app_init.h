/**  @file
  ql_open_app_init.h

  @brief
  TODO

*/

/*================================================================
  Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/

#ifndef __QUEC_OPEN_APP_INIT_H__
#define __QUEC_OPEN_APP_INIT_H__

#ifdef UNIRTOS_OPEN_MODE_SUPPORT

#include "ql_type.h"

#define QUEC_OPENSDK_MAGIC "QOS-APP"

typedef struct boot_para
{
    void *static_maping;
    void *kernel_maping;
	void *reserver_maping;
}ql_boot_para;

typedef struct {
	int (*trace)(const char* fmt, ... );
	int (*sprintf)(char *, const char *, ...);
	int (*snprintf)(char*, size_t, const char*, ...);
	int (*sscanf)(const char*, const char*, ...);
	int (*printf)(const char* fmt, ... );
	int  reserver[16];
} ql_static_mapping;

typedef struct func_map
{
    uint32_t  *table_size;
    void  *get_fun_ptr_dep_name;
	//add system info here
}func_mapping;

typedef struct
{
	char	magic[16];
	int 	crc;
	int (*qos_boot)(void *arvg);
	int (*qos_kernel)(void *arvg);
	int (*qos_app)(void *arvg);
	int *app_config;
	int *system_infomation;
	uint32_t qos_app_rom_code_start_addr; /* app rom code段起始地址*/
	uint32_t qos_app_rom_rw_start_addr; /* app rom rw段起始地址*/
	uint32_t qos_app_ram_start_addr; /* app ram 段起始地址*/
	uint32_t qos_app_ram_rw_end_addr; /* app ram rw段结束地址*/
	uint32_t qos_app_ram_end_addr; /* app rom code段起始地址*/
	int  reserver[9];/*整个结构体大小为：前面的字段大小：60字节 + 保留字段大小：9*4字节，一共96字节，保持32字节对齐*/
}qos_app_entry_s;


typedef struct
{
    signed int hash;
    void * api_ptr;
} ql_open_api_list;

#define UNIRTOS_APP_ENTRY_SECTION 	__attribute__((section(".unirtos_app_entry")))
#define UNIRTOS_APP_RAMCODE_SECTION __attribute__((section(".unirtos_app_ramcode")))
/* 跳转到新的app
 * @param p_rom_base_addr: 新appROM基地址
 * @param p_ram_base_addr: 新appRAM基地址
 */
extern void qosa_jump_app(uint32_t p_rom_base_addr, uint32_t p_ram_base_addr);
typedef void (*_api_qosa_jump_app_t)(uint32_t p_rom_base_addr, uint32_t p_ram_base_addr);

#endif
#endif

