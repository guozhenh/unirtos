/****************************************************************************
 *
 * Copy right:   2019-, Copyrigths of EigenComm Ltd.
 * File name:    plat_config.h
 * Description:  platform configuration header file
 * History:      Rev1.0   2019-01-18
 *               Rev1.1   2019-11-27   Reimplement file operations with OSA APIs(LFS wrapper), not directly using LFS APIs in case of file system replacement
 *               Rev1.2   2020-01-01   Separate plat config into one parts, raw flash
 *
 ****************************************************************************/

#ifndef  _QUEC_PLAT_CONFIG_H
#define  _QUEC_PLAT_CONFIG_H

#include "Driver_Common.h"
#include "cmsis_compiler.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define QUEC_RAW_FLASH_PLAT_CONFIG_FILE_CURRENT_VERSION    (1)


 /** \brief config file header typedef */
__PACKED_STRUCT quec_config_file_header
{
    uint16_t fileBodySize;     /**< size of file body, in unit of byte */
    uint8_t  version;          /**< file version, this field shall be updated when file structure is changed */
    uint8_t  checkSum;         /**< check sum value of file body */
};
typedef struct quec_config_file_header quec_config_file_header_t;

/** \brief typedef of platform configuration stored in raw flash --old v0*/
__PACKED_STRUCT quec_plat_config_raw_flash_v0
{
    /* quec add for ab fota */
    uint32_t q_new_app_runing_flash_addr;

    uint32_t q_new_app_runing_ram_addr;
    /* quec add for ab fota end */
 
};

#define     QUEC_PLAT_CFG_RAW_FLASH_RSVD_SIZE    232
/** \brief typedef of platform configuration stored in raw flash */
__PACKED_STRUCT quec_plat_config_raw_flash
{
    /* quec add for ab fota */
    uint32_t q_new_app_runing_flash_addr;

    uint32_t q_new_app_runing_ram_addr;
    /* quec add for ab fota end */
    /* 'QUEC_PLAT_CFG_RAW_FLASH_RSVD_SIZE' bytes rsvd for future */
    /*quec plat_config struct total size is 240 bytes , eigen plat_config struct total size is 60 bytes */
    uint8_t resv[QUEC_PLAT_CFG_RAW_FLASH_RSVD_SIZE];
};

typedef struct quec_plat_config_raw_flash quec_plat_config_raw_flash_t;//current
typedef struct quec_plat_config_raw_flash_v0 quec_plat_config_raw_flash_v0_t;//old v0

/** \brief typedef of platform info layout stored in raw flash */
__PACKED_STRUCT quec_plat_info_layout
{
    quec_config_file_header_t header;           /**< raw flash plat config header */
    quec_plat_config_raw_flash_t config;        /**< raw flash plat config body */
    uint32_t quec_fsAssertCount;                /**< count for monitoring FS assert, when it reaches specific number, FS region will be re-formated */
};
typedef struct quec_plat_info_layout quec_plat_info_layout_t;

/** @brief List of platform configuration items used to set/get sepecific setting */
typedef enum quec_plat_config_id
{
    PLAT_CONFIG_ITEM_NEW_APP_RUNING_FLASH_ADDR,/**< Flash address of the new application to be executed */
    PLAT_CONFIG_ITEM_NEW_APP_RUNING_RAM_ADDR,/**< RAM address where the new application will be loaded and executed */
} quec_plat_config_id_t;


/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
    extern "C" {
#endif

/**
  \fn        void QUEC_SavePlatConfigToRawFlash(void)
  \brief     Save platform configuration into raw flash
  \return    void
 */
void QUEC_SavePlatConfigToRawFlash(void);

/**
  \fn        void QUEC_LoadPlatConfigFromRawFlash(void)
  \brief     Load platform configuration from raw flash
  \return    void
 */
void QUEC_LoadPlatConfigFromRawFlash(void);

/**
  \fn        quec_plat_config_raw_flash_t* QUEC_GetRawFlashPlatConfig(void)
  \brief     Get raw flash platform configuration variable pointer
  \return    pointer to internal platform configuration loaded from raw flash
 */
quec_plat_config_raw_flash_t* QUEC_GetRawFlashPlatConfig(void);

/**
  \fn        uint32_t QUEC_GetPlatConfigItemValue(quec_plat_config_id_t id)
  \brief     Get value of specific platform configuration item
  \param[in] id    id of platform configuration item, \ref quec_plat_config_id_t
  \return    value of current configuration item
 */
uint32_t QUEC_GetPlatConfigItemValue(quec_plat_config_id_t id);

/**
  \fn        void QUEC_SetPlatConfigItemValue(quec_plat_config_id_t id, uint32_t value)
  \brief     Set value of specific platform configuration item
  \param[in] id    id of platform configuration item, \ref quec_plat_config_id_t
  \param[in] value value of configuration item to set
  \return    void
 */
void QUEC_SetPlatConfigItemValue(quec_plat_config_id_t id, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif /* QUEC_PLAT_CONFIG_H */
