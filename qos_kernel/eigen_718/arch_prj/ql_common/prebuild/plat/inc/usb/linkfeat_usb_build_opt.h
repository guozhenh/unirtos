#ifndef __LINK_FEAT_USB_BUILD_OPT_H__
#define __LINK_FEAT_USB_BUILD_OPT_H__
#include "linkeat_usb_build_touch_only.h"


#define LINK_FEAT_USBMOD_BUILD_DEFAULT_T 0xfea00001
#define LINK_FEAT_USBMOD_BUILD_ADDHID_T 0xfea00002
//add this head file to a open-link only file , not built-in private lib file for usb open sourcefile build, 
//source files defined in MakeFeatModUsb.inc libdriver-$(DRIVER_USB_ENABLE)  
//each file need select by feature should include this file
//file list
//open/usbd_multi_usrcfg_custom.o 
//open/function_hid_inherent.o 
//usb_device/usb_hid_test.o 

//by different feat select, built into final elf image



//LINK_FEAT_USBMOD_CUR_BUILD_SEL defined in makefile
//#pragma message "@@@@@@LINK_FEAT_USB_BUILD_OPT: LINK_FEAT_USBMOD_CUR_BUILD_SEL build selecting"

#if defined(CHK_LIBFEAT_USB_BUILD_ADDHID)
#error "LINK_FEAT_USB_BUILD_OPT CHK_LIBFEAT_USB_BUILD_ADDHID defined corrupt, a source file can only include one linkfeat or libfeat head file"
#endif


#if (LINK_FEAT_USBMOD_CUR_BUILD_SEL==LINK_FEAT_USBMOD_BUILD_DEFAULT_T)
#pragma message "@@@@@@LINK_FEAT_USB_BUILD_OPT: LINK_FEAT_USBMOD_BUILD_DEFAULT_T build selected"

#define LINK_FEAT_USBMOD_BUILD_IN_SEL LINK_FEAT_USBMOD_BUILD_DEFAULT_T

#elif (LINK_FEAT_USBMOD_CUR_BUILD_SEL==LINK_FEAT_USBMOD_BUILD_ADDHID_T)
#pragma message "@@@@@@LINK_FEAT_USB_BUILD_OPT: LINK_FEAT_USBMOD_BUILD_ADDHID_T build selected"

#define LINK_FEAT_USBMOD_BUILD_IN_SEL LINK_FEAT_USBMOD_BUILD_ADDHID_T
#else

#if defined(LINK_FEAT_USBMOD_CUR_BUILD_SEL)
#error "LINK_FEAT_USB_BUILD_OPT: LINK_FEAT_USBMOD_CUR_BUILD_SEL="#LINK_FEAT_USBMOD_CUR_BUILD_SEL" not valid, compile error"

#else
#error "LINK_FEAT_USB_BUILD_OPT: LINK_FEAT_USBMOD_CUR_BUILD_SEL not defined, compile error"
#endif

#endif

//logs code like ECOMM_TRACE can be included in this branch code in open source files using link feat, comdb.txt will be updated every build
#define CHK_LINK_FEAT_USB_BUILD_ADDHID ((LINK_FEAT_USBMOD_BUILD_IN_SEL)==(LINK_FEAT_USBMOD_BUILD_ADDHID_T))

#endif

