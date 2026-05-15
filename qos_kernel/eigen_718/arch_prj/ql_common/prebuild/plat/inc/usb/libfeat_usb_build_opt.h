#ifndef __LIBFEAT_USB_BUILD_OPT_H__
#define __LIBFEAT_USB_BUILD_OPT_H__
//add this head file to source for libfeat pirvate usb lib build, 
//source files defined in MakeFeatModUsb.inc libusbf_dev_priv_sobj-$(DRIVER_USB_ENABLE)    
//each file need select by feature should include this file
//file list
//usb_device/usbd_clsdev.o  
//usb_device/usbd_funclib.o 
//usb_device/usbd_multi.o 
//usb_device/usbd_multi_usrcfg_lib.o 
//usb_device/device_vcom.o 
//usb_device/device_hid.o 
//usb_device/device_rndis.o 
//usb_device/device_ecm.o 
//usb_device/device_ccid.o 
//usb_device/func_dbg.o
//usb_device/function_vcom_tpl.o 
//usb_device/function_hid_tpl.o 
//usb_device/function_rndis.o 
//usb_device/function_ecm.o 
//usb_device/function_ccid.o 
//usb_device/usb_hid_api.o     
//by different feat select, built into libfeat_USBMOD_FEAT_ADDHID.a or libfeat_USBMOD_FEAT_DEFAULT.a


#define LIBFEAT_USBMOD_BUILD_DEFAULT_T 0xfea00001
#define LIBFEAT_USBMOD_BUILD_ADDHID_T 0xfea00002


//LIBFEAT_USBMOD_CUR_BUILD_SEL defined in makefile
#pragma message "@@@@@@LIBFEAT_USBMOD_BUILD_IN_SEL: LIBFEAT_USBMOD_CUR_BUILD_SEL build selecting"

#if defined(CHK_LINK_FEAT_USB_BUILD_ADDHID)
#error "LIBFEAT_USB_BUILD_OPT CHK_LINK_FEAT_USB_BUILD_ADDHID defined corrupt, a source file can only include one linkfeat or libfeat head file"
#endif


#if (LIBFEAT_USBMOD_CUR_BUILD_SEL==LIBFEAT_USBMOD_BUILD_DEFAULT_T)
#pragma message "@@@@@@LIBFEAT_USB_BUILD_OPT: LIBFEAT_USBMOD_BUILD_DEFAULT_T build selected"

#define LIBFEAT_USBMOD_BUILD_IN_SEL LIBFEAT_USBMOD_BUILD_DEFAULT_T

#elif (LIBFEAT_USBMOD_CUR_BUILD_SEL==LIBFEAT_USBMOD_BUILD_ADDHID_T)
#pragma message "@@@@@@LIBFEAT_USB_BUILD_OPT: LIBFEAT_USBMOD_BUILD_ADDHID_T build selected"

#define LIBFEAT_USBMOD_BUILD_IN_SEL  LIBFEAT_USBMOD_BUILD_ADDHID_T
#else

#if defined(LIBFEAT_USBMOD_CUR_BUILD_SEL)
#error "LIBFEAT_USB_BUILD_OPT: LIBFEAT_USBMOD_CUR_BUILD_SEL="#LIBFEAT_USBMOD_CUR_BUILD_SEL" not valid, compile error"

#else
#error "LIBFEAT_USB_BUILD_OPT: LIBFEAT_USBMOD_CUR_BUILD_SEL not defined, compile error"
#endif

#endif

//logs code like ECOMM_TRACE should no be included in this branch code, may cause comdblib.txt not match for different libfeat
//or use if (CHK_LIBFEAT_USB_BUILD_ADDHID) instead, not #if CHK_LIBFEAT_USB_BUILD_ADDHID
#define CHK_LIBFEAT_USB_BUILD_ADDHID ((LIBFEAT_USBMOD_BUILD_IN_SEL)==(LIBFEAT_USBMOD_BUILD_ADDHID_T))

#endif
