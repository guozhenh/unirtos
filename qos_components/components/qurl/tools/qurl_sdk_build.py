#!/usr/bin/env python3
# Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
# All rights reserved.
#
# This software is supplied "AS IS" without any warranties.
# RDA assumes no responsibility or liability for the use of the software,
# conveys no license or title under any patent, copyright, or mask work
# right to the product. RDA reserves the right to make changes in the
# software without notification.  RDA also make no representation or
# warranty that such application will be suitable for the specified use
# without further testing or modification.

import argparse
import sys
import io
import os
import shutil
import json
import xml.etree.ElementTree as ET
import glob
from pathlib import Path
import tarfile

DESCRIPTION = """
Compile ohos.
"""

DESCRIPTION_COPYFILE = """
Create deliver pack from XML description.
"""

"""
deliver.xml format

For each XML file:
- src_base: base directory for source paths
- dst_base: base directory for destination paths

Both are usually None. In that case, source paths are resolved from the current
directory and destination paths are resolved from the output root. All source
paths should remain relative to the source base. Destinations may be rooted at
the output directory by setting root="y".

The root tag must be 'deliver'.

When an element supports 'inclusive' and 'exclusive', apply these rules:
- If neither attribute is present, keep the element.
- If no valid 'inclusive' tag matches, skip the element.
- If any valid 'exclusive' tag matches, skip the element.

Supported element types:
1. Copy one file:
   <file src="src" dst="dst" dstdir="dir" root="y" optional="y"
       inclusive="tag,tag" exclusive="tag,tag"/>

   Unless optional="y", the source file must exist.

   Destination priority:
   - Use "dst" when present.
   - Otherwise place the file under "dstdir" when present.
   - Otherwise keep the source-relative directory and filename.

2. Copy files by glob:
   <globfile src="*.h" dstdir="dst" root="y"
       inclusive="tag,tag" exclusive="tag,tag"/>

   Empty glob matches are allowed. Matching directories are ignored. When
   "dstdir" is absent, the source directory is used as the destination.

3. Copy one directory recursively:
   <recursive src="src" dst="dst" root="y" optional="y"
       exclude="name,name" inclusive="tag,tag" exclusive="tag,tag"/>

   When "dst" is absent, the source directory name is reused.

4. Process a subdirectory (not implemented yet):
   <subdir dir="dir" dst="dst" xml="xml" root="y" optional="y"
       inclusive="tag,tag" exclusive="tag,tag"/>

   By default, both the source base and destination base switch into "dir".
   When "dst" is present, the destination base switches into "dst" instead.
   The default XML filename is "deliver.xml".

5. Include another XML file without changing the working directory:
   <include xml="xml" optional="y"
       inclusive="tag,tag" exclusive="tag,tag"/>

6. Group elements that share the same conditions:
   <group inclusive="tag,tag" exclusive="tag,tag">
       <file .../>
       <globfile .../>
       <recursive .../>
       <subdir .../>
       <include .../>
       <group .../>
   </group>
"""


class Deliver(object):

    def __init__(self, itags, etags, kwords, src_base, dst_base):
        self.src_base = src_base
        self.dst_base = dst_base
        self.itags = itags
        self.etags = etags
        self.kwords = kwords
        self.flist = {}

    def loadxml(self, fname):
        with open(fname, 'r') as fh:
            fdata = fh.read()

        root = ET.fromstring(fdata.format(**self.kwords))
        if root.tag != 'deliver':
            raise Exception('invalid deliver xml')

        for elem in list(root):
            self._load_elem(elem)

    def _load_elem(self, elem):
        if not self._check_tags(elem):
            return

        if elem.tag == 'file':
            self._load_file(elem)
        elif elem.tag == 'globfile':
            self._load_globfile(elem)
        elif elem.tag == 'recursive':
            self._load_recursive(elem)
        elif elem.tag == 'subdir':
            self._load_subdir(elem)
        elif elem.tag == 'include':
            self._load_include(elem)
        elif elem.tag == 'group':
            self._load_group(elem)
#        elif elem.tag == 'copydir':
#            self._load_copydir(elem)
        else:
            raise Exception('invalid deliver xml tag' + elem.tag)

    def _add_file(self, src, dst):
        if dst in self.flist and src != self.flist[dst]:
            raise Exception('conflict ' + dst)

        self.flist[dst] = src

    def _check_tags(self, elem):
        itags = []
        etags = []
        if elem.get('inclusive'):
            itags = elem.get('inclusive').split(',')
        if elem.get('exclusive'):
            etags = elem.get('exclusive').split(',')

        if not itags and not etags:
            return True

        checked = False
        if self.itags:
            for tag in itags:
                if tag in self.itags:
                    checked = True
                    break

        if not checked:
            return False

        if self.etags:
            for tag in etags:
                if tag in self.etags:
                    return False

        return True

    def _load_group(self, elem):
        for subelem in list(elem):
            self._load_elem(subelem)

    def _build_dst(self, dst, dstroot):
        if dstroot or not self.dst_base:
            return dst

        return self.dst_base + '/' + dst

    def _build_src(self, src):
        if not self.src_base:
            return src

        return self.src_base + '/' + src

    def _load_file(self, elem):
        optional = elem.get('optional', 'n') == 'y'
        dstroot = elem.get('root', 'n') == 'y'

        src = elem.get('src')
        if src is None:
            raise Exception('src is missed in file element')

        src = os.path.normpath(src.replace('\\', '/'))

        if elem.get('dst'):
            dst = self._build_dst(elem.get('dst'), dstroot)
        elif elem.get('dstdir'):
            dst = self._build_dst(elem.get('dstdir'),
                                  dstroot) + '/' + Path(src).name
        else:
            dst = self._build_dst(src, False)

        src = self._build_src(src)
        if not optional and not os.path.exists(src):
            raise Exception(src + ' doesn\'t exist')

        self._add_file(src, dst)

    def _load_globfile(self, elem):
        src = elem.get('src')
        if src is None:
            raise Exception('src is missed in globfile element')

        src = os.path.normpath(src.replace('\\', '/'))
        dstroot = elem.get('root', 'n') == 'y'

        if elem.get('dstdir'):
            dstdir = self._build_dst(elem.get('dstdir'), dstroot)
        else:
            dstdir = self._build_dst(str(Path(src).parent), False)

        src = self._build_src(src)
        srcdir = str(Path(src).parent)
        for fname in glob.glob(src):
            name = Path(fname).name
            srcfname = srcdir + '/' + name
            dstfname = dstdir + '/' + name
            if not os.path.isfile(srcfname):
                continue

            self._add_file(srcfname, dstfname)

    def _load_recursive(self, elem):
        excludes = []
        if elem.get('exclude'):
            excludes = elem.get('exclude').split(',')

        src = elem.get('src')
        if src is None:
            raise Exception('src is missed in recursive element')

        src = os.path.normpath(src.replace('\\', '/'))
        dstroot = elem.get('root', 'n') == 'y'

        if elem.get('dst'):
            dst = self._build_dst(elem.get('dst'), dstroot)
        else:
            dst = self._build_dst(src, False)

        src = self._build_src(src)
        self._load_recursive_dir(src, dst, excludes)

    def _load_recursive_dir(self, src, dst, excludes):
        for child in os.listdir(src):
            if excludes and child in excludes:
                continue

            srcname = src + '/' + child
            dstname = dst + '/' + child
            if os.path.isfile(srcname):
                self._add_file(srcname, dstname)
            elif os.path.isdir(srcname):
                self._load_recursive_dir(srcname, dstname, excludes)

    def _load_subdir(self, elem):
        pass

    def _load_include(self, elem):
        src = elem.get("xml")
        if src is None:
            raise Exception('xml is missed in include element')

        fname = self._build_src(src)
        optional = elem.get('optional', 'n') == 'y'
        if not optional and not os.path.exists(fname):
            raise Exception(fname + ' doesn\'t exist')

        deliver = Deliver(self.itags, self.etags, self.kwords)
        deliver.src_base = self.src_base
        deliver.dst_base = self.dst_base
        deliver.loadxml(fname)

        for dst, src in deliver.flist.items():
            self._add_file(src, dst)

    def _load_copydir(self, elem):
        src = elem.get('src')
        if src is None:
            raise Exception('src is missed in copydir element')

        src = os.path.normpath(src.replace('\\', '/'))
        dstroot = elem.get('root', 'n') == 'y'

        if elem.get('dstdir'):
            dstdir = self._build_dst(elem.get('dstdir'), dstroot)
        else:
            dstdir = self._build_dst(str(Path(src).parent), False)

        src = self._build_src(src)
        srcdir = str(Path(src).parent)
        shutil.copytree(srcdir, dstdir)


def load_args(sub_parsers):
    parser = sub_parsers.add_parser('load',
                                    help='load description xml to json')
    parser.set_defaults(func=load)
    parser.add_argument('--inclusive', dest="inclusive", nargs="*",
                        help="inclusive tags")
    parser.add_argument('--exclusive', dest="exclusive", nargs="*",
                        help="exclusive tags")
    parser.add_argument('--keyword', dest='keyword', nargs='*',
                        help='keyword used in description')
    parser.add_argument('--src_base', dest='src_base',
                        help='src base')
    parser.add_argument('--dst_base', dest='dst_base',
                        help='dst base')
    parser.add_argument('xml', help='description XML file')
    parser.add_argument('output', help='output json file')


def load(args):
    kwords = {}
    if args.keyword:
        for elem in args.keyword:
            k, v = elem.split('=')
            kwords[k] = v
#            print(k,v)

    deliver = Deliver(args.inclusive, args.exclusive, kwords, args.src_base, args.dst_base)
    deliver.loadxml(args.xml)
    with open(args.output, 'w') as fh:
        json.dump(deliver.flist, fh, sort_keys=True, indent=4)

def pack_args(sub_parsers):
    parser = sub_parsers.add_parser('pack',
                                    help='pack to jamie_pack based on file list json')
    parser.set_defaults(func=pack)
    parser.add_argument('--output', dest='output', default='jamie_output_pack',
                        help='output pack dir name')
    parser.add_argument('json', nargs='+', help='file list json')

def pack(args):
    flist = {}
    
    for fname in args.json:
        with open(fname, 'r') as fh:
            fl = json.loads(fh.read())

        if not fl:
            continue

        for dst, src in fl.items():
            if dst in flist and src != flist[dst]:
                raise Exception('conflict ' + dst)
            flist[dst] = src
    
    for dst in sorted(flist.keys()):
        dst_file=dst
        if not os.path.exists(os.path.dirname(dst_file)):
            os.makedirs(os.path.dirname(dst_file))
        shutil.copyfile(flist[dst], dst_file)

def test_args(sub_parsers):
    parser = sub_parsers.add_parser('test',
                                    help='test func')
    parser.set_defaults(func=test)
    parser.add_argument('--unir', dest="unir", help="unir test")

def test(args):
    print("test unir str:"+args.unir)

def qurl_args_args(sub_parsers):
    parser = sub_parsers.add_parser('qurl_args', help='about dir')
    parser.set_defaults(func=get_qurl_args)
    parser.add_argument('--qurl_top_dir', dest="qurl_top_dir", help="qurl top directory")
    parser.add_argument('--qurl_platform_dir', dest="qurl_platform_dir", help="qurl platform directory")
    parser.add_argument('--qurl_sdk_xml_file', dest="qurl_sdk_xml_file", help="qurl sdk xml file")

def get_qurl_args(args):
    print("--qurl_top_dir ", args.qurl_top_dir)
    print("--qurl_platform_dir ", args.qurl_platform_dir)
    print("--qurl_sdk_xml_file ", args.qurl_sdk_xml_file)
    return args

def ql_pack(argv):
    parser = argparse.ArgumentParser(description=DESCRIPTION_COPYFILE)
    sub_parsers = parser.add_subparsers(help="sub-commands")

    test_args(sub_parsers)
    load_args(sub_parsers)
    pack_args(sub_parsers)

    args = parser.parse_args(argv)
    if args.__contains__('func'):
        return args.func(args)

    parser.parse_args(['-h'])
    return 0

def ql_qurl_args(argv):
    parser = argparse.ArgumentParser(description=DESCRIPTION_COPYFILE)
    sub_parsers = parser.add_subparsers(help="sub-commands")

    qurl_args_args(sub_parsers)

    args = parser.parse_args(argv)
    if args.__contains__('func'):
        return args.func(args)

    parser.parse_args(['-h'])
    return 0

def ql_change_xml_name(xml_path, board_name, product_name):
    # [TODO]need to do
    print("jamie: xml xml_path", xml_path)
    print("jamie: xml board_name", board_name)
    print("jamie: xml product_name", product_name)
    # Update entity definitions.
    ET.entity['board_name'] = board_name
    ET.entity['product_name'] = product_name

    # Parse the XML file.
    tree = ET.parse(xml_path)
    root = tree.getroot()

    # Write the updated ElementTree object back to the XML file.
    tree.write(xml_path)

def main(argv):

    script_path = os.path.abspath(__file__)
    current_dir = os.path.dirname(script_path)
    qurl_top_dir = os.path.abspath(current_dir + '/../')
    qurl_sdk_top_dir = os.path.abspath(current_dir + '/../build/qurl_sdk/')

    if qurl_top_dir[-1] == '\\' or qurl_top_dir[-1] == '/':
        qurl_top_dir = qurl_top_dir[:-1]

    xml_path = qurl_top_dir+'/'+'tools/qurl_sdk_filelist.xml'
    json_path = qurl_top_dir+'/'+'tools/qurl_sdk_filelist.json'
    
    if(len(sys.argv) >= 3):
        print("args", sys.argv)
        qurl_args = ql_qurl_args(argv)
        if(qurl_args.qurl_sdk_xml_file):
            xml_path = qurl_args.qurl_sdk_xml_file
        elif(qurl_args.qurl_platform_dir):
            xml_path = qurl_args.qurl_platform_dir+'/'+'tools/qurl_sdk_filelist.xml'

    sdk_inc_path = qurl_sdk_top_dir + '/' + 'include'
    sdk_libs_path = qurl_sdk_top_dir + '/' + 'libs'

    print("jamie: xml_path", xml_path)

    if not os.path.exists(sdk_inc_path):
        # Create the directory if it does not exist.
        os.makedirs(sdk_inc_path)
    #else:
        # Clear the directory.
    #    shutil.rmtree(sdk_inc_path)

    if not os.path.exists(sdk_libs_path):
        # Create the directory if it does not exist.
        os.makedirs(sdk_libs_path)
    #else:
        # Clear the directory.
    #    shutil.rmtree(sdk_libs_path)
   
    # ql_pack(['test', '--unir', 'hhh'])
    ql_pack(['load', '--src_base', qurl_top_dir, '--dst_base', qurl_sdk_top_dir, '--', xml_path, json_path])
    ql_pack(['pack', '--', json_path])
    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
