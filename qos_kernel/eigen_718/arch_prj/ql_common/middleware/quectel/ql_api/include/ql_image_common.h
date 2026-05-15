/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/

#ifndef QL_FLASH_RD_H
#define QL_FLASH_RD_H
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <alloca.h>
#include "mem_map.h"
#include "commontypedef.h"
#ifdef __cplusplus
extern "C" {
#endif



#ifndef NoError
#define NoError            		  (0x0)
#endif


//  check flash area for erase
typedef struct
{
	UINT32 start_addr;
	UINT32 end_addr;
}QUEC_PART_ADDR_S;

typedef struct
{
	char name[16];
	QUEC_PART_ADDR_S AddrInfo;
}QUEC_PART_ADDR_TYPE_S;


#ifdef QL_NORFLASH_ERASE_OPTIMIZE
#define QUEC_RECORD_MAGIC1        (0x78E5D4C2)
#define QUEC_RECORD_MAGIC2        (0x54F7D60E)  // only record cell 0 used , for mark record area is erase successed
#define QEUC_RECORD_ADDR_FLAG 	  (0x3F145605)
#define CHIP_SECTOR_SIZE 		  (0x1000)    	//erase 4k each times
#define CHIP_BLOCK_SIZE 		  (0x10000)    	//erase 64k each times
/*
#define QL_FLASH_EARSE_RECORD_ADDR  (UNIR_FOTA_INFO_ADDR + 0x400)
#define QL_FLASH_EARSE_RECORD_SECTOR_BEGIN_ADDR  (UNIR_FOTA_INFO_ADDR) // 4K split fota flag(1K) and FLASH erase record (3k)
#define QL_FLASH_EARSE_RECORD_SIZE  0xBF4  //erase 3060byte not 3k ,It must be an integer multiple quec_flashErase_record_s(20byte)
*/

typedef enum
{
	Record_type_Page,
	Record_type_32k,
	Record_type_Block
}quec_Record_type_enum;


typedef struct
{
	uint32_t flag;// 0xAAA55AA55
	uint32_t reserve;
}quec_Eraseflag_s;

typedef struct
{
	uint32_t RecordMagic;
	uint32_t EraseAddr;		// recored the eraseing  block id ,
	uint32_t Record_Size;  // recored the eraseing  block or page
}quec_RecordInfo_s;

typedef struct
{
	quec_RecordInfo_s ReInfo;
	quec_Eraseflag_s Eflag;
}quec_flashErase_record_s;
#endif

#define QL_BOOT_VERSION_OFFSET  (0x100)     //do not change,use in .ld,all version save at bin offeset 0x100(updater and app)
#define QL_BOOT_VERSION_LEN     (32)
#define QL_BOOT_VERSION_HEAD    "QUECTEL-UPDATE-VER:"
#define QL_BOOT_VERSION_DEFULAT QL_BOOT_VERSION_HEAD "V1.0"

typedef struct
{
	char ver[QL_BOOT_VERSION_LEN];
	char res[QL_BOOT_VERSION_OFFSET - QL_BOOT_VERSION_LEN];
}quec_version_info_s;


#ifdef QL_IMAGE_CHECK
#define QL_UPDATE_INFO_ADDR  (UPDATER_FLASH_LOAD_ADDR - AP_FLASH_XIP_ADDR + UPDATER_FLASH_LOAD_SIZE - CHIP_SECTOR_SIZE)

typedef enum
{
	ql_update_type,
	ql_type_max,
}ql_image_type;

typedef struct {
    uint8_t  version[16];
    uint8_t  build_time[64];
    uint32_t file_len;
    uint32_t crc;
	uint32_t reserved[128];
}ql_file_info_s;

#endif
void Quec_FlashEraseRecord_step1(uint32_t flash_addr,uint32_t size);
void Quec_FlashEraseRecord_step2(uint32_t flash_addr,uint32_t size);
void Quec_FlashErase_Optimize(void);
#ifdef QL_IMAGE_CHECK
void ql_udate_bin_crc(uint32_t *crc, const void *buffer, uint32_t size);
int ql_get_update_bin_info(ql_file_info_s *info);
int ql_check_updater_image_crc(void);
#endif
bool qosa_flash_limit_allow_check(uint32_t SectorAddress, uint32_t size);
uint32_t qosa_flash_limit_allow_record(uint32_t SectorAddress, uint32_t size);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_FLASH_RD_H */


