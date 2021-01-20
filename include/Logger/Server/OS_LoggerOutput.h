/*
 * Copyright (C) 2020-2021, HENSOLDT Cyber GmbH
 */

/**
 * @file
 * @brief       Observer pattern interface.
 *
 * @ingroup     OS_LoggerOutput
 */
#pragma once

/**
 * @defgroup    OS_LoggerOutput OS Logger Output API
 *
 * @brief       OS Logger API library observer pattern definition
 *
 * @details     This level provides a mechanism for an abstract observer
 *              interface.
 *
 *              The Observer pattern describes a specific relationship between
 *              objects.
 *
 *              The subject and the observer are provided, and the subject can
 *              have any number of observers referenced.
 *
 *              Every time the subject changes state, all observers are
 *              notified. In response, each observer will contact the subject to
 *              synchronize its own state with that of the subject.
 *
 *              The relation between objects is 1-to-n, so that in the event of
 *              the subject's state change, all dependent objects are notified
 *              accordingly and updated automatically.
 *
 *              ## Attendees
 *
 *              ### Subject
 *
 *              Knows its observers. A subject can be observed by any number of
 *              observers. Provides an interface for connecting and detaching
 *              observer objects.
 *
 *              ### Observer
 *
 *              Defines an update interface for the subject so that it is called
 *              by the subject, when subject's state changed.
 *
 * @see         OS_LoggerSubject_Handle_t
 *
 * @ingroup     OS_LoggerServer
*/
#include "OS_Error.h"
#include "Logger/Server/OS_LoggerFormat.h"
#include "Logger/Server/OS_LoggerListT.h"

typedef
struct OS_LoggerOutput_Handle
    OS_LoggerOutput_Handle_t;

/**
 * @brief   Observer's update callback.
 *
 * @return  An error code.
*/
typedef OS_Error_t
(*OS_LoggerOutput_update_t)(
    OS_LoggerOutput_Handle_t* self, //!< [in] This handle.
    void* data                      //!< [in] Data related to the update.
);

/**
 * @brief   The observer pattern handle.
*/
struct OS_LoggerOutput_Handle
{
    OS_LoggerNodeT_Handle_t   node;      //!< Linked list of observers node.
    OS_LoggerFormat_Handle_t* logFormat; /**< Format in which data will be
                                              presented. */
    OS_LoggerOutput_update_t update;     //!< Update callback.
};

/**
 * @brief  Constructs given observer object.
 *
 * @note   Child class must implement update() callback!
 *
 * @return An error code.
 *
 * @retval OS_ERROR_INVALID_PARAMETER - logFormat is NULL.
 * @retval OS_SUCCESS                 - Operation was successful.
 */
OS_Error_t
OS_LoggerOutput_ctor(
    OS_LoggerOutput_Handle_t* self,      //!< [in] This pointer.
    OS_LoggerFormat_Handle_t* logFormat, //!< [in] Printing format.
    OS_LoggerOutput_update_t update      //!< [in] Update callback.
);

/**
 * @brief   Update function.
 *
 * Wrapper for calling the child's class implementation.
 *
 * @return  An error code which is child's class implementation specific.
*/
OS_Error_t
OS_LoggerOutput_update(
    OS_LoggerOutput_Handle_t* self, //!< [in] This handle.
    void* data                      //!< [in] Data related to the update.
);
