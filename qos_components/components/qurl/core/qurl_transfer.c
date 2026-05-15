/** @file         qurl_transfer.c
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-06 17:21:17
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#include "qurl_config.h"
#include "qurl_transfer.h"
#include "qurl_log.h"
#include "qurl_url.h"
#include "qurl_slist.h"
#include "qurl_port.h"
#include "qurl_default_basic.h"

#if (QURL_CFG_ENABLE_FS == 0)
#include "qurl_default_fs.h"
#endif

#if QURL_CFG_ENABLE_HTTP
#include "qurl_http_def.h"
#endif

/**
 * @brief Transmission initialization executed upon entering the initial state QURL_STATE_INIT of the state machine.
 * 1. Verify the user-provided configuration.
 * 2. Initialize some necessary parameters.
 * 3. Parse URL.
 * The purpose is to prepare for normal transmission later.
 * @note This function is executed only once per state machine cycle; it will not be executed again even if redirection or multiple authentications occur during the transmission process.
 * @return Return the ecode
 */
qurl_ecode_t qurl_trans_init(qurl_core_ctrl_t *core_ptr)
{
    qurl_ecode_t ret = QURL_OK;
    long         len = 0;

    /*URL Handling:
	 * If no new URL is set this time, then extract the old URL. The judgment logic needs to be combined with url_alloc_flag.
	 */

    if (!core_ptr->usr_cfg.url_ptr && !core_ptr->cfg.uurl_ptr)
    {
        return QURL_ECODE_NO_URL;
    }
    if (core_ptr->cfg.uurl_ptr) /*Set a new URL or have not yet parsed the URL*/
    {
        qurl_url_delete(core_ptr->cfg.uurl_ptr);
        core_ptr->cfg.uurl_ptr = QOSA_NULL;
    }
    if (core_ptr->cfg.bits.url_alloc_flag)
    {
        qurl_free(core_ptr->cfg.url_ptr);
        core_ptr->cfg.url_ptr = QOSA_NULL;
        core_ptr->cfg.bits.url_alloc_flag = QOSA_FALSE;
    }
    if (core_ptr->usr_cfg.url_ptr != core_ptr->cfg.url_ptr)
    {
        core_ptr->cfg.url_ptr = core_ptr->usr_cfg.url_ptr;
    }

    ret = qurl_url_create(&core_ptr->cfg.uurl_ptr);
    if (!core_ptr->cfg.uurl_ptr)
    {
        return ret;
    }
    ret = qurl_url_setopt(
        core_ptr->cfg.uurl_ptr,
        QURL_URL_OPT_URL,
        core_ptr->cfg.url_ptr,
        QURL_URL_GUESS_SCHEME | QURL_URL_NON_SUPPORT_SCHEME | (core_ptr->usr_cfg.bits.disallow_username_in_url ? QURL_URL_DISALLOW_USER : 0)
            | (core_ptr->usr_cfg.bits.path_as_is ? QURL_URL_PATH_AS_IS : 0)
    );
    if (ret != QURL_OK)
    {
        qurl_url_delete(core_ptr->cfg.uurl_ptr);
        core_ptr->cfg.uurl_ptr = QOSA_NULL;
        return ret;
    }
    /** Update the port number in uurl*/
    if (core_ptr->usr_cfg.port > 0)
    {
        char portbuf[8] = {0};

        qurl_snprintf(portbuf, sizeof(portbuf), "%ld", core_ptr->usr_cfg.port);
        ret = qurl_url_setopt(core_ptr->cfg.uurl_ptr, QURL_URL_OPT_PORT, portbuf, 0);
        if (ret != QURL_OK)
        {
            qurl_url_delete(core_ptr->cfg.uurl_ptr);
            core_ptr->cfg.uurl_ptr = QOSA_NULL;
            return ret;
        }
    }

    /*Transmission Related*/
    /** General*/
    core_ptr->cfg.resume_from = core_ptr->usr_cfg.resume_from;
    /** Is there a range*/
    if (core_ptr->usr_cfg.resume_from || core_ptr->usr_cfg.range_ptr)
    {
        /*resume_from priority*/
        if (core_ptr->cfg.bits.range_alloc_flag && core_ptr->cfg.range_ptr)
        {
            qurl_free(core_ptr->cfg.range_ptr);
            core_ptr->cfg.range_ptr = QOSA_NULL;
        }
        if (core_ptr->usr_cfg.resume_from)
        {
            qurl_asprintf(&core_ptr->cfg.range_ptr, "%d-", core_ptr->usr_cfg.resume_from);
            if (core_ptr->cfg.range_ptr == QOSA_NULL)
            {
                return QURL_ECODE_NO_MEMORY;
            }
            core_ptr->cfg.bits.range_alloc_flag = QOSA_TRUE;
        }
        else
        {
            core_ptr->cfg.range_ptr = core_ptr->usr_cfg.range_ptr;
            core_ptr->cfg.bits.range_alloc_flag = QOSA_FALSE;
        }

        core_ptr->cfg.bits.use_range = QOSA_TRUE;
    }
    else
    {
        core_ptr->cfg.bits.use_range = QOSA_FALSE;
    }
    core_ptr->cfg.bits.prefer_ascii = core_ptr->usr_cfg.bits.prefer_ascii;
    core_ptr->cfg.bits.remote_append = core_ptr->usr_cfg.bits.remote_append;
    /** Download*/
    if (core_ptr->usr_cfg.max_filesize > 0)
    {
        /*[lzm][note]This can consider not using max_filesize. Because the format may affect the size*/
        core_ptr->cfg.download_max = core_ptr->usr_cfg.max_filesize;
    }
    else
    {
        core_ptr->cfg.download_max = -1; /*Unrestricted*/
    }

#if QURL_CFG_ENABLE_HTTP
    core_ptr->cfg.http.http_method = core_ptr->usr_cfg.http.http_method;
    /*[lzm][todo]To be corrected for specific method*/
    if (core_ptr->usr_cfg.http.http_method <= QURL_HTTP_METHOD_NONE)
    {
        core_ptr->cfg.http.http_method = QURL_HTTP_METHOD_GET;
    }
#endif /* QURL_CFG_ENABLE_HTTP */
    /**
	 * By default, the use of user-specified port numbers is allowed.
	 * If a redirection occurs and the new Location points to a different port number, simply disallow the use of the user-specified port number.
	 */
    core_ptr->cfg.bits.allow_port = QOSA_TRUE;

    /** Transmission buffer section*/
    /*** Clear the transmit and receive buffers*/
    qurl_dbuf_clear_data(core_ptr->send_cache);
    qurl_dbuf_clear_data(core_ptr->recv_cache);

    /*Some header fields are being prepared for processing*/
    /** Processing username and password*/
    if (core_ptr->usr_cfg.username_ptr)
    {
        len = qurl_strlen(core_ptr->usr_cfg.username_ptr);
        if (core_ptr->cfg.user != QOSA_NULL)
        {
            qurl_free(core_ptr->cfg.user);
            core_ptr->cfg.user = QOSA_NULL;
        }
        core_ptr->cfg.user = qurl_malloc(len + 1);
        if (core_ptr->cfg.user)
        {
            core_ptr->cfg.user[len] = 0x00;
            qurl_memcpy(core_ptr->cfg.user, core_ptr->usr_cfg.username_ptr, len);
        }
    }
    if (core_ptr->usr_cfg.password_ptr)
    {
        len = qurl_strlen(core_ptr->usr_cfg.password_ptr);
        if (core_ptr->cfg.passwd != QOSA_NULL)
        {
            qurl_free(core_ptr->cfg.passwd);
            core_ptr->cfg.passwd = QOSA_NULL;
        }
        core_ptr->cfg.passwd = qurl_malloc(len + 1);
        if (core_ptr->cfg.passwd)
        {
            core_ptr->cfg.passwd[len] = 0x00;
            qurl_memcpy(core_ptr->cfg.passwd, core_ptr->usr_cfg.password_ptr, len);
        }
    }

    /*Data transmission section*/
    /** head */
    core_ptr->cfg.bits.upload_head_raw = core_ptr->usr_cfg.bits.upload_head_raw;
    core_ptr->cfg.upload_head_size = 0;
    if (core_ptr->usr_cfg.upload_head_data_ptr)
    {
        if (core_ptr->usr_cfg.upload_head_size < 0)
        {
            ret = QURL_ECODE_PARAM_INVALID;
            goto exit;
        }
        core_ptr->cfg.upload_head_size = core_ptr->usr_cfg.upload_head_size;
    }
    else if (core_ptr->usr_cfg.upload_head_file_ptr)
    {
        core_ptr->cfg.upload_head_file_fd = qurl_fopen(core_ptr->usr_cfg.upload_head_file_ptr, QURL_O_RDONLY);
        if (core_ptr->cfg.upload_head_file_fd < 0)
        {
            ret = QURL_ECODE_FS_OPEN_ERR;
            goto exit;
        }
        len = qurl_fgetsize(core_ptr->cfg.upload_head_file_fd);
        if (len < 0)
        {
            ret = QURL_ECODE_FS_READ_ERR;
            goto exit;
        }
        else
        {
            core_ptr->cfg.upload_head_size = len;
        }
        qurl_loge("[%d][%ld][%s]\r\n", core_ptr->cfg.upload_head_file_fd, core_ptr->cfg.upload_head_size, core_ptr->usr_cfg.upload_head_file_ptr);
    }
    else if (core_ptr->usr_cfg.read_head_from_client_func)
    {
        if (core_ptr->usr_cfg.upload_head_size == 0)
        {
            ret = QURL_ECODE_PARAM_INVALID;
            goto exit;
        }
        core_ptr->cfg.upload_head_size = core_ptr->usr_cfg.upload_head_size;
    }

    /* body */
    /** Confirm the size of the data to be uploaded*/
    /*** [lzm][todo]What still needs to be developed here is that when only a filename is passed, we need to operate here to determine the file size and then confirm the upload_size.
	 *   Of course, there is also the development of chunked transmission.
	 */
#if QURL_CFG_ENABLE_HTTP
    if (core_ptr->usr_cfg.http.http_method == QURL_HTTP_METHOD_POST_FORM)
    {
        ret = qurl_http_multiform_trans_init(&core_ptr->usr_cfg.http.multiform_ctrl);
        if (ret != QURL_OK)
        {
            goto exit;
        }
    }
    else if (core_ptr->usr_cfg.http.http_method == QURL_HTTP_METHOD_PUT_FORM)
    {
        ret = qurl_http_multiform_trans_init(&core_ptr->usr_cfg.http.multiform_ctrl);
        if (ret != QURL_OK)
        {
            goto exit;
        }
    }
    else
#endif /* QURL_CFG_ENABLE_HTTP */
    {
        core_ptr->cfg.upload_size = 0;
        if (core_ptr->usr_cfg.upload_data_ptr)
        {
            core_ptr->cfg.upload_size = core_ptr->usr_cfg.upload_size;
            core_ptr->cfg.bits.upload = QOSA_TRUE;
        }
        else if (core_ptr->usr_cfg.upload_file_ptr)
        {
            core_ptr->cfg.upload_file_fd = qurl_fopen(core_ptr->usr_cfg.upload_file_ptr, QURL_O_RDONLY);
            if (core_ptr->cfg.upload_file_fd < 0)
            {
                ret = QURL_ECODE_FS_OPEN_ERR;
                goto exit;
            }
            len = qurl_fgetsize(core_ptr->cfg.upload_file_fd);
            if (len < 0)
            {
                core_ptr->cfg.bits.upload = QOSA_FALSE;
            }
            else
            {
                core_ptr->cfg.upload_size = len;
                core_ptr->cfg.bits.upload = QOSA_TRUE;
            }
            qurl_logd("[%d][%ld][%s]\r\n", core_ptr->cfg.upload_file_fd, core_ptr->cfg.upload_size, core_ptr->usr_cfg.upload_file_ptr);
        }
        else if (core_ptr->usr_cfg.read_from_client_func)
        {
            core_ptr->cfg.upload_size = core_ptr->usr_cfg.upload_size;
            core_ptr->cfg.bits.upload = QOSA_TRUE;
        }
        else
        {
            core_ptr->cfg.bits.upload = QOSA_FALSE;
        }
    }
#if QURL_CFG_ENABLE_HTTP
    core_ptr->cfg.http.http_resp_headers_slist = QOSA_NULL;
#endif /* QURL_CFG_ENABLE_HTTP */

    /*info Initialization*/
    core_ptr->info.content_length = -1; /*By default, the specified length has not been received.*/
    core_ptr->info.start_pos = 0;

    /*Wildcard*/
    core_ptr->cfg.bits.wildcard_match = core_ptr->usr_cfg.bits.wildcard_match;
    if (core_ptr->cfg.bits.wildcard_match)
    {
        /*[lzm][todo] Create wildcard control block*/
    }

    /*Transmission Record Initialization*/
    /** Redirection*/
    core_ptr->trans.redirs_cnt = 0;
    /** Download*/
    core_ptr->trans.download_len_max = 0;
    core_ptr->trans.downloaded_len = 0;
    /** Upload*/
    core_ptr->trans.upload_head_len_max = 0;
    core_ptr->trans.uploaded_head_len = 0;
    core_ptr->trans.upload_len_max = 0;
    core_ptr->trans.uploaded_len = 0;
    /** User Side*/
    core_ptr->trans.client_w_len = 0;
    /** bits */
    core_ptr->trans.bits.chunked = 0;

    return ret;

exit:
    if (core_ptr->cfg.user)
    {
        qurl_free(core_ptr->cfg.user);
        core_ptr->cfg.user = QOSA_NULL;
    }
    if (core_ptr->cfg.passwd)
    {
        qurl_free(core_ptr->cfg.passwd);
        core_ptr->cfg.passwd = QOSA_NULL;
    }
    return ret;
}

/**
 * @brief Operation completed. (Regardless of success or failure)
 * @note Perform some deinitialization operations. Ensure that it does not affect the client's operations between two perform calls.
 * @note The transmission completion is executed only once; neither retries nor redirects during the process will trigger this operation. This aligns with the qurl_trans_init() function.
 * @note qurl_trans_init has been executed; all other states must exit through this function.
 * @return Return the ecode
 */
qurl_ecode_t qurl_trans_completed(qurl_core_ctrl_t *core_ptr)
{
    qurl_logd("enter\r\n");

    /*Some files need to be closed after opening.*/
#if QURL_CFG_ENABLE_HTTP
    /** HTTP multi-form*/
    qurl_http_multiform_trans_deinit(&core_ptr->usr_cfg.http.multiform_ctrl);
#endif /* QURL_CFG_ENABLE_HTTP */

    /** Uploaded file*/
    if (core_ptr->cfg.upload_head_file_fd >= 0)
    {
        qurl_fclose(core_ptr->cfg.upload_head_file_fd);
        core_ptr->cfg.upload_head_file_fd = -1;
    }
    if (core_ptr->cfg.upload_file_fd >= 0)
    {
        qurl_fclose(core_ptr->cfg.upload_file_fd);
        core_ptr->cfg.upload_file_fd = -1;
    }

    return QURL_OK;
}

/**
 * @return Returns a pointer to the first matching header or NULL if none matched.
 */
char *qurl_trans_check_headers(qurl_conn_t *conn_ptr, const char *header)
{
    qurl_slist_ctrl_t *node = QOSA_NULL;
    int                len = qurl_strlen(header);
    qurl_core_ctrl_t  *core_ptr = conn_ptr->core_ptr;

    QURL_UNUSED(node);
    QURL_UNUSED(len);
    QURL_UNUSED(core_ptr);

#if QURL_CFG_ENABLE_HTTP
    for (node = core_ptr->usr_cfg.http.http_headers_slist; node; node = node->next)
    {
        if (!qurl_strncasecmp(node->data, header, len) && qurl_trans_headersep(((char *)(node->data))[len]))
        {
            return (char *)(node->data);
        }
    }
#endif /* QURL_CFG_ENABLE_HTTP */

    return QOSA_NULL;
}

qurl_ecode_t qurl_trans_follow(qurl_core_ctrl_t *core_ptr, char *new_url, qurl_trans_followtype_e type)
{
    qurl_ecode_t ret = QURL_OK;
    qurl_bit_t   reachedmax = QOSA_FALSE;
    qurl_bit_t   disallowport = QOSA_FALSE;

    qurl_logd("enter\r\n");

    /*Pre-processing*/
    if (type == QURL_TRANS_FOLLOWTYPE_REDIR)
    {
        if ((core_ptr->usr_cfg.redirs_cnt_max > -1) && (core_ptr->trans.redirs_cnt >= core_ptr->usr_cfg.redirs_cnt_max))
        {
            reachedmax = QOSA_TRUE;
            type = QURL_TRANS_FOLLOWTYPE_FAKE;
        }
        else
        {
            core_ptr->trans.redirs_cnt++;

            /*[lzm][note]Consider whether to provide the HTTP Referer field here*/
            if (core_ptr->usr_cfg.bits.http_auto_referer)
            {
                qurl_url_t *uurl_ptr = QOSA_NULL;
                char       *referer_ptr = QOSA_NULL;

                ret = qurl_url_create(&uurl_ptr);
                if (ret != QURL_OK)
                {
                    return ret;
                }

                ret = qurl_url_setopt(uurl_ptr, QURL_URL_OPT_URL, core_ptr->cfg.url_ptr, 0);
                if (ret == QURL_OK)
                {
                    /*Eliminate Fragment*/
                    ret = qurl_url_setopt(uurl_ptr, QURL_URL_OPT_FRAGMENT, QOSA_NULL, 0);
                }
                if (ret == QURL_OK)
                {
                    /*Kill the username*/
                    ret = qurl_url_setopt(uurl_ptr, QURL_URL_OPT_USER, QOSA_NULL, 0);
                }
                if (ret == QURL_OK)
                {
                    /*Remove password: for security.*/
                    ret = qurl_url_setopt(uurl_ptr, QURL_URL_OPT_PASSWORD, QOSA_NULL, 0);
                }
                if (ret == QURL_OK)
                {
                    ret = qurl_url_getopt(uurl_ptr, QURL_URL_OPT_URL, &referer_ptr, 0);
                }

                qurl_url_delete(uurl_ptr);

                if (ret != QURL_OK)
                {
                    return ret;
                }

                core_ptr->cfg.referer_ptr = referer_ptr;
                core_ptr->cfg.bits.referer_alloc_flag = QOSA_TRUE;
            }
        }
    }

    /** If this is not a redirect due to a 401 or 407 response, and it is an absolute URL: custom port numbers are not allowed.
	 */
    if ((type != QURL_TRANS_FOLLOWTYPE_RETRY) && (core_ptr->info.resp_code != 401) && (core_ptr->info.resp_code != 407)
        && qurl_url_is_absolute(new_url, QOSA_NULL, 0))
    {
        disallowport = QOSA_TRUE;
    }

    /*Update URL*/
    ret = qurl_url_setopt(
        core_ptr->cfg.uurl_ptr,
        QURL_URL_OPT_URL,
        new_url,
        (type == QURL_TRANS_FOLLOWTYPE_FAKE) ? QURL_URL_GUESS_SCHEME
                                             : ((type == QURL_TRANS_FOLLOWTYPE_REDIR) ? QURL_URL_URLENCODE : 0) | QURL_URL_ALLOW_SPACE
                                                   | (core_ptr->usr_cfg.bits.path_as_is ? QURL_URL_PATH_AS_IS : 0)
    );

    if (ret != QURL_OK)
    {
        if (type != QURL_TRANS_FOLLOWTYPE_FAKE)
        {
            return ret;
        }

        /*If the new URL parsing fails, because this is FAKE, just for obtaining information, then directly copy it.*/
        new_url = qurl_strdup(new_url);
        if (new_url == QOSA_NULL)
        {
            return QURL_ECODE_NO_MEMORY;
        }
    }
    else
    {
        ret = qurl_url_getopt(core_ptr->cfg.uurl_ptr, QURL_URL_OPT_URL, &new_url, 0);
        if (ret != QURL_OK)
        {
            return ret;
        }

        /*When redirecting to different port numbers or protocols, check whether it is necessary to clear the username and password provided by the user. For security.*/
        if (core_ptr->usr_cfg.bits.allow_auth_to_other_hosts == QOSA_FALSE && (type != QURL_TRANS_FOLLOWTYPE_FAKE))
        {
            char      *portnum = QOSA_NULL;
            int        port = 0;
            qurl_bit_t clear = QOSA_FALSE;

            if (core_ptr->usr_cfg.port && core_ptr->cfg.bits.allow_port)
            {
                /*Allows the use of user-configured port numbers.*/
                port = (int)core_ptr->usr_cfg.port;
            }
            else
            {
                /*If the user does not specify, obtain it from the location.*/
                ret = qurl_url_getopt(core_ptr->cfg.uurl_ptr, QURL_URL_OPT_PORT, &portnum, QURL_URL_DEFAULT_PORT);
                if (ret != QURL_OK)
                {
                    qurl_free(new_url);
                    new_url = QOSA_NULL;
                    return ret;
                }
                port = qurl_atoi(portnum);
                qurl_free(portnum);
                portnum = QOSA_NULL;
            }

            if (port != core_ptr->cfg.first_remote_port)
            {
                qurl_logd("Clear auth, redirects to port from %u to %u\r\n", core_ptr->cfg.first_remote_port, port);
                clear = QOSA_TRUE;
            }
            else
            {
                char                   *scheme = QOSA_NULL;
                const qurl_port_prot_t *port_handler = QOSA_NULL;

                ret = qurl_url_getopt(core_ptr->cfg.uurl_ptr, QURL_URL_OPT_SCHEME, &scheme, 0);
                if (ret != QURL_OK)
                {
                    qurl_free(new_url);
                    new_url = QOSA_NULL;
                    return ret;
                }

                ret = qurl_port_prot_match_by_scheme(scheme, &port_handler);
                if (ret == QURL_OK && (port_handler->protocol != core_ptr->cfg.first_protocol))
                {
                    qurl_logd("Clear auth, redirects scheme from %s to %s\r\n", core_ptr->cfg.scheme, scheme);
                    clear = QOSA_TRUE;
                }
                qurl_free(scheme);
                scheme = QOSA_NULL;
            }

            /**
			 * [lzm][note]Confirm that no username and password are provided to the redirected location.
			 * Here is the cleared parsed cache.
			 * Of course, there are other solutions, such as using a flag? Or directly clearing the username and password in uurl_ptr and then re-parsing?
			 */
            if (clear == QOSA_TRUE)
            {
                if (core_ptr->cfg.user != QOSA_NULL)
                {
                    qurl_free(core_ptr->cfg.user);
                    core_ptr->cfg.user = QOSA_NULL;
                }
                if (core_ptr->cfg.passwd != QOSA_NULL)
                {
                    qurl_free(core_ptr->cfg.passwd);
                    core_ptr->cfg.passwd = QOSA_NULL;
                }
            }
        }
    }

    if (type == QURL_TRANS_FOLLOWTYPE_FAKE)
    {
        /**
		 * OK, just record the information and return.
		 */
        core_ptr->cfg.wouldredirect = new_url;

        if (reachedmax)
        {
            return QURL_ECODE_TRANS_TOO_MANY_REDIRECTS;
        }
        return QURL_OK;
    }
    else if (type == QURL_TRANS_FOLLOWTYPE_REDIR)
    {
        core_ptr->trans.client_w_len = 0;   /*Recalculate. [lzm][note]Of course, the total and the current can also be separated.*/
        core_ptr->trans.downloaded_len = 0; /*Recalculate. [lzm][note]Of course, the total and the current can also be separated.*/
    }

    if (disallowport)
    {
        /*Disable user-specified port numbers*/
        core_ptr->cfg.bits.allow_port = QOSA_FALSE;
    }

    if (core_ptr->cfg.bits.url_alloc_flag && core_ptr->cfg.url_ptr != QOSA_NULL)
    {
        qurl_free(core_ptr->cfg.url_ptr);
        core_ptr->cfg.url_ptr = QOSA_NULL;
    }
    core_ptr->cfg.url_ptr = new_url;
    core_ptr->cfg.bits.url_alloc_flag = QOSA_TRUE;

    qurl_logd("Issue another request to this URL: '%s'\r\n", core_ptr->cfg.url_ptr);

#if QURL_CFG_ENABLE_HTTP
    /**
	 * Handle HTTP response codes in the range 300-399 (and 401). Different actions need to be taken based on the specific response code.
	 * If an HTTP response code of 401 or 407 is encountered (i.e., HTTP proxy authentication error codes), corresponding actions will also be executed. This is typically due to the use of HTTP (proxy) authentication mechanisms other than Basic.
	 */
    switch (core_ptr->info.resp_code)
    {
        default: {
            /*401 - Perform action based on WWW-Authenticate,
and add the Authorization: XXXX header in the HTTP request code snippet*/
            /*407 - Perform action based on Proxy-Authenticate,
and add the Proxy-Authorization: XXXX header in the HTTP request code snippet*/
            /*300 - Multiple Choices*/
            /*306 - Unused*/
            /*307 - Temporary Redirect*/
            break;
        }
        case 301: /*Permanent Redirect*/
        {
            /** Cited from RFC7231 Section 6.4.2
			 * Note: For historical reasons, user agents may change the request method of subsequent requests from POST to GET.
			 * If you do not want this behavior, you can use the 307 (Temporary Redirect) status code instead.
			 *
			 * Many web servers expect this, so these servers often respond to POST requests with error pages.
			 * To ensure qurl obtains the page as most user agents would, qurl must enforce the use of the GET method.
			 * This behavior is prohibited in RFC1945 and the obsolete RFC2616, and can be overridden via CURLOPT_POSTREDIR. [note]To be developed
			 */
            if ((core_ptr->cfg.http.http_method == QURL_HTTP_METHOD_POST || core_ptr->cfg.http.http_method == QURL_HTTP_METHOD_POST_FORM
                 || core_ptr->cfg.http.http_method == QURL_HTTP_METHOD_POST_MIME)
                && !(core_ptr->usr_cfg.http.keep_post & QURL_HTTP_REDIR_POST_301))
            {
                core_ptr->cfg.http.http_method = QURL_HTTP_METHOD_GET;
                qurl_logd("Switch from POST to GET");
            }
            break;
        }
        case 302: /*Temporary Transfer*/
        {
            /** Quoted from RFC7231 Section 6.4.3
			 * Note: For historical reasons, user agents may change the request method of subsequent requests from POST to GET.
			 * If you do not want this behavior, you can use the 307 (Temporary Redirect) status code instead.
			 *
			 * Many web servers expect this, so these servers often respond to POST requests with error pages.
			 * To ensure qurl obtains the page as most user agents would, qurl must enforce the use of the GET method.
			 * This behavior is prohibited in RFC1945 and the obsolete RFC2616, and can be overridden via CURLOPT_POSTREDIR. [note]To be developed
			 */
            if ((core_ptr->cfg.http.http_method == QURL_HTTP_METHOD_POST || core_ptr->cfg.http.http_method == QURL_HTTP_METHOD_POST_FORM
                 || core_ptr->cfg.http.http_method == QURL_HTTP_METHOD_POST_MIME)
                && !(core_ptr->usr_cfg.http.keep_post & QURL_HTTP_REDIR_POST_302))
            {
                core_ptr->cfg.http.http_method = QURL_HTTP_METHOD_GET;
                qurl_logd("Switch from POST to GET");
            }
            break;
        }
        case 303: /*The corresponding resource exists at another URI*/
        {
            /** The position of '303' is not the resource itself, but an alternative location for the resource.
			 * In this case, we will switch the method to GET/HEAD,
			 * Unless the method is POST and the user specifies to keep it as POST.
			 */
            if ((core_ptr->cfg.http.http_method == QURL_HTTP_METHOD_POST || core_ptr->cfg.http.http_method == QURL_HTTP_METHOD_POST_FORM
                 || core_ptr->cfg.http.http_method == QURL_HTTP_METHOD_POST_MIME)
                && !(core_ptr->usr_cfg.http.keep_post & QURL_HTTP_REDIR_POST_303))
            {
                core_ptr->cfg.http.http_method = QURL_HTTP_METHOD_GET;
                qurl_logd("Switch to %s", core_ptr->usr_cfg.bits.opt_nobody ? "HEAD" : "GET");
            }
            break;
        }
        case 304: /*Unmodified*/
        {
            /**
			 * 304 indicates that we made a conditional request and it was "Not Modified".
			 * We should not receive any Location in this response!
			 */
            break;
        }
        case 305: /*Using a proxy*/
        {
            /**
			 * (Quoted from RFC2616, Section 10.3.6):
			 * Access to the requested resource must be made through the proxy specified in the Location field.
			 * The Location field provides the URI of the proxy.
			 * The recipient should repeat this single request through the proxy.
			 * A 305 response can only be generated by the origin server.
			 */
            break;
        }

    }
#endif /* QURL_CFG_ENABLE_HTTP */

    return QURL_OK;
}
