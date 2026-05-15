/** @file         qurl_http_multiform.c
 *  @brief        Brief description: Implementation of multi-form functionality
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-11-06 11:37:25
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#include "qurl_config.h"
#include "qurl_code_in.h"
#include "qurl_log.h"
#include "qurl_def_in.h"
#include "qurl_slist.h"
#include "qurl_list.h"
#if (QURL_CFG_ENABLE_FS == 0)
#include "qurl_default_fs.h"
#endif
#include "qurl_platform_port.h"

#if QURL_CFG_ENABLE_HTTP
#include "qurl_http_def.h"
#include "qurl_http.h"
#include "qurl_http_multiform.h"

static char g_multiform_boundary_str[] = QURL_CFG_MULTIFORM_BOUNDARY_DEFAULT;

/**
 * @brief Clear data, do not clear location: such as ID and mounted linked lists are not cleared
 *
 * @param form_ctrl_ptr
 * @return qurl_ecode_t
 */
static qurl_ecode_t multiform_clean_form_data(qurl_http_form_ctrl_t *form_ctrl_ptr)
{
    if (form_ctrl_ptr->name_ptr != QOSA_NULL)
    {
        qurl_free(form_ctrl_ptr->name_ptr);
        form_ctrl_ptr->name_ptr = QOSA_NULL;
    }
    if (form_ctrl_ptr->filename_ptr != QOSA_NULL)
    {
        qurl_free(form_ctrl_ptr->filename_ptr);
        form_ctrl_ptr->filename_ptr = QOSA_NULL;
    }
    if (form_ctrl_ptr->content_ptr != QOSA_NULL)
    {
        if (form_ctrl_ptr->content_type == QURL_HTTP_FORM_CONTENT_DATA_CP)
        {
            qurl_free(form_ctrl_ptr->content_ptr);
        }
        else if (form_ctrl_ptr->content_type == QURL_HTTP_FORM_CONTENT_FILE)
        {
            qurl_free(form_ctrl_ptr->content_ptr);
            if (form_ctrl_ptr->content_fd >= 0)
            {
                qurl_fclose(form_ctrl_ptr->content_fd);
            }
        }
        form_ctrl_ptr->content_ptr = QOSA_NULL;
    }

    form_ctrl_ptr->read_content_func = QOSA_NULL;
    form_ctrl_ptr->headers_slist = QOSA_NULL;
    form_ctrl_ptr->content_len = 0;
    form_ctrl_ptr->content_fd = -1;
    form_ctrl_ptr->send_index = 0;

    return QURL_OK;
}

/**
 * @brief Clear all runtime-generated data in multiple forms
 *
 * @param[in] multiform_ctrl_ptr
 * @return qurl_ecode_t
 */
static qurl_ecode_t multiform_clean_running_data(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr)
{
    qurl_http_form_ctrl_t *form_ctrl_ptr = QOSA_NULL;
    qurl_list_node_t      *node_ptr = QOSA_NULL;

    node_ptr = (qurl_list_node_t *)qurl_list_check_head(&multiform_ctrl_ptr->form_list);
    while (node_ptr)
    {
        form_ctrl_ptr = qurl_list_node_entry(node_ptr, qurl_http_form_ctrl_t, node);

        if (form_ctrl_ptr->content_fd >= 0)
        {
            qurl_fclose(form_ctrl_ptr->content_fd);
            form_ctrl_ptr->content_fd = -1;
        }

        if (form_ctrl_ptr->content_type == QURL_HTTP_FORM_CONTENT_FILE)
        {
            form_ctrl_ptr->content_len = 0;
        }

        form_ctrl_ptr->send_index = 0;

        //Continue
        node_ptr = (qurl_list_node_t *)qurl_list_check_next(&multiform_ctrl_ptr->form_list, node_ptr);
    }

    multiform_ctrl_ptr->state = QURL_HTTP_MULTIFORM_STATE_NONE;
    multiform_ctrl_ptr->boundary_send_index = 0;
    multiform_ctrl_ptr->content_total_len = 0;
    multiform_ctrl_ptr->content_upload_len = 0;
    multiform_ctrl_ptr->cur_form_ctrl_ptr = QOSA_NULL;

    return QURL_OK;
}

qurl_ecode_t qurl_http_multiform_init(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr)
{
    if (multiform_ctrl_ptr == QOSA_NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    multiform_ctrl_ptr->form_id_limit = 10;
    multiform_ctrl_ptr->form_id_cnt = 0;

    multiform_ctrl_ptr->state = QURL_HTTP_MULTIFORM_STATE_NONE;
    multiform_ctrl_ptr->boundary_send_index = 0;
    multiform_ctrl_ptr->content_total_len = 0;
    multiform_ctrl_ptr->content_upload_len = 0;
    multiform_ctrl_ptr->cur_form_ctrl_ptr = QOSA_NULL;

    multiform_ctrl_ptr->boundary_len = qurl_strlen(g_multiform_boundary_str);
    multiform_ctrl_ptr->boundary_ptr = g_multiform_boundary_str;
    multiform_ctrl_ptr->boundary_alloc = QOSA_FALSE;

    qurl_list_init(&multiform_ctrl_ptr->form_list);

    return QURL_OK;
}

/**
 * @brief Reasonably delete all single forms within this multi-form
 *
 * @param[in] multiform_ctrl_ptr
 * @return qurl_ecode_t
 */
qurl_ecode_t qurl_http_multiform_delete(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr)
{
    qurl_http_form_ctrl_t *form_ctrl_ptr = QOSA_NULL;
    qurl_list_node_t      *node_ptr = QOSA_NULL;

    if (multiform_ctrl_ptr == QOSA_NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    node_ptr = (qurl_list_node_t *)qurl_list_check_head(&multiform_ctrl_ptr->form_list);
    while (node_ptr)
    {
        form_ctrl_ptr = qurl_list_node_entry(node_ptr, qurl_http_form_ctrl_t, node);

        multiform_clean_form_data(form_ctrl_ptr);

        //Continue
        node_ptr = (qurl_list_node_t *)qurl_list_check_next(&multiform_ctrl_ptr->form_list, node_ptr);

        qurl_free(form_ctrl_ptr);
    }

    multiform_ctrl_ptr->state = QURL_HTTP_MULTIFORM_STATE_NONE;
    multiform_ctrl_ptr->boundary_send_index = 0;
    multiform_ctrl_ptr->content_total_len = 0;
    multiform_ctrl_ptr->content_upload_len = 0;
    multiform_ctrl_ptr->cur_form_ctrl_ptr = QOSA_NULL;

    multiform_ctrl_ptr->boundary_len = 0;
    if (multiform_ctrl_ptr->boundary_alloc == QOSA_TRUE)
    {
        qurl_free(multiform_ctrl_ptr->boundary_ptr);
        multiform_ctrl_ptr->boundary_ptr = QOSA_NULL;
        multiform_ctrl_ptr->boundary_alloc = QOSA_FALSE;
    }

    return QURL_OK;
}

qurl_ecode_t qurl_http_multiform_set_id_limit(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr, unsigned char max_id_cnt)
{
    if (multiform_ctrl_ptr == QOSA_NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    multiform_ctrl_ptr->form_id_limit = max_id_cnt;

    return QURL_OK;
}

char *qurl_http_multiform_get_boundary(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr)
{
    return (multiform_ctrl_ptr->boundary_ptr);
}

long qurl_http_multiform_get_total_len(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr)
{
    return (multiform_ctrl_ptr->upload_total_len);
}

long qurl_http_multiform_get_len(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr)
{
    return (multiform_ctrl_ptr->upload_len);
}

qurl_ecode_t qurl_http_multiform_set_form(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr, qosa_uint8_t id, qurl_http_form_cfg_t *form_cfg_ptr)
{
    long                   len = 0;
    qurl_bit_t             found = QOSA_FALSE;
    qurl_bit_t             content_alloc = QOSA_FALSE;
    qurl_list_node_t      *node_ptr = QOSA_NULL;
    qurl_http_form_ctrl_t *form_ctrl_ptr = QOSA_NULL;
    char                  *name_ptr = QOSA_NULL;
    char                  *filename_ptr = QOSA_NULL;
    void                  *content_ptr = QOSA_NULL;
    qurl_http_form_read_cb read_content_func = QOSA_NULL;

    /*Parameter Check*/
    if (multiform_ctrl_ptr == QOSA_NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    if (form_cfg_ptr != QOSA_NULL)
    {
        if (form_cfg_ptr->content_type < QURL_HTTP_FORM_CONTENT_DATA || form_cfg_ptr->content_type > QURL_HTTP_FORM_CONTENT_FILE
            || (form_cfg_ptr->content_type != QURL_HTTP_FORM_CONTENT_FILE && form_cfg_ptr->content_len <= 0))
        {
            return QURL_ECODE_PARAM_INVALID;
        }
        if (form_cfg_ptr->content_type == QURL_HTTP_FORM_CONTENT_CB && form_cfg_ptr->read_content_func == QOSA_NULL)
        {
            return QURL_ECODE_PARAM_INVALID;
        }
    }

    if (id >= multiform_ctrl_ptr->form_id_limit)
    {
        return QURL_ECODE_OVER_LIMIT;
    }

    /*Find this form*/
    node_ptr = (qurl_list_node_t *)qurl_list_check_head(&(multiform_ctrl_ptr->form_list));
    while (node_ptr)
    {
        form_ctrl_ptr = qurl_list_node_entry(node_ptr, qurl_http_form_ctrl_t, node);

        if (form_ctrl_ptr->id == id)
        {
            found = QOSA_TRUE;
            break;
        }
        else if (form_ctrl_ptr->id > id)
        {
            break;
        }
        //< id: Continue
        node_ptr = (qurl_list_node_t *)qurl_list_check_next(&(multiform_ctrl_ptr->form_list), node_ptr);
    }

    /*Resource Preparation*/
    if (form_cfg_ptr != QOSA_NULL)
    {
        if (form_cfg_ptr->name_ptr != QOSA_NULL)
        {
            len = qurl_strlen(form_cfg_ptr->name_ptr);
            name_ptr = qurl_malloc(len + 1);
            if (name_ptr == QOSA_NULL)
            {
                return QURL_ECODE_NO_MEMORY;
            }
            name_ptr[len] = 0x00;
            qurl_memcpy(name_ptr, form_cfg_ptr->name_ptr, len);
        }
        if (form_cfg_ptr->filename_ptr != QOSA_NULL)
        {
            len = qurl_strlen(form_cfg_ptr->filename_ptr);
            filename_ptr = qurl_malloc(len + 1);
            if (filename_ptr == QOSA_NULL)
            {
                qurl_free(name_ptr);
                return QURL_ECODE_NO_MEMORY;
            }
            filename_ptr[len] = 0x00;
            qurl_memcpy(filename_ptr, form_cfg_ptr->filename_ptr, len);
        }
        if ((form_cfg_ptr->content_type == QURL_HTTP_FORM_CONTENT_DATA_CP || form_cfg_ptr->content_type == QURL_HTTP_FORM_CONTENT_FILE)
            && form_cfg_ptr->content_ptr != QOSA_NULL)
        {
            len = qurl_strlen(form_cfg_ptr->content_ptr);
            content_ptr = qurl_malloc(len + 1);
            if (content_ptr == QOSA_NULL)
            {
                qurl_free(name_ptr);
                qurl_free(filename_ptr);
                return QURL_ECODE_NO_MEMORY;
            }
            ((char *)(content_ptr))[len] = 0x00;
            content_alloc = QOSA_TRUE;
            qurl_memcpy(content_ptr, form_cfg_ptr->content_ptr, len);
        }
        else if ((form_cfg_ptr->content_type == QURL_HTTP_FORM_CONTENT_CB) && form_cfg_ptr->read_content_func != QOSA_NULL)
        {
            read_content_func = form_cfg_ptr->read_content_func;
            content_ptr = form_cfg_ptr->content_ptr;
        }
        else if (form_cfg_ptr->content_ptr != QOSA_NULL)
        {
            content_ptr = form_cfg_ptr->content_ptr;
        }
        if (found == QOSA_FALSE)
        {
            form_ctrl_ptr = qurl_malloc(sizeof(qurl_http_form_ctrl_t));
            if (form_ctrl_ptr == QOSA_NULL)
            {
                qurl_free(name_ptr);
                qurl_free(filename_ptr);
                if (content_alloc)
                {
                    qurl_free(content_ptr);
                }
                return QURL_ECODE_NO_MEMORY;
            }
            qurl_memset(form_ctrl_ptr, 0x00, sizeof(qurl_http_form_ctrl_t));
        }
    }

    /*Processing*/
    if (found)
    {
        /*The form already exists*/
        if (form_cfg_ptr == QOSA_NULL)
        {
            /*Delete*/
            multiform_clean_form_data(form_ctrl_ptr);
            qurl_list_node_delete(&(multiform_ctrl_ptr->form_list), node_ptr);
            qurl_free(form_ctrl_ptr);
            form_ctrl_ptr = QOSA_NULL;

            multiform_ctrl_ptr->form_id_cnt--;
        }
        else
        {
            /*Update*/
            multiform_clean_form_data(form_ctrl_ptr);
            form_ctrl_ptr->content_type = form_cfg_ptr->content_type;
            form_ctrl_ptr->content_len = form_cfg_ptr->content_len;
            form_ctrl_ptr->name_ptr = name_ptr;
            form_ctrl_ptr->filename_ptr = filename_ptr;
            form_ctrl_ptr->content_ptr = content_ptr;
            form_ctrl_ptr->read_content_func = read_content_func;
            form_ctrl_ptr->headers_slist = (qurl_slist_ctrl_t *)form_cfg_ptr->headers_slist;
        }
    }
    else if (form_cfg_ptr != QOSA_NULL)
    {
        /*Newly created form*/
        form_ctrl_ptr->id = id;
        form_ctrl_ptr->content_type = form_cfg_ptr->content_type;
        form_ctrl_ptr->content_len = form_cfg_ptr->content_len;
        form_ctrl_ptr->content_fd = -1;
        form_ctrl_ptr->send_index = 0;
        form_ctrl_ptr->name_ptr = name_ptr;
        form_ctrl_ptr->filename_ptr = filename_ptr;
        form_ctrl_ptr->content_ptr = content_ptr;
        form_ctrl_ptr->read_content_func = read_content_func;
        form_ctrl_ptr->headers_slist = (qurl_slist_ctrl_t *)form_cfg_ptr->headers_slist;

        if (node_ptr == QOSA_NULL) /*The linked list has no form with an ID greater than the current form ID, so it can be inserted at the tail of the linked list.*/
        {
            qurl_list_put_tail(&(multiform_ctrl_ptr->form_list), &form_ctrl_ptr->node);
        }
        else /*Currently, node_ptr is the node after the insertion position.*/
        {
            qurl_list_put_prev(&(multiform_ctrl_ptr->form_list), node_ptr, &form_ctrl_ptr->node);
        }
        multiform_ctrl_ptr->form_id_cnt++;
    }

    return QURL_OK;
}

/**
 * @brief Transmission initialization, such as checking files, calculating the length of data to be uploaded, etc.
 *
 * @param[in] multiform_ctrl_ptr
 * @return qurl_ecode_t
 */
qurl_ecode_t qurl_http_multiform_trans_init(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr)
{
    long                   len = 0;
    qurl_http_form_ctrl_t *form_ctrl_ptr = QOSA_NULL;
    qurl_list_node_t      *node_ptr = QOSA_NULL;
    //char deal_str[24]                    = {0};

    if (multiform_ctrl_ptr == QOSA_NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    multiform_ctrl_ptr->upload_total_len = 0;
    multiform_ctrl_ptr->content_total_len = 0; /*Only count the actual body, which is the content of all forms.*/
    /*Traverse and check each form*/
    node_ptr = (qurl_list_node_t *)qurl_list_check_head(&(multiform_ctrl_ptr->form_list));
    /** Find the current transmission position*/
    if (node_ptr != QOSA_NULL)
    {
        multiform_ctrl_ptr->cur_form_ctrl_ptr = qurl_list_node_entry(node_ptr, qurl_http_form_ctrl_t, node);
    }
    /** Search*/
    while (node_ptr)
    {
        qurl_slist_ctrl_t *node = QOSA_NULL;

        len = 0;

        form_ctrl_ptr = qurl_list_node_entry(node_ptr, qurl_http_form_ctrl_t, node);

        if (form_ctrl_ptr->content_type == QURL_HTTP_FORM_CONTENT_FILE)
        {
            form_ctrl_ptr->content_fd = qurl_fopen(form_ctrl_ptr->content_ptr, QURL_O_RDONLY);
            if (form_ctrl_ptr->content_fd < 0)
            {
                /*If one fails to open, all opened files must still be closed.*/
                multiform_clean_running_data(multiform_ctrl_ptr);
                return QURL_ECODE_FS_OPEN_ERR;
            }
            form_ctrl_ptr->content_len = qurl_fgetsize(form_ctrl_ptr->content_fd);
            if (form_ctrl_ptr->content_len < 0)
            {
                multiform_clean_running_data(multiform_ctrl_ptr);
                return QURL_ECODE_FS_READ_ERR;
            }
            form_ctrl_ptr->send_index = 0;
        }

        /*Calculate the effective body (i.e., the content field)*/
        multiform_ctrl_ptr->content_total_len += form_ctrl_ptr->content_len;
        /*Calculate total body: [lzm][lab][202311062245] keep consistent*/
        /** Content-Disposition: */
        len += qurl_strlen("Content-Disposition: form-data; name=\"\"");
        len += qurl_strlen(form_ctrl_ptr->name_ptr);
        if (form_ctrl_ptr->filename_ptr)
        {
            len += qurl_strlen("; filename=\"\"");
            len += qurl_strlen(form_ctrl_ptr->filename_ptr);
        }
        len += qurl_strlen("\r\n");
        /** Content-length: */
        //len += qurl_strlen("Content-Length:");
        //len += qurl_snprintf(deal_str, sizeof(deal_str), " %ld", form_ctrl_ptr->content_len);
        //len += qurl_strlen("\r\n");
        if (form_ctrl_ptr->headers_slist != QOSA_NULL)
        {
            for (node = form_ctrl_ptr->headers_slist; node; node = node->next)
            {
                len += qurl_strlen(node->data);
                len += qurl_strlen("\r\n");
            }
        }
        /**
		 * --boundary\r\n
		 * Content-Disposition: form-data; name="param1"\r\n
		 * Other headers
		 * \r\n
		 * content\r\n
		 */
        multiform_ctrl_ptr->upload_total_len += (2 + multiform_ctrl_ptr->boundary_len + 2 + len + 2 + form_ctrl_ptr->content_len + 2);

        //Continue
        node_ptr = (qurl_list_node_t *)qurl_list_check_next(&(multiform_ctrl_ptr->form_list), node_ptr);
    }
    if (multiform_ctrl_ptr->content_total_len == 0)
    {
        return QURL_ECODE_NO_DATA_SEND; /*There is no data to send.*/
    }
    /* --boundary--\r\n */
    multiform_ctrl_ptr->upload_total_len += (2 + multiform_ctrl_ptr->boundary_len + 2 + 2);
    multiform_ctrl_ptr->upload_len = multiform_ctrl_ptr->upload_total_len;
    multiform_ctrl_ptr->state = QURL_HTTP_MULTIFORM_STATE_BOUNDARY; /*Forms can now be submitted.*/
    multiform_ctrl_ptr->boundary_send_index = 0;
    multiform_ctrl_ptr->content_upload_len = 0;

    return QURL_OK;
}

/**
 * @brief Graceful exit: Clear data generated during operation
 *
 * @param[in] multiform_ctrl_ptr
 * @return qurl_ecode_t
 */
qurl_ecode_t qurl_http_multiform_trans_deinit(qurl_http_multiform_ctrl_t *multiform_ctrl_ptr)
{
    if (multiform_ctrl_ptr == QOSA_NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    return multiform_clean_running_data(multiform_ctrl_ptr);
}

qurl_ecode_t qurl_http_multiform_send_body(qurl_conn_t *conn_ptr)
{
    qurl_bit_t                  continue_flag = QOSA_TRUE;
    long                        len = 0;
    qurl_list_node_t           *node_ptr = QOSA_NULL;
    qurl_http_multiform_ctrl_t *multiform_ctrl_ptr = QOSA_NULL;
    qurl_core_ctrl_t           *core_ptr = conn_ptr->core_ptr;

    multiform_ctrl_ptr = &(core_ptr->usr_cfg.http.multiform_ctrl);

    if (multiform_ctrl_ptr->cur_form_ctrl_ptr == QOSA_NULL)
    {
        return QURL_OK; /*There are no forms to send, so the processing is complete.*/
    }

    /*Send data*/

    /*[lzm][note]If memory permits, we should send the entire form at once. Even if there is available memory, we should exit the loop and send it out first to reduce runtime memory usage. This runtime memory granularity limit can also be handled by creating a new macro.*/
    do {
        continue_flag = QOSA_FALSE;
        switch (multiform_ctrl_ptr->state)
        {
            case QURL_HTTP_MULTIFORM_STATE_BOUNDARY: {
                /*[lzm][todo]Cache optimization is necessary. The available cache space length should be checked, and data should be pushed based on the available space length.
During optimization, the boundary can refer to the following code's process for three-stage processing.*/

                qurl_dbuf_push_data(core_ptr->send_cache, "--", 2);
                qurl_dbuf_push_data(core_ptr->send_cache, multiform_ctrl_ptr->boundary_ptr, multiform_ctrl_ptr->boundary_len);
                multiform_ctrl_ptr->boundary_send_index += multiform_ctrl_ptr->boundary_len;

                qurl_dbuf_push_data(core_ptr->send_cache, "\r\n", 2);
                multiform_ctrl_ptr->upload_len -= (2 + multiform_ctrl_ptr->boundary_len + 2);

                multiform_ctrl_ptr->state = QURL_HTTP_MULTIFORM_STATE_HEADER;
                multiform_ctrl_ptr->boundary_send_index = 0; /*Reset*/
                continue_flag = QOSA_TRUE;                   /*Continue pushing data*/

                break;
            }
            case QURL_HTTP_MULTIFORM_STATE_HEADER: {
                /* [lzm][lab][202311062245] */
                /* Content-Disposition: */
                qurl_slist_ctrl_t *node = QOSA_NULL;

                len += qurl_snprintf(
                    (char *)(conn_ptr->deal_cache_ptr + len),
                    QURL_CONN_DEAL_CACHE_SIZE - len,
                    "Content-Disposition: form-data; name=\"%s\"",
                    multiform_ctrl_ptr->cur_form_ctrl_ptr->name_ptr ? multiform_ctrl_ptr->cur_form_ctrl_ptr->name_ptr : ""
                );
                if (multiform_ctrl_ptr->cur_form_ctrl_ptr->filename_ptr)
                {
                    len += qurl_snprintf(
                        (char *)(conn_ptr->deal_cache_ptr + len),
                        QURL_CONN_DEAL_CACHE_SIZE - len,
                        "; filename=\"%s\"",
                        multiform_ctrl_ptr->cur_form_ctrl_ptr->filename_ptr
                    );
                }
                len += qurl_snprintf((char *)(conn_ptr->deal_cache_ptr + len), QURL_CONN_DEAL_CACHE_SIZE - len, "\r\n");
                qurl_dbuf_push_data(core_ptr->send_cache, conn_ptr->deal_cache_ptr, len);
                multiform_ctrl_ptr->upload_len -= (len);
                len = 0;

                /* Content-Type: */
                /** [lzm][todo]To be developed based on customer requirements*/

                /* Content-length: */
                //              len += qurl_snprintf((char *)(conn_ptr->deal_cache_ptr + len), QURL_CONN_DEAL_CACHE_SIZE - len, "Content-length: %ld\r\n", multiform_ctrl_ptr->cur_form_ctrl_ptr->content_len);
                //              qurl_dbuf_push_data(core_ptr->send_cache, conn_ptr->deal_cache_ptr, len);
                //              multiform_ctrl_ptr->upload_len -= (len);
                //              len                             = 0;

                if (multiform_ctrl_ptr->cur_form_ctrl_ptr->headers_slist != QOSA_NULL)
                {
                    for (node = multiform_ctrl_ptr->cur_form_ctrl_ptr->headers_slist; node; node = node->next)
                    {
                        qurl_dbuf_push_data(core_ptr->send_cache, node->data, qurl_strlen(node->data));
                        multiform_ctrl_ptr->upload_len -= qurl_strlen(node->data);
                        qurl_dbuf_push_data(core_ptr->send_cache, "\r\n", 2);
                        multiform_ctrl_ptr->upload_len -= (2);
                    }
                }

                qurl_dbuf_push_data(core_ptr->send_cache, "\r\n", 2);
                multiform_ctrl_ptr->upload_len -= (2);

                /*[lzm][todo]Custom header for single form pending development. To be developed based on customer requirements.*/
                multiform_ctrl_ptr->state = QURL_HTTP_MULTIFORM_STATE_CONTENT;
                continue_flag = QOSA_TRUE; /*Continue pushing data*/

                break;
            }
            case QURL_HTTP_MULTIFORM_STATE_CONTENT: {
                if (multiform_ctrl_ptr->cur_form_ctrl_ptr->content_type == QURL_HTTP_FORM_CONTENT_DATA
                    || multiform_ctrl_ptr->cur_form_ctrl_ptr->content_type == QURL_HTTP_FORM_CONTENT_DATA_CP)
                {
                    /*[lzm][todo]Here, QURL_CONN_DEAL_CACHE_SIZE is used, which is entirely borrowed from the macro of the HTTP protocol layer, because in principle, QURL_HTTP_DEAL_CACHE_SIZE should not be used.
In principle, a new transmission granularity limit should be established, or the available space in the dbuf should be obtained.*/
                    len = QURL_MIN(
                        (multiform_ctrl_ptr->cur_form_ctrl_ptr->content_len - multiform_ctrl_ptr->cur_form_ctrl_ptr->send_index),
                        QURL_CONN_DEAL_CACHE_SIZE
                    );

                    /*Full Push*/
                    qurl_dbuf_push_data(
                        core_ptr->send_cache,
                        (void *)((char *)multiform_ctrl_ptr->cur_form_ctrl_ptr->content_ptr + multiform_ctrl_ptr->cur_form_ctrl_ptr->send_index),
                        len
                    );
                    multiform_ctrl_ptr->upload_len -= (len);

                    multiform_ctrl_ptr->cur_form_ctrl_ptr->send_index += len;
                }
                else if (multiform_ctrl_ptr->cur_form_ctrl_ptr->content_type == QURL_HTTP_FORM_CONTENT_CB)
                {
                    /*[lzm][todo]Here, QURL_CONN_DEAL_CACHE_SIZE is used, which is entirely borrowed from the macro of the HTTP protocol layer, because in principle, QURL_HTTP_DEAL_CACHE_SIZE should not be used.
In principle, a new transmission granularity limit should be established, or the available space in the dbuf should be obtained.*/
                    len = QURL_MIN(
                        (multiform_ctrl_ptr->cur_form_ctrl_ptr->content_len - multiform_ctrl_ptr->cur_form_ctrl_ptr->send_index),
                        QURL_CONN_DEAL_CACHE_SIZE
                    );

                    if (multiform_ctrl_ptr->cur_form_ctrl_ptr->read_content_func != QOSA_NULL)
                    {
                        len = multiform_ctrl_ptr->cur_form_ctrl_ptr
                                  ->read_content_func(conn_ptr->deal_cache_ptr, len, multiform_ctrl_ptr->cur_form_ctrl_ptr->content_ptr);
                        if (len < 0)
                        {
                            return QURL_ECODE_HTTP_MULTIFORM_ERR;
                        }
                        qurl_dbuf_push_data(core_ptr->send_cache, conn_ptr->deal_cache_ptr, len);
                        multiform_ctrl_ptr->upload_len -= (len);

                        multiform_ctrl_ptr->cur_form_ctrl_ptr->send_index += len;
                    }
                    else
                    {
                        qurl_loge("\r\n\r\n");
                        return QURL_ECODE_HTTP_MULTIFORM_ERR;
                    }
                }
                else if (multiform_ctrl_ptr->cur_form_ctrl_ptr->content_type == QURL_HTTP_FORM_CONTENT_FILE)
                {
                    /*[lzm][todo]Here, QURL_CONN_DEAL_CACHE_SIZE is used, which is entirely borrowed from the macro of the HTTP protocol layer, because in principle, QURL_HTTP_DEAL_CACHE_SIZE should not be used.
In principle, a new transmission granularity limit should be established, or the available space in the dbuf should be obtained.*/
                    len = QURL_MIN(
                        (multiform_ctrl_ptr->cur_form_ctrl_ptr->content_len - multiform_ctrl_ptr->cur_form_ctrl_ptr->send_index),
                        QURL_CONN_DEAL_CACHE_SIZE
                    );

                    /*[lzm][note]Currently, reading files is done sequentially, so there is no need to add a seek function, but this development may still be required later if issues arise.*/
                    len = qurl_fread(multiform_ctrl_ptr->cur_form_ctrl_ptr->content_fd, conn_ptr->deal_cache_ptr, len);
                    if (len < 0)
                    {
                        return QURL_ECODE_FS_READ_ERR;
                    }
                    qurl_dbuf_push_data(core_ptr->send_cache, conn_ptr->deal_cache_ptr, len);
                    multiform_ctrl_ptr->upload_len -= (len);

                    multiform_ctrl_ptr->cur_form_ctrl_ptr->send_index += len;
                }

                if (multiform_ctrl_ptr->cur_form_ctrl_ptr->send_index >= multiform_ctrl_ptr->cur_form_ctrl_ptr->content_len)
                {
                    /*The form has been sent.*/
                    qurl_dbuf_push_data(core_ptr->send_cache, "\r\n", 2);
                    multiform_ctrl_ptr->upload_len -= (2);

                    node_ptr = (qurl_list_node_t *)qurl_list_check_next(&(multiform_ctrl_ptr->form_list), &multiform_ctrl_ptr->cur_form_ctrl_ptr->node);
                    if (node_ptr == QOSA_NULL)
                    {
                        multiform_ctrl_ptr->state = QURL_HTTP_MULTIFORM_STATE_BOUNDARY_LAST;
                        continue_flag = QOSA_TRUE; /*Just be straightforward.*/
                    }
                    else
                    {
                        multiform_ctrl_ptr->cur_form_ctrl_ptr = qurl_list_node_entry(node_ptr, qurl_http_form_ctrl_t, node);
                        multiform_ctrl_ptr->state = QURL_HTTP_MULTIFORM_STATE_BOUNDARY;
                    }
                }

                break;
            }
            case QURL_HTTP_MULTIFORM_STATE_BOUNDARY_LAST: {
                qurl_dbuf_push_data(core_ptr->send_cache, "--", 2);
                qurl_dbuf_push_data(core_ptr->send_cache, multiform_ctrl_ptr->boundary_ptr, multiform_ctrl_ptr->boundary_len);
                multiform_ctrl_ptr->boundary_send_index += multiform_ctrl_ptr->boundary_len;

                qurl_dbuf_push_data(core_ptr->send_cache, "--", 2);
                qurl_dbuf_push_data(core_ptr->send_cache, "\r\n", 2);
                multiform_ctrl_ptr->upload_len -= (2 + multiform_ctrl_ptr->boundary_len + 2 + 2);

                multiform_ctrl_ptr->state = QURL_HTTP_MULTIFORM_STATE_DONE;
                multiform_ctrl_ptr->boundary_send_index = 0; /*Reset*/

                break;
            }
            case QURL_HTTP_MULTIFORM_STATE_DONE: {
                continue_flag = QOSA_FALSE;
                break;
            }
            default:
                return QURL_ECODE_HTTP_MULTIFORM_STATE_ERR;
                //break;
        }
    } while (continue_flag);

    qurl_loge("need to send:[%d]\r\n", multiform_ctrl_ptr->upload_len);
    return QURL_OK;
}

qurl_bit_t qurl_http_multiform_send_body_is_done(qurl_conn_t *conn_ptr)
{
    qurl_http_multiform_ctrl_t *multiform_ctrl_ptr = QOSA_NULL;

    multiform_ctrl_ptr = &(conn_ptr->core_ptr->usr_cfg.http.multiform_ctrl);

    return (multiform_ctrl_ptr->state == QURL_HTTP_MULTIFORM_STATE_DONE);
}

#endif
