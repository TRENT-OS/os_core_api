/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief       Log file client flow control.
 *
 * @details     The functions of this level serve to control the flow of log
 *              file client.
 *
 *              This layer only defines the interface functions.
 *
 *              The implementation itself is not place here.
 *
 * @ingroup     OS_LoggerFileClient
*/
#pragma once
#include "OS_Error.h"
#include <stdint.h>
#include <stdbool.h>


/**
 * @details OS_LoggerFileClientCallback_read_t defines the interface for
 *          function pointer to read a log file from log server.
 *
 * @param   filename:       name of log file
 * @param   offset:         offset in log file
 * @param   len:            length of buffer
 * @param   log_file_size:  size of log file in bytes
 *
 * @return  The number of bytes read.
 *
 * @retval -1 - if the error occurred.
 * @retval  N - if success.
*/
typedef int64_t
(*OS_LoggerFileClientCallback_read_t)(
    const char* filename,
    uint64_t    offset,
    uint64_t    len,
    int64_t*    log_file_size);


/**
 * @details OS_LoggerFileClientCallback_Handle_t contain interface functions.
*/
typedef struct
{
    OS_LoggerFileClientCallback_read_t read_log_file;
} OS_LoggerFileClientCallback_Handle_t;


/**
 * @details %OS_LoggerFileClientCallback_ctor is the constructor.
 *
 * @param   self:           pointer to the class
 * @param   read_log_file:  function pointer to read log file function
 *
 * @return  An error code.
 *
 * @retval OS_ERROR_INVALID_PARAMETER - Read Callback is NULL.
 * @retval OS_SUCCESS                 - Initialization was successful.
*/
OS_Error_t
OS_LoggerFileClientCallback_ctor(
    OS_LoggerFileClientCallback_Handle_t* self,
    OS_LoggerFileClientCallback_read_t read_log_file);
