/** @file         qurl_prot.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-19 17:47:16
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#ifndef QURL_PROT_H
#define QURL_PROT_H

#define QURL_PROT_OPT_NONE            0        /* nothing extra */
#define QURL_PROT_OPT_SSL             (1 << 0) /* uses SSL */
#define QURL_PROT_OPT_DUAL            (1 << 1) /* this protocol uses two connections */
#define QURL_PROT_OPT_CLOSEACTION     (1 << 2) /* need action before socket close */
/* some protocols will have to call the underlying functions without regard to
   what exact state the socket signals. IE even if the socket says "readable",
   the send function might need to be called while uploading, or vice versa.
*/
#define QURL_PROT_OPT_DIRLOCK         (1 << 3)
#define QURL_PROT_OPT_NONETWORK       (1 << 4)  /* protocol doesn't use the network! */
#define QURL_PROT_OPT_NEEDSPWD        (1 << 5)  /* needs a password, and if none is set it gets a default */
#define QURL_PROT_OPT_NOURLQUERY      (1 << 6)  /* protocol can't handle url query strings (?foo=bar) ! */
#define QURL_PROT_OPT_CREDSPERREQUEST (1 << 7)  /* requires login credentials per request instead of per connection */
#define QURL_PROT_OPT_ALPN_NPN        (1 << 8)  /* set ALPN and/or NPN for this */
#define QURL_PROT_OPT_STREAM          (1 << 9)  /* a protocol with individual logical streams */
#define QURL_PROT_OPT_URLOPTIONS      (1 << 10) /* allow options part in the userinfo field of the URL */
#define QURL_PROT_OPT_PROXY_AS_HTTP   (1 << 11) /* allow this non-HTTP scheme over a HTTP proxy as HTTP proxies may know this protocol and act as a gateway */
#define QURL_PROT_OPT_WILDCARD        (1 << 12) /* protocol supports wildcard matching */
#define QURL_PROT_OPT_USERPWDCTRL     (1 << 13) /* Allow "control bytes" (< 32 ascii) in user name and password */

#endif                                          /* Head define end*/
