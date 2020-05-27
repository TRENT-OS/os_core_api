/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Abstract subject interface
 */
#pragma once

/**
 * @defgroup    OS_LoggerAbstractSubject Abstract subject interface
 *
 * @brief       Defined abstract subject interface.
 * @details     This level provides the mechanism for an abstract subject
 *              interface.
 *              \n
 *              This class provides the provision of subjects that can be
 *              observed by Observer.
 *
 *              The interface functions provide: Registration and deregistration
 *              of observers to specific subjects, and the signaling function
 *              when a subject contains new information that should be
 *              communicated to the Observer in the form of a "notify" function.
 *              \n
 *              The subject's interface allows the observer to register only for
 *              certain events of particular interest. The "attach" function
 *              exists for this. If such an event occurs, the subject only
 *              notifies those observers who have expressed interest in it.
 *
 *              The function "detach" is available to deregister objects.
 *              \n \n
 *              Further it must be implemented the subject functions in the
 *              derived layer.
 *              \n \n
 *              There is a strong dependency to the observer objects.
 *
 * @see         abstract_observer
 *
 * @ingroup     OS_LoggerServer
*/
#include "OS_Error.h"
#include "Logger/Server/OS_LoggerAbstractObserver.h"
#include <stdbool.h>

/**
 * @details OS_LoggerAbstractSubject_Handle_t defines the class datatype.
 *
 * @ingroup OS_LoggerAbstractSubject
*/
typedef
struct OS_LoggerAbstractSubject_Handle OS_LoggerAbstractSubject_Handle_t;

/**
 * @details OS_LoggerAbstractSubject_attach_t defines the interface for function
 *          pointer to register observer object to a subject.
 *
 * @param   self:       pointer to the class
 * @param   observer:   pointer to a observer object
 *
 * @return  An error code.
 *
 * @ingroup OS_LoggerAbstractSubject
*/
typedef OS_Error_t
(*OS_LoggerAbstractSubject_attach_t)(
    OS_LoggerAbstractSubject_Handle_t* self,
    OS_LoggerAbstractObserver_Handle_t* observer);


/**
 * @details OS_LoggerAbstractSubject_detach_t defines the interface for function
 *          pointer to deregister observer object from a subject.
 *
 * @param   self:       pointer to the class
 * @param   observer:   pointer to a observer object
 *
 * @return  An error code.
 *
 * @ingroup OS_LoggerAbstractSubject
*/
typedef OS_Error_t
(*OS_LoggerAbstractSubject_detach_t)(
    OS_LoggerAbstractSubject_Handle_t*  self,
    OS_LoggerAbstractObserver_Handle_t* observer);


/**
 * @details OS_LoggerAbstractSubject_notify_t defines the interface for function
 *          pointer to notify the registraded observer objects.
 *
 * @param   self:   pointer to the class
 * @param   data:   user data from observer
 *
 * @ingroup OS_LoggerAbstractSubject
*/
typedef void
(*OS_LoggerAbstractSubject_notify_t)(
    OS_LoggerAbstractSubject_Handle_t* self,
    void* data);


/**
 * @details OS_LoggerAbstractSubject_vtable_t contain the member functions to
 *          his class.
 *
 * @ingroup OS_LoggerAbstractSubject
*/
typedef struct
{
    OS_LoggerAbstractSubject_attach_t
    attach; /**< function pointer to the attach function */
    OS_LoggerAbstractSubject_detach_t
    detach; /**< function pointer to the detach function */
    OS_LoggerAbstractSubject_notify_t
    notify; /**< function pointer to the notify function */
}
OS_LoggerAbstractSubject_vtable_t;


/**
 * @details OS_LoggerAbstractSubject_Handle_t contain the vtable to his class.
 *
 * @ingroup OS_LoggerAbstractSubject
*/
struct OS_LoggerAbstractSubject_Handle
{
    const OS_LoggerAbstractSubject_vtable_t* vtable; /**< vtable */
};

/**
 * @details %OS_LoggerAbstractSubject_attach is an abstract function to register
 *          observer object to a subject.
 *
 * @param   self:       pointer to the class
 * @param   observer:   pointer to a observer object
 *
 * @return  An error code.
 *
 * @retval  OS_ERROR_INVALID_PARAMETER - log_info is a NULL pointer.
 * @retval  other                      - Implementation specific.
 *
 * @ingroup OS_LoggerAbstractSubject
*/
OS_Error_t
OS_LoggerAbstractSubject_attach(
    OS_LoggerAbstractSubject_Handle_t* self,
    OS_LoggerAbstractObserver_Handle_t* observer);


/**
 * @details %OS_LoggerAbstractSubject_detach is an abstract function to
 *          deregister observer object from a subject.
 *
 * @param   self:       pointer to the class
 * @param   observer:   pointer to a observer object
 *
 * @return  An error code.
 *
 * @retval  OS_ERROR_INVALID_PARAMETER - log_info is a NULL pointer.
 * @retval  other                      - Implementation specific.
 *
 * @ingroup OS_LoggerAbstractSubject
*/
OS_Error_t
OS_LoggerAbstractSubject_detach(
    OS_LoggerAbstractSubject_Handle_t* self,
    OS_LoggerAbstractObserver_Handle_t* observer);


/**
 * @details %OS_LoggerAbstractSubject_notify is an abstract function to notify
 *          the registraded observer objects.
 *
 * @param   self:   pointer to the class
 * @param   data:   user data from observer
 *
 * @ingroup OS_LoggerAbstractSubject
*/
void
OS_LoggerAbstractSubject_notify(
    OS_LoggerAbstractSubject_Handle_t* self,
    void* data);

