/*
 * Portions of this file are derived from curl/libcurl's lib/ftp.h.
 *
 * Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * should be included in the curl distribution. The terms are also
 * available at https://curl.se/docs/copyright.html.
 *
 * For qurl redistribution, see:
 * - qos_components/components/qurl/THIRD_PARTY_NOTICES.md
 * - qos_components/components/qurl/licenses/curl.txt
 */
/** @file         qurl_ftp_def.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-11-28 20:34:13
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#ifndef QURL_FTP_DEF_H
#define QURL_FTP_DEF_H

#include "qurl_config.h"
#include "qurl_code_in.h"

#if QURL_CFG_ENABLE_FTP

#include "qurl_pingpong.h"

typedef enum
{
    QURL_FTP_STATE_STOP,          /*!< Idle state, no operation is performed, the state machine is stopped.*/
    QURL_FTP_STATE_WAIT220,       /*!< Wait for the initial 220 response after connection.*/
    QURL_FTP_STATE_AUTH,          /*!< Perform authentication.*/
    QURL_FTP_STATE_USER,          /*!< Send username.*/
    QURL_FTP_STATE_PASS,          /*!< Send password.*/
    QURL_FTP_STATE_ACCT,          /*!< Send account.*/
    QURL_FTP_STATE_PBSZ,          /*!< Set the data encryption buffer size.*/
    QURL_FTP_STATE_PROT,          /*!< Set the protection level for the data connection.*/
    QURL_FTP_STATE_CCC,           /*!< Clear the protection level of the data connection.*/
    QURL_FTP_STATE_PWD,           /*!< Get the current working directory.*/
    QURL_FTP_STATE_SYST,          /*!< Get remote system information.*/
    QURL_FTP_STATE_NAMEFMT,       /*!< Set the remote file name format.*/
    QURL_FTP_STATE_QUOTE,         /*!< Wait for the response of the command to be sent (the command starts with QUOTE).*/
    QURL_FTP_STATE_RETR_PREQUOTE, /*!< Execute command list before downloading files.*/
    QURL_FTP_STATE_STOR_PREQUOTE, /*!< Execute command list before uploading files.*/
    QURL_FTP_STATE_POSTQUOTE,     /*!< Execute the command list after the transmission is completed.*/
    QURL_FTP_STATE_CWD,           /*!< Change the current working directory.*/
    QURL_FTP_STATE_MKD,           /*!< If the directory does not exist, create the directory.*/
    QURL_FTP_STATE_MDTM,          /*!< Get the timestamp of the remote file.*/
    QURL_FTP_STATE_TYPE,          /*!< Set the file transfer type.*/
    QURL_FTP_STATE_LIST_TYPE,     /*!< Set the file list transfer type.*/
    QURL_FTP_STATE_RETR_TYPE,     /*!< Set the download file transfer type.*/
    QURL_FTP_STATE_STOR_TYPE,     /*!< Set the upload file transfer type.*/
    QURL_FTP_STATE_SIZE,          /*!< Get remote file size (for HEAD-like requests).*/
    QURL_FTP_STATE_RETR_SIZE,     /*!< Get the remote file size (used for downloading files).*/
    QURL_FTP_STATE_STOR_SIZE,     /*!< Get the size of the uploaded file.*/
    QURL_FTP_STATE_REST,          /*!< Check if the server supports resumable upload (used for requests similar to HEAD).*/
    QURL_FTP_STATE_RETR_REST,     /*!< Request to resume transmission when downloading a file.*/
    QURL_FTP_STATE_PORT,          /*!< General state machine, used for PORT, LPRT, and EPRT commands, checking counter 1.*/
    QURL_FTP_STATE_PRET,          /*!< A general state machine used for PRET RETR, PRET STOR, and PRET LIST/NLST commands.*/
    QURL_FTP_STATE_PASV,          /*!< General state machine, used for PASV and EPSV commands, checking counter 1.*/
    QURL_FTP_STATE_LIST,          /*!< A general-purpose state machine for LIST, NLST, or custom list commands.*/
    QURL_FTP_STATE_RETR,          /*!< A general-purpose state machine for downloading files.*/
    QURL_FTP_STATE_STOR,          /*!< A general-purpose state machine for uploading files.*/
    QURL_FTP_STATE_QUIT,          /*!< Exit the FTP connection after completing the transmission.*/
    QURL_FTP_STATE_LAST           /*!< Never to be used.*/
} qurl_ftp_state_e;

/*Core User Configuration*/
/**
 * @struct  qurl_core_usr_cfg_ftp_t
 * @brief ftp exclusive user configuration information
 */
struct qurl_core_usr_cfg_ftp_s
{
    qurl_slist_ctrl_t *quote_slist; /*!< The command linked list entered by the user. After the FTP connection is established, and before each file transfer, the specified QUOTE command is sent.*/
    qurl_slist_ctrl_t *postquote_slist; /*!< The command linked list entered by the user. After the FTP transfer is completed but while still in the connected state, send the specified QUOTE command.*/
    qurl_slist_ctrl_t *prequote_slist; /*!< The command list entered by the user. Send the specified QUOTE command after the FTP connection is established, but before actual file transfer begins.*/
    char *ftp_port_ptr;          /*!< The port number specified in FTP active mode. Accepts format: (ipv4|ipv6|domain name|network interface)?(:port(-range)?)? .alloc*/
    unsigned char
        ftp_create_missing_dirs; /*!< 1. Create a non-existent directory. 2. Also allow MKD to fail once (because other sessions may create this directory, so one failure is permitted). Used for FTP(S) and SFTP.*/
    unsigned char ftp_filemethod; /*!< The method to obtain the file. qurl_ftp_filemethod_e*/
    unsigned char ftp_auth;       /*!< ftp AUTH type. Refer to qurl_ftp_auth_e*/
    unsigned char ftp_ccc;        /*!< The CCC behavior option for FTP. Refer to qurl_ftp_ccc_e*/
};
typedef struct qurl_core_usr_cfg_ftp_s qurl_core_usr_cfg_ftp_t;

/*core configuration*/
/**
 * @struct  qurl_core_cfg_ftp_t
 * @brief FTP exclusive configuration information
 */
struct qurl_core_cfg_ftp_s
{
    char *ftp_most_recent_entrypath_ptr; /*!< Stores the location of the most recently set FTP entry path. Not allocated.*/
};
typedef struct qurl_core_cfg_ftp_s qurl_core_cfg_ftp_t;

/*Data Structure*/
/**
 * @struct qurl_ftp_core_t
 * @brief Data related to the FTP protocol stack is stored in `qurl_core_ctrl_t` within the core.
 * @note Connection-related data should be placed in qurl_ftp_ctrl_t, because qurl_ftp_ctrl_t will be registered in the connection.
 */
struct qurl_ftp_core_s
{
    char *path; /*!< Path in the URL*/

    /*Transmission Content Mode*/
    qurl_pp_transfer_e transfer; /*!< Content to be transmitted*/
};
typedef struct qurl_ftp_core_s qurl_ftp_core_t;

/**
 * @struct qurl_ftp_ctrl_bits_t
 * @brief Some flags of the FTP control block
 */
struct qurl_ftp_ctrl_bits_s
{
    qurl_bit_t ctrl_valid; /*!< Control connection validity. Avoid generating invalid network traffic. For example, referring to RFC959, when the control connection is disconnected, the data connection should also be disconnected.*/
    qurl_bit_t cwd_done;       /*!< Already in the target path.*/
    qurl_bit_t cwd_faild;      /*!< CWD command failure flag. If the CWD command fails, set to TRUE, because we must prevent caching the current directory.*/
    qurl_bit_t wait_data_conn; /*!< Wait for data connection*/
    qurl_bit_t dont_check; /*!< Set to true to skip the final (post-transfer) file size and 226/250 status checks. It should still read the line, just ignore the result.*/
    qurl_bit_t pp_valid;   /*!< Whether pp is valid*/
};
typedef struct qurl_ftp_ctrl_bits_s qurl_ftp_ctrl_bits_t;

/**
 * @struct qurl_ftp_ctrl_t
 * @brief Data structure for FTP protocol control
 */
struct qurl_ftp_ctrl_s
{
    /*Status*/
    qurl_ftp_state_e state;
    qurl_ftp_state_e
        state_saved; /*!< Indicates the saved transmission type, which is reloaded after establishing a data connection. This variable is typically used to save the current transmission type before establishing a data connection and restore it after the connection is established. This ensures data consistency during transmission.*/

    /* pingpong */
    qurl_pp_ctrl_t pp_ctrl;

    /*Transmission Related*/
    char transfer_type; /*!< Transmission type. Its value is usually 'A', 'I', or zero.*/

    /*Running Information*/
    /** Path Related*/
    char  *entrypath_ptr; /*!< PWD command result after login, alloc*/
    char  *prevpath_ptr;  /*!< The URL path decoded from the last transmission. Essentially, it is the current working directory for FTP. alloc*/
    char  *file_ptr;      /*!< Target file, may include full path, alloc*/
    char **dirs_pptr;     /*!< The name of each directory in the path that needs to continue switching*/
    int    dir_depth;     /*!< Path depth of dirs_pptr*/
    /** Download Information*/
    long known_filesize; /*!< -1: Unknown download size, >=0 Known*/
    /** Data Connection*/
    char          *newhost_ptr; /*!< The host address for the data connection*/
    unsigned short newport;     /*!< Data connection port number*/

    /*Operating Data*/
    int cwd_count; /*!< Number of CWD commands already sent*/
    /** General-Purpose Counter*/
    int count1;
    int count2;
    int count3;

    qurl_ftp_ctrl_bits_t bits;
};
typedef struct qurl_ftp_ctrl_s qurl_ftp_ctrl_t;

#endif

#endif /* Head define end*/
