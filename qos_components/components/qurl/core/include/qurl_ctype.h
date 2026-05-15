/** @file         qurl_ctype.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-18 16:50:46
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#ifndef QURL_CTYPE_H
#define QURL_CTYPE_H

char qurl_ctype_isspace(char c);

char qurl_ctype_isdigit(char c);

char qurl_ctype_isalnum(char c);

char qurl_ctype_isxdigit(char c);

char qurl_ctype_isgraph(char c);

char qurl_ctype_isprchar(char c);

char qurl_ctype_isalpha(char c);

char qurl_ctype_isupper(char c);

char qurl_ctype_islower(char c);

char qurl_ctype_iscntrl(char c);

#define qurl_ctype_isblank(c) (((c) == ' ') || ((c) == '\t'))

#endif /* Head define end*/
