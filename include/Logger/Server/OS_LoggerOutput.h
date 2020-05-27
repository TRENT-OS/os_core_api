/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log server backend class definition
 */
#pragma once

/**
 * @defgroup    OS_LoggerOutput Log server backend interface
 *
 * @brief       Defined an generally class for log server backend
 *              implementation.
 *
 * @details     This is an definition of a class interface for concrete log
 *              server backend implementations.
 *
 *              A "OS_LoggerOutput_Handle_t" object provides the interface to a log
 *              server backend and at the same time is this object an observer.
 *
 *              Several observer can be assigned to one subject. To avoid the
 *              "malloc" function, objects of this type are defined externally.
 *
 *              To ensure that the observers are assigned to their subjects,
 *              they are assigned based on their nodes.
 *
 *              The auxiliary object OS_LoggerListT_t_Handle_t is also
 *              responsible for managing this nodes.
 *
 *              A log message should be printed on the log server backend.
 *              The handling of the format of the log is defined in the
 *              "OS_LoggerFormat_Handle_t" level.
 *
 *              This class is the consumer of the log formats and uses the log
 *              format only.
 *
 * @ingroup     OS_LoggerAbstractOutput
*/
#include "Logger/Server/OS_LoggerAbstractOutput.h"
#include "Logger/Server/OS_LoggerFormat.h"
#include "Logger/Server/OS_LoggerListT.h"


/**
 * @brief OS_LoggerOutput_Handle_t contains the vtable to its class.
 *
 * This layer is depend on the layers \link OS_LoggerNodeT_Handle_t \endlink ,
 * \link OS_LoggerListT_t_Handle_t \endlink and
 * \link OS_LoggerFormat_Handle_t \endlink.
 * \n
 * The "OS_LoggerFormat_Handle_t" layer is implemented as an pointer, so the
 * concrete implementation depends on this class itself.
 *
 * The log format is are interchangeable due to its special
 * implementation.
 *
 * @ingroup OS_LoggerOutput
*/
typedef struct
{
    OS_LoggerNodeT_Handle_t   node;       //!< node interface
    OS_LoggerFormat_Handle_t* log_format; //!< layer for log format

    const OS_LoggerAbstractOutput_vtable_t* vtable; //!< pointer to vtable
} OS_LoggerOutput_Handle_t;

