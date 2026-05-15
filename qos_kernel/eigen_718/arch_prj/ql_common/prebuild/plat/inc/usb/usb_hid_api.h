#ifndef __USB_HID_API_H__
#define __USB_HID_API_H__

#define HID_DBGEN

#ifdef HID_DBGEN

#define HID_DBG_LOG_INST_NUM 4

void hiddbg_hex(uint32_t hex);
void hiddbg_str(uint8_t* str);
void hiddbg_str_and_hex(uint8_t* str, uint32_t hex);

#ifndef USB_DRV_SMALL_IMAGE
//#define HIDBG_STR(str) ECOMM_TRACE_OPT(UNILOG_PLA_DRIVER, usb_portmon_init_1, P_WARNING, 0, str)

void hid_sdk_test_log_str(char * str);
void hid_sdk_test_log_strhex(char * str,uint32_t hex);


#define HIDBG_STR(str)  hid_sdk_test_log_str(str)


//#define HIDBG_STR_AND_HEX(str, hex) ECOMM_TRACE_OPT(UNILOG_PLA_DRIVER, usb_portmon_init_1, P_WARNING, 1, str, hex)

#define HIDBG_STR_AND_HEX(str, hex) hid_sdk_test_log_strhex(str, hex)


#else
#define HIDBG_STR(str) hiddbg_str((uint8_t*)str)
#define HIDBG_STR_AND_HEX(str, hex) hiddbg_str_and_hex((uint8_t*)str, hex)
#endif
#else
void hiddbg_hex(uint32_t hex);
void hiddbg_str(uint8_t* str);
void hiddbg_str_and_hex(uint8_t* str, uint32_t hex);
#define HIDBG_STR(str) 
#define HIDBG_STR_AND_HEX(str, hex) 
#endif

int hid_chktype_mouse(uint8_t hid_num);
int hid_chktype_keyboard(uint8_t hid_num);


#include "usbd_open_errinfo.h"

#endif

