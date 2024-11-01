/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/**
 * @file
 * @brief Timestamp interface and implementation.
 */
#pragma once

/**
 * @defgroup    OS_LoggerTimestamp OS Logger Timestamp API
 *
 * @brief       OS Logger API library timestamp functionality
 *
 * @details     This object contain to create or convert a timestamp.
 *              \n
 *              It can be transformed a timestamp to human readable format and
 *              vice verse. Further it can be create a timestamp from the macro
 *              "__DATE__" and "__TIME".
 *              \n
 *              This class has been created for a time server. So the time
 *              server hosts the timestamp object as singleton.
 *
 * @ingroup     OS_LoggerServer
*/
#include "OS_Error.h"
#include <stdint.h>
#include <stdbool.h>


/**
 * @details OS_LoggerTimestamp_Handle defines the class datatype for timestamp.
 *
 * @ingroup OS_LoggerTimestamp
*/
typedef struct OS_LoggerTimestamp_Handle OS_LoggerTimestamp_Handle_t;

/**
 * @details OS_LoggerTime_Handle_t defines the class datatype for human readable
 *          format.
 *
 * @ingroup OS_LoggerTimestamp
*/
typedef struct OS_LoggerTime_Handle OS_LoggerTime_Handle_t;

/**
 * @details OS_LoggerTimestamp_create_t defines the interface for functions
 *          pointer to create a timestamp.
 *
 * @param   date:   string from the macro "__DATE"
 * @param   time:   string from the macro "__TIME"
 *
 * @return  An error code.
 *
 * @ingroup OS_LoggerTimestamp
*/
typedef OS_Error_t
(*OS_LoggerTimestamp_create_t)(const char* date, const char* time);


/**
 * @details OS_LoggerTimestamp_getTime_t defines the interface for functions
 *          pointer to convert a timestamp in human readable format.
 *
 * @param   t_stamp:    pointer to timestamp
 * @param   hours:      offset in hours for time shift
 * @param   tm:         destination struct to save the converted time
 *
 * @return  An error code.
 *
 * @ingroup OS_LoggerTimestamp
*/
typedef OS_Error_t
(*OS_LoggerTimestamp_getTime_t)(
    OS_LoggerTimestamp_Handle_t* t_stamp,
    uint8_t hours,
    OS_LoggerTime_Handle_t* tm);


/**
 * @details OS_LoggerTimestamp_getTimestamp_t defines the interface for
 *          functions pointer to convert from human readable format into
 *          timestamp.
 *
 * @param   tm:         source time
 * @param   t_stamp:    destination struct to save the timestamp
 *
 * @return  An error code.
 *
 * @ingroup OS_LoggerTimestamp
*/
typedef OS_Error_t
(*OS_LoggerTimestamp_getTimestamp_t)(
    OS_LoggerTime_Handle_t* tm,
    OS_LoggerTimestamp_Handle_t* t_stamp);


/**
 * @details OS_LoggerTime_Handle contain the parameters for human readable
 *          format.
 *
 * @ingroup OS_LoggerTimestamp
*/
struct OS_LoggerTime_Handle
{
    uint8_t  sec;   /**< second */
    uint8_t  min;   /**< minute */
    uint8_t  hour;  /**< hour */
    uint8_t  day;   /**< day in month */
    uint8_t  month; /**< month */
    uint16_t year;  /**< year */
};


/**
 * @details OS_LoggerTimestamp_vtable_t contain the member functions to his
 *          class.
 *
 * @ingroup OS_LoggerTimestamp
*/
typedef struct
{
    OS_LoggerTimestamp_create_t          create_timestamp;
    OS_LoggerTimestamp_getTime_t         get_time;
    OS_LoggerTimestamp_getTimestamp_t    get_timestamp;
} OS_LoggerTimestamp_vtable_t;


/**
 * @details OS_LoggerTimestamp_Handle contain the vtable to his class.
 *
 * @ingroup OS_LoggerTimestamp
*/
struct OS_LoggerTimestamp_Handle
{
    uint64_t timestamp;
    const OS_LoggerTimestamp_vtable_t* vtable;
};


/**
 * @details %OS_LoggerTimestamp_getInstance is a getter to instantiate this
 *          object.
 *
 *          This is an singleton implementation. It will be internally create a
 *          timestamp object.
 *
 *          If the object has been successfully created, the pointer to this
 *          object will returned, and NULL pointer in the other case.
 *
 * @return  pointer to OS_LoggerTimestamp_Handle object
 *
 * @retval  not NULL, if all allright
 *              NULL, if an error has been occurred
 *
 * @ingroup OS_LoggerTimestamp
*/
OS_LoggerTimestamp_Handle_t*
OS_LoggerTimestamp_getInstance(void);

/**
 * @details %OS_LoggerTimestamp_create create a timestamp from the given
 *          strings "__DATE__" and "__TIME__".
 *
 * @param   date:   string from the macro "__DATE"
 * @param   time:   string from the macro "__TIME"
 *
 * @return  An error code.
 *
 * @retval  OS_ERROR_INVALID_PARAMETER - One of the parameters is a NULL
 *                                       pointer.
 * @retval  OS_SUCCESS                 - Operation was successful.
 *
 * @ingroup OS_LoggerTimestamp
*/
OS_Error_t
OS_LoggerTimestamp_create(const char* date, const char* time);


/**
 * @details %OS_LoggerTimestamp_getTime convert a timestamp in human readable
 *          format.
 *
 * @param   t_stamp:    pointer to timestamp
 * @param   hours:      offset in hours for time shift
 * @param   tm:         destination struct to save the converted time
 *
 * @return  An error code.
 *
 * @retval  OS_ERROR_INVALID_PARAMETER - One of the parameters is a NULL
 *                                       pointer or `hours` is too large.
 * @retval  OS_SUCCESS                 - Operation was successful.
 *
 * @ingroup OS_LoggerTimestamp
*/
OS_Error_t
OS_LoggerTimestamp_getTime(
    OS_LoggerTimestamp_Handle_t* t_stamp,
    uint8_t hours,
    OS_LoggerTime_Handle_t* tm);


/**
 * @details %OS_LoggerTimestamp_getTimestamp convert from human readable format
 *          into timestamp.
 *
 * @param   tm:         source time
 * @param   t_stamp:    destination struct to save the timestamp
 *
 * @return  An error code.
 *
 * @retval  OS_ERROR_INVALID_PARAMETER - One of the parameters is a NULL
 *                                       pointer.
 * @retval  OS_SUCCESS                 - Operation was successful.
 *
 * @ingroup OS_LoggerTimestamp
*/
OS_Error_t
OS_LoggerTimestamp_getTimestamp(
    OS_LoggerTime_Handle_t* tm,
    OS_LoggerTimestamp_Handle_t* t_stamp);
