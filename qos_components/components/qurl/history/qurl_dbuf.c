/** @file         qurl_dbuf.c
 *  @brief        Brief description: dbuf, which stands for dynamic data packet, dynamic\data
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-10-11 16:08:45
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#include "qurl_dbuf.h"

#define DBUF_MIN(a, b) ((a) < (b) ? (a) : (b))

static inline qurl_ecode_t dbuf_node_new(qurl_dbuf_node_t *node_ptr, long size)
{
    unsigned char *ptr = NULL;

    ptr = (unsigned char *)qurl_malloc(size + 1);
    if (ptr == NULL)
    {
        return QURL_ECODE_NO_MEMORY;
    }
    ptr[size] = 0x00;

    node_ptr->total_size = size;
    node_ptr->valid_size = 0;
    node_ptr->w_index = 0;
    node_ptr->r_index = 0;
    node_ptr->buf = ptr;

    return QURL_OK;
}

static inline void dbuf_node_delete(qurl_dbuf_node_t *node_ptr)
{
    if (node_ptr->buf != NULL)
    {
        qurl_free(node_ptr->buf);
    }
    memset(node_ptr, 0x00, sizeof(qurl_dbuf_node_t));
}

static inline void dbuf_node_write(qurl_dbuf_node_t *node_ptr, void *data, long len)
{
    long w_index = node_ptr->w_index;
    long total_size = node_ptr->total_size;
    long first_w_size = 0;

    if (node_ptr->valid_size + len > total_size) /*Insufficient space, bro.*/
    {
        /*Logging can be added here*/
        return;
    }

    if (len + w_index <= total_size) /*No need for segmented writing*/
    {
        memcpy(node_ptr->buf + w_index, data, len);
    }
    else /*It needs to be written in segments because it has wrapped around.*/
    {
        first_w_size = total_size - w_index;
        memcpy(node_ptr->buf + w_index, data, first_w_size);
        memcpy(node_ptr->buf, (unsigned char *)data + first_w_size, len - first_w_size);
    }
    /*Update flag*/
    node_ptr->w_index += len;
    node_ptr->w_index %= total_size;
    node_ptr->valid_size += len;
}

/*dbuf_node_write_header is not provided*/

static inline void dbuf_node_read(qurl_dbuf_node_t *node_ptr, void *buff, long len)
{
    long r_index = node_ptr->r_index;
    long total_size = node_ptr->total_size;
    long first_r_size = 0;

    if (len > node_ptr->valid_size) /*Bro, there's not enough data for you to read.*/
    {
        /*Logging can be added here*/
        return;
    }

    if (total_size - r_index >= len) /*No need for segmented reading*/
    {
        memcpy(buff, node_ptr->buf + r_index, len);
    }
    else /*Need to read in segments*/
    {
        first_r_size = total_size - r_index;
        memcpy(buff, node_ptr->buf + r_index, first_r_size);
        memcpy((unsigned char *)buff + first_r_size, node_ptr->buf, len - first_r_size);
    }
    /*Update flag*/
    node_ptr->r_index += len;
    node_ptr->r_index %= total_size;
    node_ptr->valid_size -= len;
}

static inline long dbuf_node_valid_size(qurl_dbuf_node_t *node_ptr)
{
    return (node_ptr->valid_size);
}

static inline long dbuf_node_idle_size(qurl_dbuf_node_t *node_ptr)
{
    return (node_ptr->total_size - node_ptr->valid_size);
}

static inline unsigned char dbuf_node_is_empty(qurl_dbuf_node_t *node_ptr)
{
    return (node_ptr->valid_size == 0);
}

static inline unsigned char dbuf_node_is_full(qurl_dbuf_node_t *node_ptr)
{
    return (node_ptr->valid_size == node_ptr->total_size);
}

qurl_ecode_t qurl_dbuf_init(qurl_dbuf_t *dbuf_ptr, long limit_size)
{
    qurl_ecode_t      ret = 0;
    qurl_dbuf_ctrl_t *ctrl_ptr = (qurl_dbuf_ctrl_t *)dbuf_ptr;

    if (ctrl_ptr == NULL || limit_size == 0)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    ctrl_ptr->limit_size = limit_size;
    ctrl_ptr->total_size = 0;
    ctrl_ptr->valid_size = 0;

    ret = qurl_list_init(&(ctrl_ptr->data_list));
    if (ret != QURL_OK)
    {
        return ret;
    }

    return QURL_OK;
}

qurl_ecode_t qurl_dbuf_push_data(qurl_dbuf_t *dbuf_ptr, void *data_ptr, long data_size)
{
    qurl_ecode_t      ret = 0;
    long              idle_size = 0; /*Current writable space of the dbuf chain*/
    long              new_dbuf_len = 0;
    long              written_len = 0;
    qurl_list_t       debris_list = {0};
    qurl_list_t      *pos_ptr = NULL;
    qurl_list_t      *next_ptr = NULL;
    qurl_dbuf_node_t *new_node_ptr = NULL;
    qurl_dbuf_node_t *debris_new_node_ptr = NULL;
    qurl_dbuf_node_t *node_ptr = NULL;
    qurl_dbuf_ctrl_t *ctrl_ptr = (qurl_dbuf_ctrl_t *)dbuf_ptr;

    if (ctrl_ptr == NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    if (data_ptr == NULL || data_size == 0)
    {
        return QURL_OK;
    }

    if (ctrl_ptr->limit_size > 0 && (ctrl_ptr->valid_size + data_size) > ctrl_ptr->limit_size)
    {
        return QURL_ECODE_OVER_LIMIT;
    }

    /*Two memory allocation schemes:
	 * 1. Apply for sufficient memory at once, then push it in.
	 * 2. Allocate dynamic memory according to the minimum granularity of dbuf, then push it into the chain.
	 * Both solutions can be implemented, and when a single large allocation fails, try the minimal granularity allocation. This approach is suitable for platforms prone to memory fragmentation.*/
    qurl_list_for_each_prev(pos_ptr, &(ctrl_ptr->data_list))
    {
        node_ptr = qurl_list_entry(pos_ptr, qurl_dbuf_node_t, node);
        idle_size = dbuf_node_idle_size(node_ptr);
        break;
    }

    if (node_ptr != NULL && idle_size >= data_size) /*Currently there is sufficient free space.*/
    {
        dbuf_node_write(node_ptr, data_ptr, data_size);
        ctrl_ptr->valid_size += data_size;
    }
    else /*Need to add dbuf*/
    {
        new_dbuf_len = data_size - idle_size;
        if (new_dbuf_len < QURL_DBUF_NODE_MIN_SIZE)
        {
            new_dbuf_len = QURL_DBUF_NODE_MIN_SIZE;
        }

        new_node_ptr = qurl_malloc(sizeof(qurl_dbuf_node_t));
        if (new_node_ptr == NULL)
        {
            return QURL_ECODE_NO_MEMORY;
        }

        ret = dbuf_node_new(new_node_ptr, new_dbuf_len);
        if (ret == QURL_OK)
        {
            /*Option 1: One-time Write*/
            if (node_ptr != NULL && idle_size > 0)
            {
                dbuf_node_write(node_ptr, data_ptr, idle_size);
            }
            dbuf_node_write(new_node_ptr, data_ptr + idle_size, data_ptr - idle_size);
            ctrl_ptr->total_size += new_dbuf_len;
            ctrl_ptr->valid_size += data_size;
        }
        else if ((data_size - idle_size) != new_dbuf_len)
        {
            /*Option 2: Fragmented Write*/
            qurl_free(new_node_ptr);
            new_node_ptr = NULL;

            qurl_list_init(&(debris_list));

            new_dbuf_len = 0;
            while (written_len < (data_size - idle_size))
            {
                debris_new_node_ptr = qurl_malloc(sizeof(qurl_dbuf_node_t));
                if (debris_new_node_ptr == NULL)
                {
                    break;
                }

                ret = dbuf_node_new(debris_new_node_ptr, QURL_DBUF_NODE_MIN_SIZE);
                if (ret != QURL_OK)
                {
                    qurl_free(debris_new_node_ptr);
                    debris_new_node_ptr = NULL;
                    break;
                }

                dbuf_node_write(
                    debris_new_node_ptr,
                    data_ptr + idle_size + written_len,
                    DBUF_MIN((data_size - idle_size - written_len), QURL_DBUF_NODE_MIN_SIZE)
                );
                lzm_list_add_tail(&(debris_list), &(debris_new_node_ptr->node));
                written_len += DBUF_MIN((data_size - idle_size - written_len), QURL_DBUF_NODE_MIN_SIZE);
                new_dbuf_len += QURL_DBUF_NODE_MIN_SIZE;
            };
            if (written_len < (data_size - idle_size))
            {
                /*Insufficient space caused*/
                qurl_list_for_each_safe(pos_ptr, next_ptr, &(debris_list))
                {
                    debris_new_node_ptr = qurl_list_entry(pos_ptr, qurl_dbuf_node_t, node);
                    qurl_list_delete(&(debris_new_node_ptr->node));
                    dbuf_node_delete(debris_new_node_ptr);
                    qurl_free(debris_new_node_ptr);
                }
                return QURL_ECODE_NO_MEMORY;
            }
            /*There is no issue with the memory.*/
            if (node_ptr != NULL && idle_size > 0)
            {
                dbuf_node_write(node_ptr, data_ptr, idle_size);
            }
            qurl_list_concat(&(ctrl_ptr->data_list), &(debris_list));
            ctrl_ptr->total_size += new_dbuf_len;
            ctrl_ptr->valid_size += data_size;
        }
        else
        {
            /*There is really no space left.*/
            qurl_free(new_node_ptr);
            return QURL_ECODE_NO_MEMORY;
        }
    }

    return QURL_OK;
}

qurl_ecode_t qurl_dbuf_pull_data(qurl_dbuf_t *dbuf_ptr, void *data_ptr, long pull_size)
{
    long              node_idle_len = 0;
    qurl_list_t      *pos_ptr = NULL;
    qurl_list_t      *next_ptr = NULL;
    qurl_dbuf_node_t *node_ptr = NULL;
    qurl_dbuf_ctrl_t *ctrl_ptr = (qurl_dbuf_ctrl_t *)dbuf_ptr;

    if (ctrl_ptr == NULL || (pull_size != 0 && data_ptr == NULL))
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    if (pull_size == 0)
    {
        return QURL_OK;
    }

    if (pull_size > ctrl_ptr->valid_size)
    {
        return QURL_ECODE_OVER_LIMIT;
    }

    qurl_list_for_each_safe(pos_ptr, next_ptr, &(ctrl_ptr->data_list))
    {
        node_ptr = qurl_list_entry(pos_ptr, qurl_dbuf_node_t, node);
        node_idle_len = dbuf_node_idle_size(node_ptr);
        dbuf_node_read(node_ptr, data_ptr, DBUF_MIN(pull_size, node_idle_len));
        pull_size -= DBUF_MIN(pull_size, node_idle_len);
        if (dbuf_node_is_empty(node_ptr))
        {
            qurl_list_delete(&(node_ptr->node));
            dbuf_node_delete(node_ptr);
            qurl_free(node_ptr);
            node_ptr = NULL;
        }
        if (pull_size <= 0)
        {
            break;
        }
    }

    return QURL_OK;
}

qurl_ecode_t qurl_dbuf_delete(qurl_dbuf_t *dbuf_ptr)
{
    qurl_list_t      *pos_ptr = NULL;
    qurl_list_t      *next_ptr = NULL;
    qurl_dbuf_node_t *node_ptr = NULL;
    qurl_dbuf_ctrl_t *ctrl_ptr = (qurl_dbuf_ctrl_t *)dbuf_ptr;

    if (ctrl_ptr == NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    qurl_list_for_each_safe(pos_ptr, next_ptr, &(ctrl_ptr->data_list))
    {
        node_ptr = qurl_list_entry(pos_ptr, qurl_dbuf_node_t, node);
        qurl_list_delete(&(node_ptr->node));
        dbuf_node_delete(node_ptr);
        qurl_free(node_ptr);
    }

    return QURL_OK;
}
