/** @file         qurl_ctype.c
 *  @brief        Brief description: Determine character type
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-18 16:44:57
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
*/
#include <stdbool.h>
#include "qosa_def.h"
/** 
 *   _U: Uppercase letter
 *   _L: lowercase letters
 *   _N: Decimal digit character
 *   _S: Space character
 *   _P: Punctuation character
 *   _C: Control characters
 *   _X: Hexadecimal character
 *   _B: Blank character
 */
#undef _U
#define _U (1 << 0) /* upper case */
#undef _L
#define _L (1 << 1) /* lower case */
#undef _N
#define _N (1 << 2) /* decimal numerical digit */
#undef _S
#define _S (1 << 3) /* space */
#undef _P
#define _P (1 << 4) /* punctuation */
#undef _C
#define _C (1 << 5) /* control */
#undef _X
#define _X (1 << 6) /* hexadecimal letter */
#undef _B
#define _B (1 << 7) /* blank */

static const unsigned char ascii[128]
    = {_C,      _C,      _C,      _C,      _C,      _C, _C, _C, _C, _C | _S, _C | _S, _C | _S, _C | _S, _C | _S, _C,      _C, _C, _C, _C, _C, _C, _C,
       _C,      _C,      _C,      _C,      _C,      _C, _C, _C, _C, _C,      _S | _B, _P,      _P,      _P,      _P,      _P, _P, _P, _P, _P, _P, _P,
       _P,      _P,      _P,      _P,      _N,      _N, _N, _N, _N, _N,      _N,      _N,      _N,      _N,      _P,      _P, _P, _P, _P, _P, _P, _U | _X,
       _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U, _U, _U, _U, _U,      _U,      _U,      _U,      _U,      _U,      _U, _U, _U, _U, _U, _U, _U,
       _U,      _U,      _U,      _P,      _P,      _P, _P, _P, _P, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L, _L, _L, _L, _L, _L, _L,
       _L,      _L,      _L,      _L,      _L,      _L, _L, _L, _L, _L,      _L,      _L,      _L,      _P,      _P,      _P, _P, _C};

char qurl_ctype_isspace(char c)
{
    if (c < 0)
        return QOSA_FALSE;
    return (ascii[(int)c] & _S);
}

char qurl_ctype_isdigit(char c)
{
    if (c < 0)
        return QOSA_FALSE;
    return (ascii[(int)c] & _N);
}

char qurl_ctype_isalnum(char c)
{
    if (c < 0)
        return QOSA_FALSE;
    return (ascii[(int)c] & (_N | _U | _L));
}

char qurl_ctype_isxdigit(char c)
{
    if (c < 0)
        return QOSA_FALSE;
    return (ascii[(int)c] & (_N | _X));
}

char qurl_ctype_isgraph(char c)
{
    if ((c < 0) || (c == ' '))
        return QOSA_FALSE;
    return (ascii[(int)c] & (_N | _X | _U | _L | _P | _S));
}

char qurl_ctype_isprint(char c)
{
    if (c < 0)
        return QOSA_FALSE;
    return (ascii[(int)c] & (_N | _X | _U | _L | _P | _S));
}

char qurl_ctype_isalpha(char c)
{
    if (c < 0)
        return QOSA_FALSE;
    return (ascii[(int)c] & (_U | _L));
}

char qurl_ctype_isupper(char c)
{
    if (c < 0)
        return QOSA_FALSE;
    return (ascii[(int)c] & (_U));
}

char qurl_ctype_islower(char c)
{
    if (c < 0)
        return QOSA_FALSE;
    return (ascii[(int)c] & (_L));
}

char qurl_ctype_iscntrl(char c)
{
    if (c < 0)
        return QOSA_FALSE;
    return (ascii[(int)c] & (_C));
}
