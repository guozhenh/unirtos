
/**  @file
  ql_fota_common.h

  @brief
  TODO

*/

/*================================================================
  Copyright (c) 2025 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/

/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef QL_FOTA_COMMON_H
#define QL_FOTA_COMMON_H





#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_FOTA_FILENAME_MAX     (64)
#define QL_FOTA_HEAD_INFO        "QUEC-FOTA"
#define QL_FOTA_READY            (0x12345678)  //DFOTA模式标记
#define QL_FULLFOTA_READY        (0x87654321)  //FULLFOTA模式标记
#define QL_FOTA_INFO_FLASH_ADDR  (UNIR_FOTA_INFO_ADDR)
#define QL_FOTA_FLAG_RW_ADDR     (QL_FOTA_INFO_FLASH_ADDR + 0x300)
#define QL_FOTA_INFO_FLASH_SIZE  (0x1000)



#define QL_FOTAUPL_URL_MAX_LEN             (256)
#define QL_FOTAUPL_FILENAME_MAX_LEN        (64)
#define QL_FOTA_URC_NUM_NONE      (0)
#define QL_FOTA_URC_NUM_MIN       (5)
#define QL_FOTA_URC_NUM_MAX       (100)
#define QL_FOTA_GENUS_URC_NUM     (5)
#define QL_FOTA_PORT_NUM     	  (8)

#define QL_FOTA_DFOTA_MODE		  (0)
#define QL_FOTA_QDOWNLOAD_MODE	  (1)


//文件路径名处理的结果枚举
typedef enum
{
	QL_FOTA_PATH_NULL = 0,
	QL_FOTA_PATH_INVALID,
	QL_FOTA_PATH_FILE,//fota by file
	QL_FOTA_PATH_SD,//fota by sd card
	QL_FOTA_PATH_MEMORY,//fota by memory
	QL_FOTA_PATH_HTTP,//fota by http
	QL_FOTA_PATH_HTTPS,//fota by https
	QL_FOTA_PATH_FTP,//fota by ftp
	QL_FOTA_PATH_FTPS,//fota by ftp
	QL_FOTA_PATH_DLOAD_FILE,//fota by dload file mode
	QL_FOTA_PATH_MAX,
}quec_fota_path_e;

/*===========================================================================
 * Enum
 ===========================================================================*/

typedef struct {
    uint8_t             dl_urc_num;       //max number of urc when downloading
    uint8_t             up_urc_num;
    uint8_t             urc_cnt;
    uint8_t             is_send_end;
	quec_fota_path_e    fota_type;
    uint32_t            flash_wr_size;
    uint32_t            flash_want_size;  //当下载模式为http时，下载总长度在回调函数fota_http_event_cb中获得
	float               last_urc;
	uint32_t            update_offset;
}ql_fota_urc;

typedef struct
{
	ql_fota_urc           urc_param;
	uint32_t              baudrate;
	char       		      path_name[QL_FOTAUPL_URL_MAX_LEN];
	char        		  filename[QL_FOTAUPL_FILENAME_MAX_LEN];

	/* Add for qdownload */
	//uint8_t				  mode;
	//uint8_t				  refuse_exit;
	//uint8_t 			  protocol_port;
	//uint8_t               reserved[123];
}QL_FOTA_MAJOR_INFO;

typedef struct
{
	char	 fota_head[16];
	uint32_t fota_flag;
	QL_FOTA_MAJOR_INFO info;
}QL_FOTA_INFO;

typedef enum{
	QL_FOTA_OPT_STATUS_STOP = 0,
	QL_FOTA_OPT_STATUS_START = 1,
    QL_FOTA_OPT_STATUS_UPDATA = 2
}QL_FOTA_CURRENT_OPT_STATUS;

//Fota Upgrade Result Code
typedef enum
{
	QL_FOTA_UPGRADE_SUCCESS = 0,
	QL_FOTA_UPGRADE_FAIL = 504,
	QL_FOTA_UPGRADE_CHECK_FAIL = 505,
	QL_FOTA_UPGRADE_MD5_FAIL = 506,
	QL_FOTA_UPGRADE_MATCH_FAIL = 507,
	QL_FOTA_UPGRADE_NO_FILE_FAIL = 508,
	QL_FOTA_UPGRADE_OPENFILE_FAIL = 509,
	QL_FOTA_UPGRADE_FILESIZE_FAIL = 510,
	QL_FOTA_UPGRADE_LFS_MOUNT_FAIL = 511,
	QL_FOTA_UPGRADE_PARAM_FAIL = 512,
	QL_FOTA_UPGRADE_PROJECT_MATCH_FAIL = 552,
	QL_FOTA_UPGRADE_BASELINE_MATCH_FAIL = 553
}QL_FOTA_UPGRADE_RESULT;

/*===========================================================================
 * function
 ===========================================================================*/
int ql_fota_flag_clear(void);
int ql_fota_flag_get(void *path, int *fotaflag);
int ql_fota_flag_set(const QL_FOTA_MAJOR_INFO *path, int flag);


#ifdef QL_FOTA_UPDATE_SUPPORT
int     ql_update_lfs_init(void);
int     ql_filereload_check(const char *filename, uint32_t *length);
int 	ql_check_file_whether_exist(char *file_name);
int32_t ql_fota_nvm_read(uint32_t zid, uint32_t offset, uint8_t *buf, uint32_t bufLen);
int32_t ql_fota_nvm_write(uint32_t zid, uint32_t offset, uint8_t *buf, uint32_t bufLen);
int32_t ql_fota_nvm_remove(const char *path);
int32_t ql_fota_file_size_get(const char *path, uint32_t *file_size);
int32_t ql_fota_nvm_close(const char *filename);
int32_t ql_fota_nvm_open_rw(const char *filename);
int     ql_fota_update_urc_send(int percent, int state, int error, void *param);

QL_FOTA_INFO*  ql_fota_flag_update(void);
int ql_fota_process_update(void);
int ql_fota_update_urc_send(int percent, int state, int error, void *param);
int ql_update_ext_lfs_init(void);
#endif
#ifdef __cplusplus
} /*"C" */
#endif


#endif /* QL_GPIO_H */


