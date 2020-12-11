/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief       Log subject implementation.
 *
 * @details     Derived class from class \link OS_LoggerAbstractSubject_Handle_t
 *              \endlink and provides an implemention for log subject.
 *
 *              This level provides an implementation for log subject.
 *              This class is derived by abstract subject.
 *
 *              Concrete observers can be registered to one subject by
 *              calling the "attach" function. Concrete observers can be
 *              deregistraded by calling "detach" function.
 *
 *              To inform the concrete observers by update, call the notify
 *              function.
 *
 *              The concrete implementation is realized as singleton. To one
 *              concrete subject can be registraded to different observers.
 *              There can exist different concrete subjects at the same time,
 *              but the assigning of a concrete observer is only allowed to one
 *              concrete subject at the same time. The assigned from one
 *              concrete observer to two different concrete subject leads
 *              to that the first assignee will be overwritten. And the concrete
 *              observer is assigned to the second (last - if more than two
 *              assigning issues has been done) concrete subject.
 *
 * @ingroup     OS_LoggerServer
*/
#pragma once

#include "OS_Error.h"
#include "Logger/Server/OS_LoggerAbstractSubject.h"
#include <stdbool.h>


/**
 * @details OS_LoggerSubjectNode_t contain informations about the log subject
 *          and is realized as singleton.
*/
typedef struct
{
    void* first; /**< pointer to the first element */
} OS_LoggerSubjectNode_t;


/**
 * @details OS_LoggerSubject_Handle_t contain informations about the log
 *          subject.
*/
typedef struct
{
    OS_LoggerSubjectNode_t node; //!< Contains the singleton object
    const OS_LoggerAbstractSubject_vtable_t* vtable;
} OS_LoggerSubject_Handle_t;


/**
 * @details %OS_LoggerSubject_ctor is the constructor.
 *
 * @param   self:   pointer to the class
*/
void
OS_LoggerSubject_ctor(OS_LoggerSubject_Handle_t* self);

/**
 * @details %OS_LoggerSubject_attach is for registrade a concrete observer
 *          object to a subject.
 *
 * @param   self:       pointer to the class
 * @param   observer:   pointer to concrete observer
 *
 * @return  An error code.
 *
 * @retval  OS_ERROR_INVALID_PARAMETER - Observer is a NULL pointer.
 * @retval  OS_ERROR_OPERATION_DENIED  - Observer was not attached.
 * @retval  OS_SUCCESS                 - Operation was successful.
*/
OS_Error_t
OS_LoggerSubject_attach(
    OS_LoggerAbstractSubject_Handle_t* self,
    OS_LoggerOutput_Handle_t* observer);


/**
 * @details %OS_LoggerSubject_detach is for deregistrade a concrete observer
 *          object from a subject.
 *
 * @param   self:       pointer to the class
 * @param   observer:   pointer to concrete observer
 *
 * @return  An error code.
 *
 * @retval  OS_ERROR_INVALID_PARAMETER - Observer is a NULL pointer.
 * @retval  OS_ERROR_OPERATION_DENIED  - Observer was not detach.
 * @retval  OS_SUCCESS                 - Operation was successful.
*/
OS_Error_t
OS_LoggerSubject_detach(
    OS_LoggerAbstractSubject_Handle_t* self,
    OS_LoggerOutput_Handle_t* observer);


/**
 * @details %OS_LoggerSubject_notify is for notifying the registraded observer
 *          objects by an update.
 *
 * @param   self:   pointer to the class
 * @param   data:   user data from observer
*/
void
OS_LoggerSubject_notify(OS_LoggerAbstractSubject_Handle_t* self, void* data);

