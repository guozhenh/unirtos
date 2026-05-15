/*****************************************************************/ /**
* @file qcm_web_http.c
* @brief
* @author harry.li@quectel.com
* @date 2024-04-25
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-04-25 <td>1.0 <td>harry.Li <td> Init
* </table>
**********************************************************************/

#include "qcm_base64.h"
#include "qosa_sys.h"
#include "qosa_def.h"
#include "qosa_log.h"
#include "qcm_socket_adp.h"
#include "qcm_websocket.h"
#include "qcm_web_http.h"
#include "qosa_queue_list.h"
#include "qcm_proj_config.h"
#ifdef CONFIG_QCM_VTLS_FUNC
#ifdef CONFIG_QCM_MBEDTLS_4_LIBRARY_FUNC
#include "mbedtls/private/sha1.h"
#else /* CONFIG_QCM_MBEDTLS_4_LIBRARY_FUNC */
#include "mbedtls/sha1.h"
#endif /* CONFIG_QCM_MBEDTLS_4_LIBRARY_FUNC */
#endif

#define QOS_LOG_TAG LOG_TAG_WEB_API

//WEB GUID is a globally unique identifier (GUID) used in web protocols to identify server-side responses.
#define WEB_GUID    "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

#define WEB_FREE_PTR(p)                                                                                                                                        \
    {                                                                                                                                                          \
        if ((p) != QOSA_NULL)                                                                                                                                  \
        {                                                                                                                                                      \
            qosa_free(p);                                                                                                                                      \
            (p) = QOSA_NULL;                                                                                                                                   \
        }                                                                                                                                                      \
    }

/**
 * @brief Obtain random data from the random number source
 *
 * This function generates random numbers by calling the qcm_rand() function, then fills the provided buffer with the random numbers byte by byte.
 * If the required length of random data exceeds the byte size of the return value from the qcm_rand() function, qcm_rand() will be called multiple times.
 * The qcm_rand() function is a pseudo-random function on some platforms, so the generated random numbers may repeat.
 *
 * @param buf Pointer to the buffer to be filled with random data.
 * @param len The length of random data to be filled (in bytes).
 * @return Returns the actual number of bytes filled into the buffer.
 */
int qcm_ws_get_random(void *buf, int len)
{
    qosa_uint8_t *pb = buf;  //Points to the current byte position to be filled

    while (len)
    {
        qosa_uint32_t r = (qosa_uint32_t)qosa_rand();  //Generate a random number
        qosa_uint8_t *p = (qosa_uint8_t *)&r;          //Convert random numbers to byte form
        int           b = 4;                           //Number of bytes occupied by random numbers

        //If the remaining length to be filled is less than the byte count of the random number, adjust it to the remaining length.
        if (len < b)
        {
            b = len;
        }
        len -= b;  //Update remaining padding length
        //Fill each byte of the random number into the buffer
        while (b--)
        {
            *pb++ = p[b];
        }
    }

    //Returns the number of bytes filled
    return pb - (qosa_uint8_t *)buf;
}

/**
 * Calculate the WebSocket accept key.
 * This function calculates a Base64-encoded HMAC-SHA1 hash value based on the given WebSocket key and a globally unique identifier (GUID).
 * Used to verify the SEC-WEBSOCKET-ACCEPT response header issued by the server.
 *
 * @param sec_websocket_key A random string provided by the client, used as part of calculating the accept key.
 * @return The computed Base64-encoded HMAC-SHA1 hash string. Returns NULL if memory allocation fails.
 */

static char *qcm_compute_sec_websocket_accept(const char *sec_websocket_key)
{
#ifndef CONFIG_QCM_VTLS_FUNC
    //If CONFIG_OSA_VTLS_FUNC is not defined, return NULL directly without performing subsequent operations.
    return QOSA_NULL;
#endif

    //Calculate the length of the concatenated string (key + GUID)
    int                  ret = 0;
    qosa_size_t          concat_len = qosa_strlen(sec_websocket_key) + qosa_strlen(WEB_GUID);
    mbedtls_sha1_context context;
    //Allocate memory for the concatenated string
    char *concat_str = qosa_malloc(concat_len + 1);
    if (concat_str == QOSA_NULL)
    {
        QLOGE("Memory allocation failed.\n");
        return QOSA_NULL;
    }

    qosa_memset(concat_str, 0, concat_len + 1);
    //Concatenate the key and GUID into a new string
    qosa_strcpy(concat_str, sec_websocket_key);
    qosa_strcat(concat_str, WEB_GUID);

    //Use the concatenated string and WebSocket authentication key to calculate HMAC-SHA1
    qosa_uint8_t sha1_hash[20];  //The length of the SHA1 hash is 20 bytes.
    QLOGD("concat_str=%s", concat_str);

    mbedtls_sha1_init(&context);
    mbedtls_sha1_starts(&context);
    mbedtls_sha1_update(&context, (qosa_uint8_t *)concat_str, concat_len);
    mbedtls_sha1_finish(&context, sha1_hash);

    //Free the memory of the concatenated string
    WEB_FREE_PTR(concat_str);

    //Calculate the SHA1 hash value in Base64 encoding
    int   base64_len = (20 * 4 / 3 + 3);  //Estimate the Base64 encoded length based on the SHA1 length, plus possible padding characters
    char *base64_encoded = qosa_malloc(base64_len);
    if (base64_encoded == QOSA_NULL)
    {
        QLOGE("Memory allocation failed.\n");
        return QOSA_NULL;
    }
    qosa_memset(base64_encoded, 0, base64_len);
    ret = qcm_base64_encode((char *)sha1_hash, sizeof(sha1_hash), base64_encoded, &base64_len, QOSA_FALSE);
    QLOGD("base64_encoded=%s,%d,%d", base64_encoded, ret, sizeof(sha1_hash));

    return base64_encoded;
}

/**
 * Add custom headers to the HTTP request.
 *
 * @param client_ptr points to a pointer of the qcm_web_client_t structure, containing client configuration and other information.
 * @param buf is used to store the buffer for header information.
 * @param buf_len The length of the buffer.
 *
 * This function iterates through the list of custom headers defined in the configuration and copies them into the provided buffer.
 * Each header ends with "\r\n". If the original header information does not end with "\r\n", the function will automatically add it.
 */
static int qcm_ws_http_add_customer_header(qcm_web_client_t *client_ptr, char *buf, int buf_len)
{
    qcm_web_config_internal_t *cfg_ptr = QOSA_NULL;      //Configuration pointer
    qosa_q_type_t             *header_list = QOSA_NULL;  //Head list pointer
    qcm_web_header_list_t     *header_ptr = QOSA_NULL;   //Current head pointer
    int                        write_len = 0;            //Written length

    QOSA_UNUSED(buf_len);
    //Get configuration information
    cfg_ptr = qcm_ws_config_get_ptr(client_ptr->config_id);
    if (cfg_ptr == QOSA_NULL)
    {
        return 0;
    }

    header_list = &cfg_ptr->config.header_list;

    //Check if the header list is empty
    if (qosa_q_is_initialized(header_list) == QOSA_FALSE)
    {
        QLOGE("Header list has not been initialized!");
        return 0;
    }

    //Get the first element of the header list
    header_ptr = (qcm_web_header_list_t *)qosa_q_check(header_list);
    qcm_web_header_list_t *temp_ptr = QOSA_NULL;

    //Traverse the header list, use memcpy to prevent the presence of 0x00 in the string.
    while (header_ptr != QOSA_NULL)
    {
        //Check if the current header data ends with "\r\n"
        int ends_with_crlf = (header_ptr->data.value[header_ptr->data.value_len - 2] == '\r' && header_ptr->data.value[header_ptr->data.value_len - 1] == '\n');

        //Header field format:
        //"Field name: Field content\r\n"

        //1. First copy the "field name" to the buffer
        qosa_memcpy(buf + write_len, header_ptr->data.key, header_ptr->data.key_len);
        write_len += header_ptr->data.key_len;

        //2. Then copy ": " after the "field name"
        qosa_memcpy(buf + write_len, ": ", 2);
        write_len += 2;

        //3. Finally, copy the current "field content" to the buffer.
        qosa_memcpy(buf + write_len, header_ptr->data.value, header_ptr->data.value_len);
        write_len += header_ptr->data.value_len;

        //4. If the current header data does not end with "\r\n", append "\r\n"
        if (!ends_with_crlf)
        {
            qosa_memcpy(buf + write_len, "\r\n", 2);
            write_len += 2;
        }

        //1. 2. 3. 4. After completion, form a complete:
        //"Field name: Field content\r\n"

        //Move to the next header element
        temp_ptr = qosa_q_next(header_list, &header_ptr->list);
        header_ptr = temp_ptr;
    }
    return write_len;
}

/**
 * Build WebSocket upgrade request
 *
 * This function is used to construct an HTTP request for WebSocket connection upgrade based on the given client configuration and URL.
 *
 * @param client_ptr points to a qcm_web_client_t structure, containing the client's configuration ID and the URL to be connected.
 * @param buf Pointer to the buffer used to store the constructed HTTP request string.
 * @return The function returns 0 to indicate success, and returns other error codes to indicate failure (such as invalid parameters or memory allocation failure).
 */
static int qcm_construct_websocket_upgrade_request(qcm_web_client_t *client_ptr)
{
    qcm_web_config_internal_t *cfg_ptr = QOSA_NULL;    //Client configuration pointer
    char                      *buf = QOSA_NULL;        //Buffer for constructing HTTP requests
    int                        buf_len = 0;            //Buffer length
    qosa_uint8_t               rand_buf[32] = {0};     //Buffer for generating random strings
    char                       rand_base64[32] = {0};  //Store the Base64-encoded random string
    int                        rand_len = 32;          //Length of the random string
    int                        max_len = 0;            //Maximum length of the buffer
    int                        customer_len = 0;       //Custom header length
    int                        customer_count = 0;     //Number of custom header fields
    int                        ret = 0;                //Function Return Value

    QLOGD("config_id=%d", client_ptr->config_id);
    //Get client configuration
    cfg_ptr = qcm_ws_config_get_ptr(client_ptr->config_id);
    if (cfg_ptr == QOSA_NULL)
    {
        //If the configuration acquisition fails, an invalid parameter error code is returned.
        QLOGE("QCM_WEB_ERR_INVAL_PARM");
        return QCM_WEB_ERR_INVAL_PARM;
    }

    //Header field format:
    //"Field name: Field content\r\n"

    //Get custom header length
    customer_len = qcm_ws_header_length_get(&cfg_ptr->config.header_list);

    //Get the number of custom header fields
    customer_count
        = qosa_q_cnt(&cfg_ptr->config.header_list);  //Record how many user-defined fields there are, each field adds an extra 10 bytes, used to add extra characters for each field (such as: \r \n characters)

    QLOGD("customer_len: %d, customer_count: %d", customer_len, customer_count);

    //Calculate the maximum length of the buffer
    if (customer_len > 0)
    {
        max_len = (2048 + customer_len + customer_count * 10);
    }
    else
    {
        max_len = 2048;
    }
    //Print maximum length, for debugging
    QLOGD("max_len=%d", max_len);

    //Allocate memory for the buffer
    buf = qosa_malloc(max_len);
    if (buf == QOSA_NULL)
    {
        //If memory allocation fails, return the insufficient memory error code
        QLOGE("QCM_WEB_ERR_NOMEM");
        return QCM_WEB_ERR_NOMEM;
    }

    //Generate a 16-byte random string
    qcm_ws_get_random(rand_buf, 16);

    //Base64 encode a random string
    ret = qcm_base64_encode((char *)rand_buf, 16, rand_base64, &rand_len, QOSA_FALSE);
    //Print encoding results for debugging
    QLOGD("rand_base64=%d,%s", ret, rand_base64);
    qosa_memset(client_ptr->web_key, 0, sizeof(client_ptr->web_key));
    qosa_snprintf(client_ptr->web_key, sizeof(client_ptr->web_key), "%s", rand_base64);

    //When the client initiates a WebSocket connection, it includes a randomly generated Sec-WebSocket-Key value in the request header, which is a Base64-encoded string.
    //Upon receiving this request, the server concatenates the Sec-WebSocket-Key with a predefined globally unique identifier (GUID), then performs an SHA1 hash on the concatenated string, and encodes the result in Base64 to obtain the Sec-WebSocket-Accept.

    //Calculate WebSocket accept key
    //qcm_compute_sec_websocket_accept(rand_base64); Here, there is no need to execute qcm_compute_sec_websocket_accept, comment it out (originally, after execution here, the return value was not released, which would cause a memory leak!)

    //Add corresponding Host and port information based on the port number
    if (client_ptr->url.port != 80 && client_ptr->url.port != 443)
    {
        buf_len += qosa_snprintf(
            buf,
            max_len,
            "GET %s HTTP/1.1\r\n"
            "Host: %s:%d\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n"
            "Sec-WebSocket-Key: %s\r\n"
            "Sec-WebSocket-Version: %d\r\n",
            client_ptr->url.path,
            client_ptr->url.host,
            client_ptr->url.port,
            rand_base64,
            QCM_WEB_VERSION
        );
    }
    else
    {
        buf_len += qosa_snprintf(
            buf,
            max_len,
            "GET %s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n"
            "Sec-WebSocket-Key: %s\r\n"
            "Sec-WebSocket-Version: %d\r\n",
            client_ptr->url.path,
            client_ptr->url.host,
            rand_base64,
            QCM_WEB_VERSION
        );
    }

    //If the WebSocket extension is enabled, add the extension header
    if (cfg_ptr->config.conn.extension_en)
    {
        buf_len += qosa_snprintf(buf + buf_len, max_len - buf_len, "Sec-WebSocket-Extensions: %s\r\n", cfg_ptr->config.conn.extension);
    }
    //If the WebSocket subprotocol is enabled, add the subprotocol header
    if (cfg_ptr->config.conn.subprot_en)
    {
        buf_len += qosa_snprintf(buf + buf_len, max_len - buf_len, "Sec-WebSocket-Protocol: %s\r\n", cfg_ptr->config.conn.subprot);
    }

    //Add custom HTTP headers
    buf_len += qcm_ws_http_add_customer_header(client_ptr, buf + buf_len, max_len - buf_len);
    //Add HTTP header terminator
    buf_len += qosa_snprintf(buf + buf_len, max_len - buf_len, "\r\n");
    QLOGD("header len=%d", buf_len);

    //Set the client status to HTTP handshake state, and send the HTTP request
    client_ptr->status = QCM_WEB_STATUS_HTTP_HANDSHAKE;
    ret = qcm_socket_send(client_ptr->sock_hd, buf, buf_len);
    //Print the sending result for debugging
    QLOGD("sock_hd=%d,send len=%d", client_ptr->sock_hd, ret);
    if (ret < 0)
    {
        //Send failed, return error code
        QLOGE("QCM_WEB_ERR_HTTP_UPGRADE_FAIL");
        WEB_FREE_PTR(buf);
        return QCM_WEB_ERR_HTTP_UPGRADE_FAIL;
    }
    if (ret != buf_len)
    {
        //The length of the sent data does not match the expected length, returning an error code.
        QLOGE("QCM_WEB_ERR_HTTP_UPGRADE_FAIL");
        WEB_FREE_PTR(buf);
        return QCM_WEB_ERR_HTTP_UPGRADE_FAIL;
    }

    //Send successful, return success code
    WEB_FREE_PTR(buf);
    return QCM_WEB_ERR_OK;
}

/**
 * Parsing WebSocket upgrade response
 *
 * This function is used to parse the WebSocket upgrade response returned by the server to determine whether the upgrade request was successful.
 *
 * @param client_ptr points to a structure of type qcm_web_client_t, containing information such as the client's configuration ID and socket file descriptor.
 * @return The function returns an integer value indicating the parsing result. Returning QCM_WEB_ERR_OK indicates a successful upgrade; other error codes indicate failure.
 */
static int qcm_parse_websocket_upgrade_response(qcm_web_client_t *client_ptr)
{
    char *buf = QOSA_NULL;              //Buffer for receiving server responses
    int   buf_len = 2048;               //Buffer length
    int   ret = QCM_WEB_ERROR_UNKNOW;   //Function return value
    int   read_sz = 0;                  //qcm_socket_read reads the length of data

    char *web_response_hd = QOSA_NULL;  //Response header pointer
    int   web_response_hd_len = 0;      //Response header length
    char *frame = QOSA_NULL;            //Data frame pointer
    int   frame_len = 0;                //Whether there is data after the response header, and if so, the length of the data frame needs to be recorded.

    QLOGD("config_id=%d", client_ptr->config_id);
    //Get client configuration

    //Allocate buffer for receiving response
    buf = (char *)qosa_malloc(buf_len + 1);
    if (buf == QOSA_NULL)
    {
        QLOGE("no mem");
        return QCM_WEB_ERR_NOMEM;
    }

    //Read response data from the socket
    qosa_memset(buf, 0x00, buf_len + 1);
    read_sz = qcm_socket_read(client_ptr->sock_hd, buf, buf_len);
    QLOGD("read_sz=%d", read_sz);
    if (read_sz < 0)
    {
        //Read failed, return HTTP upgrade failure error code
        QLOGE("upgrade fail");
        WEB_FREE_PTR(buf);
        return QCM_WEB_ERR_HTTP_UPGRADE_FAIL;
    }

    QLOGD("buf=%s", buf);

    //The response header may contain user-defined fields, which are meaningless for parsing but can cause the length to exceed the buffer length `buf_len` of a single `qcm_socket_read` operation. Alternatively, the complete response header might be split and sent across multiple packets (i.e., the entire response header is not transmitted in a single complete packet).
    //In this case, it is necessary to execute qcm_parse_websocket_upgrade_response multiple times to read the complete response header.
    //client_ptr->web_response_hd is designed to handle this situation. If the entire header cannot be read at once due to buf_len, the currently read header information needs to be recorded and concatenated with the header information read next time, so that a complete response header can be formed.
    //After composing a complete response header, proceed to parse the websocket header.
    if (QOSA_NULL == client_ptr->web_response_hd)
    {
        web_response_hd_len = read_sz;
        web_response_hd = (char *)qosa_malloc(web_response_hd_len + 1);
        if (QOSA_NULL == web_response_hd)
        {
            QLOGE("no mem");
            WEB_FREE_PTR(buf);
            return QCM_WEB_ERR_NOMEM;
        }
        qosa_memset(web_response_hd, 0x00, read_sz + 1);

        qosa_memcpy(web_response_hd, buf, read_sz);
    }
    else
    {
        web_response_hd_len = client_ptr->web_response_hd_len + read_sz;
        web_response_hd = (char *)qosa_realloc(client_ptr->web_response_hd, web_response_hd_len + 1);
        if (QOSA_NULL == web_response_hd)
        {
            QLOGE("no mem");
            WEB_FREE_PTR(client_ptr->web_response_hd);
            WEB_FREE_PTR(buf);

            client_ptr->web_response_hd = QOSA_NULL;
            client_ptr->web_response_hd_len = 0;
            return QCM_WEB_ERR_NOMEM;
        }
        qosa_memset(web_response_hd + client_ptr->web_response_hd_len, 0x00, read_sz + 1);

        qosa_memcpy(web_response_hd + client_ptr->web_response_hd_len, buf, read_sz);
    }

    //Update the response header record for client_ptr
    QLOGD("web_response_hd: %p, web_response_hd_len: %d", web_response_hd, web_response_hd_len);
    client_ptr->web_response_hd = web_response_hd;
    client_ptr->web_response_hd_len = web_response_hd_len;

    //The data in buf has been copied to web_response_hd and can be released directly.
    WEB_FREE_PTR(buf);
    //web_response_hd records the currently more complete header information after this read.

    //Check if the response contains "\r\n\r\n" to determine the end position of the response header.
    if (qosa_strstr(web_response_hd, "\r\n\r\n") != QOSA_NULL)
    {
        char *state_line = web_response_hd;  //Pointer to the response status line
        int   response_code = 0;             //HTTP Response Code
        char  separator = 0;                 //The separator between the version number and the response code in the status line
        int   major_version = 0;             //The major version number in the HTTP version
        int   minor_version = 0;             //Minor version number in the HTTP version
        int   acc_result = 1;                //Verification result of the "Sec-WebSocket-Accept" header field
        int   upgrade_result = 0;            //"Upgrade" header field check result
        int   connection_result = 0;         //"Connection" header field check result

        //Parse response status line
        ret = qosa_sscanf(state_line, "HTTP/%1d.%1d%c%d", &major_version, &minor_version, &separator, &response_code);
        if (ret != 4)
        {
            //Parsing failed, returning HTTP upgrade failure error code
            QLOGE("upgrade fail");
            WEB_FREE_PTR(web_response_hd);
            client_ptr->web_response_hd = QOSA_NULL;
            client_ptr->web_response_hd_len = 0;
            return QCM_WEB_ERR_HTTP_UPGRADE_FAIL;
        }
        QLOGD("response_code=%d", response_code);

        //The websocket response header must include (RFC 6455):
        //Connection: upgrade    The connection method is upgrade, indicating that the server supports protocol upgrade.
        //Upgrade: websocket     The server wishes to upgrade the connection to the WebSocket protocol.
        //Sec-WebSocket-Accept   The response value generated by the server after verifying the client's Sec-WebSocket-Key, used to verify whether the client is legitimate.

        //Traverse the response header fields (check the response header line by line, each line ends with \r\n, and the entire response header ends with \r\n\r\n), verify whether the necessary header fields exist and meet expectations.
        while (state_line != QOSA_NULL)
        {
            state_line = qosa_strstr(state_line, "\r\n");
            state_line += 2;
            QLOGD("state_line=%s", state_line);

            if (*state_line == '\r' && *(state_line + 1) == '\n')
            {
                state_line += 2;

                //Upon detecting \r\n\r\n, it indicates that the parsing of the WebSocket upgrade request response is complete.
                //This situation may occur: the WebSocket upgrade request response and WebSocket data frames are sent together.
                //Afterwards, the received WebSocket data frame should be notified to the upper layer and output.

                QLOGD("state_line - web_response_hd = %d", state_line - web_response_hd);
                if ((int)(state_line - web_response_hd) < web_response_hd_len)
                {
                    //websocket upgrade request response, followed by additional data, possibly data frames
                    frame = state_line;
                    frame_len = web_response_hd_len - (int)(state_line - web_response_hd);
                }
                break;
            }
            if (qosa_strncasecmp(state_line, "Sec-WebSocket-Accept: ", qosa_strlen("Sec-WebSocket-Accept: ")) == 0)
            {
                char *end = qosa_strstr(state_line, "\r\n");
                if (end != QOSA_NULL)
                {
                    //Extract the value of the "Sec-WebSocket-Accept" header field from the response
                    qosa_memset(client_ptr->web_accept_key, 0, sizeof(client_ptr->web_accept_key));
                    int len = (end - state_line - qosa_strlen("Sec-WebSocket-Accept: "));
                    len = MIN(len, (int)sizeof(client_ptr->web_accept_key) - 1);
                    QLOGD("len=%d", len);
                    qosa_snprintf(client_ptr->web_accept_key, len + 1, "%s", state_line + qosa_strlen("Sec-WebSocket-Accept: "));
                    QLOGD("Sec-WebSocket-Accept=%s", client_ptr->web_accept_key);
                }
            }
            if (qosa_strncasecmp(state_line, "Upgrade: websocket", qosa_strlen("Upgrade: websocket")) == 0)
            {
                //Check if the "Upgrade" header field specifies websocket
                QLOGD("upgrade_value=1");
                upgrade_result = 1;
            }
            if (qosa_strncasecmp(state_line, "Connection: Upgrade", qosa_strlen("Connection: Upgrade")) == 0)
            {
                //Check if the "Connection" header field specifies Upgrade
                QLOGD("connection_value=1");
                connection_result = 1;
            }
        }

        //Calculate and compare the value of the "Sec-WebSocket-Accept" header field
        char *base64 = qcm_compute_sec_websocket_accept(client_ptr->web_key);
        if (base64 != QOSA_NULL)
        {
            QLOGD("web_key base64=%s", base64);
            if (qosa_strncasecmp(base64, client_ptr->web_accept_key, qosa_strlen(base64)) == 0)
            {
                acc_result = 1;
            }
            else
            {
                //For local verification, do not worry about the key; during local verification, the value can be manually set to 1.
                acc_result = 0;
            }
        }
        WEB_FREE_PTR(base64);

        //Determine whether the upgrade was successful based on the header field check result.
        if (acc_result && upgrade_result && connection_result)
        {
            ret = QCM_WEB_ERR_OK;
        }
        else
        {
            ret = QCM_WEB_ERR_HTTP_UPGRADE_FAIL;
        }

        //Header field check successful, and there is still data following the response header.
        //Refer to the implementation of qcm_ws_socket_read
        if (QCM_WEB_ERR_OK == ret && 0 < frame_len)
        {
            char *data_buf = QOSA_NULL;
            int   data_len = 0;

            data_buf = (char *)qosa_malloc(frame_len + 1);
            if (QOSA_NULL == data_buf)
            {
                QLOGE("no mem");
                WEB_FREE_PTR(web_response_hd);
                client_ptr->web_response_hd = QOSA_NULL;
                client_ptr->web_response_hd_len = 0;
                return QCM_WEB_ERR_NOMEM;
            }
            qosa_memset(data_buf, 0x00, frame_len + 1);

            //Parsing WebSocket frames
            qcm_ws_parse_frame(client_ptr->client_id, frame, frame_len, data_buf, &data_len);

            //The payload content of the websocket data frame is stored in data_buf, and if data_len > 0, this content is directly reported.
            if (data_len > 0)
            {
                qosa_buffer_block_t  *read_data_item = QOSA_NULL;
                web_transfer_session *transfer_session = QOSA_NULL;

                transfer_session = &client_ptr->transfer_session;
                read_data_item = qosa_buffer_new_block();
                if (read_data_item != QOSA_NULL)
                {
                    //Add the data to the receive queue
                    qosa_buffer_pushdown_tail(&read_data_item, data_buf, data_len);
                    transfer_session->current_recv_cnt = data_len;
                    qosa_buffer_enqueue(&(transfer_session->recv_wm_ptr), &read_data_item);

                    // qosa_buffer_enqueue/memoryi_enqueue => buffer_wm_check_levels => ws_rx_wm_non_empty_cb
                    //Here, the client_ptr->status is not QCM_WEB_STATUS_WEBSOCKET
                    //ws_rx_wm_non_empty_cb will not report the recv URC here (buffer mode QCM_WEB_MODE_BUFFER)
                    //Subsequently, in the `qcm_ws_tcp_open_cnf` event, a URC notification for recv is required.

                    QLOGD("recv");
                }

                //The processing of additional control frames after the response header is handled in qcm_ws_tcp_open_cnf (refer to qcm_ws_socket_read for processing).
            }

            WEB_FREE_PTR(data_buf);
        }

        //The response headers have been fully parsed (regardless of success, only concerned with completion), the response header records of client_ptr can be released.
        WEB_FREE_PTR(web_response_hd);
        client_ptr->web_response_hd = QOSA_NULL;
        client_ptr->web_response_hd_len = 0;
    }
    else
    {
        //web_response_hd did not match \r\n\r\n, indicating the current response header information is incomplete, and further reading is required.

        int web_response_hd_max = 2048 * 10;  //Avoid unlimited response headers, limit the maximum response header length to web_response_hd_max

        if (web_response_hd_max <= web_response_hd_len)
        {
            //The length of the received response header has exceeded web_response_hd_max, and a complete header has not yet been formed, which can be considered an abnormal response header message.
            WEB_FREE_PTR(web_response_hd);
            client_ptr->web_response_hd = QOSA_NULL;
            client_ptr->web_response_hd_len = 0;
            ret = QCM_WEB_ERROR_UNKNOW;

            QLOGE("Response header exception");
        }
        else
        {
            ret = QCM_WEB_ERR_OPERATION_PENDING;
            QLOGE("Response header parsing is not complete");
        }
    }

    return ret;
}

/**
 * Send an HTTP request to the specified client and attempt to upgrade to a WebSocket connection.
 *
 * @param msg is a pointer to the message structure containing the client ID and other possible request parameters.
 * @return Always returns QCM_WEB_ERR_OK, indicating that the function executed successfully. Actual error handling is performed inside the function.
 */
qcm_web_err_e qcm_ws_http_request(qcm_web_msg_t *msg)
{
    int               client_id = 0;
    int               ret = QCM_WEB_ERR_OK;
    qcm_web_client_t *client_ptr = QOSA_NULL;

    QOSA_UNUSED(client_ptr);

    //Get the client ID from the message
    client_id = msg->param1;
    //Get client pointer based on client ID
    client_ptr = qcm_ws_client_get_ptr(client_id);
    QLOGD("client_id=%d,%d,%d", client_id, client_ptr->client_id, client_ptr->config_id);

    //Build WebSocket upgrade request
    ret = qcm_construct_websocket_upgrade_request(client_ptr);
    if (ret != QCM_WEB_ERR_OK)
    {
        //If the build request fails, set the result code and send an error acknowledgment message.
        client_ptr->result = ret;
        qcm_ws_msg_send(QCM_WEB_MSG_TCP_OPEN_CNF, client_id, QOSA_NULL);
    }

    return QCM_WEB_ERR_OK;
}

/**
 * Processing HTTP responses for WebSocket.
 *
 * @param msg points to a pointer to a structure containing WebSocket message parameters.
 * @return Returns the error code for the processing result, where OK indicates success.
 */
qcm_web_err_e qcm_ws_http_response(qcm_web_msg_t *msg)
{
    int               client_id = 0;           //Client ID
    int               ret = 0;                 //Function Return Codes
    qcm_web_client_t *client_ptr = QOSA_NULL;  //Pointer to WebSocket client information

    //The client_ptr is not used in this function, so it is marked as unused.
    QOSA_UNUSED(client_ptr);

    //Get the client ID from the message
    client_id = msg->param1;
    //Get client pointer based on client ID
    client_ptr = qcm_ws_client_get_ptr(client_id);
    //Print client ID and related configuration ID information for debugging
    QLOGD("client_id=%d,%d,%d", client_id, client_ptr->client_id, client_ptr->config_id);

    //Initialize the watermark for sending and receiving
    qcm_ws_tx_rx_watermark_init(client_id);
    //Parsing WebSocket upgrade response
    ret = qcm_parse_websocket_upgrade_response(client_ptr);

    QLOGD("ret: %d", ret);

    if (ret == QCM_WEB_ERR_OPERATION_PENDING)
    {
        //Parsing of the WebSocket upgrade response is not yet complete; there is no need to change the current state, just wait for the parsing to finish.
        return QCM_WEB_ERR_OK;
    }
    else if (ret == QCM_WEB_ERR_OK)
    {
        //Parsing successful, setting client status to WebSocket state
        client_ptr->status = QCM_WEB_STATUS_WEBSOCKET;
    }
    else
    {
        //Parsing failed, set client status to HTTP handshake failure state
        client_ptr->status = QCM_WEB_STATUS_HTTP_HANDSHAKE_FAIL;
    }
    //Set client result code
    client_ptr->result = ret;
    //Send TCP open confirmation message
    qcm_ws_msg_send(QCM_WEB_MSG_TCP_OPEN_CNF, client_id, QOSA_NULL);
    return QCM_WEB_ERR_OK;
}
