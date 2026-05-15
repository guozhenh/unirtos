#ifndef __FUNC_HID_CONF_H__
#define  __FUNC_HID_CONF_H__


//#include "usbc_ctrl_config.h"


#define USBD_CFG_MAX_NUM                1

#define HID_REQ_GET_REPORT 0x1
#define HID_REQ_GET_IDLE 0x2
#define HID_REQ_GET_PROTOCOL 0x3

#define HID_REQ_SET_REPORT 0x9
#define HID_REQ_SET_IDLE 0xA
#define HID_REQ_SET_PROTOCOL 0xB

#define HID_REPORT_INPUT 0x1
#define HID_REPORT_OUTPUT 0x2
#define HID_REPORT_FEATURE 0x3


#define HID_CLSDT_HID_TP 0x21
#define HID_CLSDT_REPORT_TP 0x22
#define HID_CLSDT_PHYSICAL_TP 0x23

#define HID_CLSDT_HID_LEN 9


#ifdef USE_USBC_CTRL_HS
//Up to 1024, here set 64 
 #define HID_DATA_MAX_PACKET_SIZE       64  



 #define HID_ITRIEP_TXBUF_MAX_SIZE               128

// #define HID_ITROEP_RXBUF_MAX_SIZE               1024 
//reference to u_serial driver align to max packet size
 #define HID_ITROEP_RXBUF_MAX_SIZE               64 
 
 
#else
 #define HID_DATA_MAX_PACKET_SIZE       64   
 #define HID_ITRIEP_TXBUF_MAX_SIZE               128 
 
#endif /* USE_USBC_CTRL_HS */


#endif


