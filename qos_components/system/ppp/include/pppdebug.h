/*****************************************************************************
* pppdebug.h - System debugging utilities.
*
* Copyright (c) 2003 by Marc Boucher, Services Informatiques (MBSI) inc.
* portions Copyright (c) 1998 Global Election Systems Inc.
* portions Copyright (c) 2001 by Cognizant Pty Ltd.
*
* The authors hereby grant permission to use, copy, modify, distribute,
* and license this software and its documentation for any purpose, provided
* that existing copyright notices are retained in all copies and that this
* notice and the following disclaimer are included verbatim in any
* distributions. No written agreement, license, or royalty fee is required
* for any of the authorized uses.
*
* THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS *AS IS* AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
* REVISION HISTORY (please don't use tabs!)
*
* 03-01-01 Marc Boucher <marc@mbsi.ca>
*   Ported to lwIP.
* 98-07-29 Guy Lancaster <lancasterg@acm.org>, Global Election Systems Inc.
*   Original.
*
*****************************************************************************
*/

#include "ppp_opts.h"
#include "qosa_log.h"

#if PPP_SUPPORT /* don't build if not configured for use in lwipopts.h */

#ifndef PPPDEBUG_H
#define PPPDEBUG_H

#ifdef __cplusplus
extern "C"
{
#endif

#define QOS_LOG_TAG   LOG_TAG_QPPP

/* Trace levels. */
#define LOG_CRITICAL  (QOSA_LOG_LEVEL_ERROR)
#define LOG_ERR       (QOSA_LOG_LEVEL_ERROR)
#define LOG_NOTICE    (QOSA_LOG_LEVEL_WARN)
#define LOG_WARNING   (QOSA_LOG_LEVEL_WARN)
#define LOG_INFO      (QOSA_LOG_LEVEL_INFO)
#define LOG_DETAIL    (QOSA_LOG_LEVEL_DEBUG)
#define LOG_DEBUG     (QOSA_LOG_LEVEL_VERBOSE)

#define PPP_LOG_LEVEL LOG_DEBUG

#define XXX_DEBUGF(a, b)                                                                                                                                       \
    do {                                                                                                                                                       \
        if (PPP_LOG_LEVEL >= a)                                                                                                                                \
        {                                                                                                                                                      \
            QLOGI b;                                                                                                                                           \
        }                                                                                                                                                      \
    } while (0)

#if PPP_DEBUG

#define MAINDEBUG(a)   XXX_DEBUGF(LOG_WARNING, a)
#define SYSDEBUG(a)    XXX_DEBUGF(LOG_WARNING, a)
#define FSMDEBUG(a)    XXX_DEBUGF(LOG_WARNING, a)
#define LCPDEBUG(a)    XXX_DEBUGF(LOG_WARNING, a)
#define IPCPDEBUG(a)   XXX_DEBUGF(LOG_WARNING, a)
#define IPV6CPDEBUG(a) XXX_DEBUGF(LOG_WARNING, a)
#define UPAPDEBUG(a)   XXX_DEBUGF(LOG_WARNING, a)
#define CHAPDEBUG(a)   XXX_DEBUGF(LOG_WARNING, a)
#define PPPDEBUG(a, b) XXX_DEBUGF(a, b)

#else /* PPP_DEBUG */

#define MAINDEBUG(a)
#define SYSDEBUG(a)
#define FSMDEBUG(a)
#define LCPDEBUG(a)
#define IPCPDEBUG(a)
#define IPV6CPDEBUG(a)
#define UPAPDEBUG(a)
#define CHAPDEBUG(a)
#define PPPDEBUG(a, b)

#endif /* PPP_DEBUG */

#ifdef __cplusplus
}
#endif

#endif /* PPPDEBUG_H */

#endif /* PPP_SUPPORT */
