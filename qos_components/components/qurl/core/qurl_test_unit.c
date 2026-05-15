/** @file         qurl_test_unit.c
 *  @brief        Brief description: qurl's internal test unit, used to check whether internal APIs and components are functioning properly.
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-10-13 11:39:29
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#include "qurl_test_unit.h"
#include "qurl_log.h"
#include "qurl_dbuf.h"

qurl_ecode_t qurl_test_unit_dbuf(void)
{
    qurl_ecode_t ret = QURL_OK;
    qurl_dbuf_t  dbuf = QOSA_NULL;
    long         len = 0;
    char         data[30] = {0};

    ret = qurl_dbuf_new(&dbuf, QURL_DBUF_NO_LIMIT);
    if (ret != QURL_OK)
    {
        qurl_loge("test faild:%x\r\n", ret);
        return ret;
    }

    ret = qurl_dbuf_push_data(dbuf, "1234567890", 10);
    if (ret != QURL_OK)
    {
        qurl_loge("test faild:%x\r\n", ret);
        return ret;
    }
    ret = qurl_dbuf_push_data(dbuf, "abcdefghij", 10);
    if (ret != QURL_OK)
    {
        qurl_loge("test faild:%x\r\n", ret);
        return ret;
    }

    ret = qurl_dbuf_read_data(dbuf, data, 6, 0);
    if (ret != QURL_OK)
    {
        qurl_loge("test faild:%x\r\n", ret);
        return ret;
    }
    if (qurl_strcmp(data, "123456") != 0)
    {
        qurl_loge("test faild:%s\r\n", data);
        return QURL_ECODE_FAILURE;
    }
    qurl_memset(data, 0x00, sizeof(data));

    len = sizeof(data);
    ret = qurl_dbuf_read_data_try(dbuf, data, &len, 0);
    if (ret != QURL_OK)
    {
        qurl_loge("test faild:%x\r\n", ret);
        return ret;
    }
    if (qurl_strcmp(data, "1234567890abcdefghij") != 0)
    {
        qurl_loge("test faild:%s\r\n", data);
        return QURL_ECODE_FAILURE;
    }
    qurl_memset(data, 0x00, sizeof(data));

    ret = qurl_dbuf_pull_data(dbuf, data, 5);
    if (ret != QURL_OK)
    {
        qurl_loge("test faild:%x\r\n", ret);
        return ret;
    }
    if (qurl_strcmp(data, "12345") != 0)
    {
        qurl_loge("test faild:%s\r\n", data);
        return QURL_ECODE_FAILURE;
    }

    ret = qurl_dbuf_pull_data(dbuf, data, 15);
    if (ret != QURL_OK)
    {
        qurl_loge("test faild:%x\r\n", ret);
        return ret;
    }
    if (qurl_strcmp(data, "67890abcdefghij") != 0)
    {
        qurl_loge("test faild:%s\r\n", data);
        return QURL_ECODE_FAILURE;
    }

    qurl_dbuf_delete(dbuf);

    qurl_loge("test unit:[dbuf] [success]\r\n");

    return QURL_OK;
}

qurl_ecode_t qurl_test_unit(void)
{
    qurl_test_unit_dbuf();

    return QURL_OK;
}
