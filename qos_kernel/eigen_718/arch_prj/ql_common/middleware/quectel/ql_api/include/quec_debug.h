/**
 ******************************************************************************
 * @file    quec_debug.h
 * @author  Chavis.Chen
 * @version V1.0.0
 * @date    26-Sep-2018
 * @brief   This file contains the common definitions, macros and functions to
 *          be shared throughout the project.
 ******************************************************************************
 *
 *  The MIT License
 *  Copyright (c) 2014 QUECTEL Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is furnished
 *  to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 */

#ifndef _QUEC_DEBUG_H
#define _QUEC_DEBUG_H

int ql_trace(const char *fmt, ...);
int ql_dbg_log(const char *fmt, ...);
int ql_warning_log(const char *fmt, ...);
int ql_error_log(const char *fmt, ...);

#define debug_log(msg, ...)	 ql_dbg_log("[QUEC]", msg, ##__VA_ARGS__)


#endif

