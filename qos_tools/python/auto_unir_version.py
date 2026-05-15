# -*- coding: utf-8 -*-

import sys

def generate_unirtos_version_h(params):
    header = '''/************************* This file is modify automaticly, please don't edit it! ***************************/
#ifndef UNIRTOS_VERSION_H
#define UNIRTOS_VERSION_H

'''
    footer = '''
#endif
'''

    content = header
    content += '#define UNIRTOS_VERSION_AUTO_GENERATE\n\n'
    content += 'const char unirtos_mob_model_id[]="{}";\n'.format(params[0])
    content += 'const char unirtos_mob_sw_rev[]="{}";\n'.format(params[1])
    content += 'const char unirtos_mob_sw_subrev[]="{}";\n'.format(params[2])
    content += 'const char unirtos_mob_sw_qinfrev[]="{}";\n'.format(params[3])
    content += 'const char unirtos_mob_imeisv_svn[]="{}";\n'.format(params[4])
    content += 'const char unirtos_mob_intra_sw_rev[]="{}";\n'.format(params[1])
    content += 'const char unirtos_mob_usb_manufacturer[]="Quectel";\n'
    content += 'const char unirtos_mob_usb_product[]="{}";\n'.format(params[0])
    content += 'const char unirtos_mob_sw_ap_rev[]="";\n'
    content += 'const unsigned char unirtos_tools_mob_sw_ap_rev[128]="UNIRTOS_TOOLS_VER_CHECK: {}";\n\n'.format(params[1])
    content += 'const char unirtos_ver_date[] = __DATE__;\n'
    content += 'const char unirtos_ver_time[] = __TIME__;\n'
    content += 'const char unirtos_authors[] = "Authors: QCT";\n'

    content += footer

    with open('unirtos_version.h', 'w') as file:
        file.write(content)

if __name__ == "__main__":
    if len(sys.argv) != 6:
        print("Usage: python script.py <mob_model_id> <mob_sw_rev> <mob_sw_subrev> <mob_sw_qinfrev> <mob_imeisv_svn> <mob_intra_sw_rev> <mob_usb_manufacturer> <mob_usb_product> <mob_sw_ap_rev>")
    else:
        generate_unirtos_version_h(sys.argv[1:])


#usage：python generate_version_h.py EC800E EC800ECNLCR01A01M04_BETA_0118_V04_L V01 FFF 01
#                                     0          1                             2   3   4
