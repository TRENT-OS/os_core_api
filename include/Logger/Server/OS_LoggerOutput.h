/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Abstract observer interface
 */
#pragma once

/**
 * @defgroup    OS_LoggerOutput Abstract observer interface
 *
 * @brief       Defined abstract observer interface.
 *
 * @details     This level provides the mechanism for an abstract observer
 *              interface.
 *              \n
 *              The Design Pattern Observer describes how relationships between
 *              objects are set up.
 *
 *              In this pattern, the subject and the observer are the crucial
 *              objects. A subject can have any number of dependent observers.
 *              Every time the subject changes state, all observers are
 *              notified. In response, each observer will contact the subject to
 *              synchronize its own state with that of the subject.
 *              \n
 *              Definition of a 1-to-n dependency between objects, so that in
 *              the event of an object's state change, all dependent objects are
 *              notified accordingly and updated automatically.
 *              \n \n
 *              \b Attendees \n
 *              \a Subject \n
 *              Knows his observer. A subject can be observed by any number of
 *              observers. Provides an interface for connecting and detaching
 *              Observer objects.
 *              \n \n
 *              \a Observer \n
 *              Defines an update interface for objects that are at Changes in
 *              the status of a subject are to be notified. Concrete Subjec_t
 *              saves the state of interest in ConcreteObserver objects. Sends a
 *              notification to its observer when the state changes.
 *              \n \n
 *              \a ConcreteObserver \n
 *              Stores a reference to a ConcreteSubject object. Stores the state
 *              in accordance with that of the subject should stay. Implements
 *              the update interface for Observer so that its own state matches
 *              that of Subject.
 *              \n \n
 *              \b Interactions \n
 *              ConcreteSubject notifies its observer as soon as a state change
 *              occurs that could result in the states of the observer objects
 *              no longer matching their own state. When a ConcreteObserver is
 *              informed of a change in ConcreteSubject, it may request
 *              information from the subject that it uses to match its own state
 *              with that of Subject. The diagram below shows the collaboration
 *              between one Subject and two observer:
 *
 *              \code
 *                  1. Assignment of subjects to their observer.
 *                  2. Observe multiple subjects.
 *                  3. Trigger update with notify / update
 *              \endcode
 *
 *              Those operations that cause changes in state call Notify after
 *              making a change in the state of the subject. The advantage of
 *              this approach is that clients do not have to remember to call
 *              the subject's Notify method. The disadvantage is that multiple
 *              consecutive operations also cause a series of consecutive
 *              updates - and this could prove inefficient. The clients are
 *              responsible for calling Notify at the right time. This has the
 *              advantage that the client can wait until several successive
 *              state changes have been completed to trigger the update, and
 *              thus unnecessary avoids interim updates. The disadvantage is
 *              that the clients also have to take responsibility for triggering
 *              the update. This increases the likelihood that errors will occur
 *              because the clients may forget to call Notify.
 *              \n \n
 *              Further it must be implemented the observer functions in the
 *              derived layer.
 *
 * @see         abstract_subject
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
 * @details OS_LoggerOutput_update_t defines the interface for
 *          function pointer to notify the specific observer in the form of a
 *          parameter of the update operation.
 *
 * @param   self:   pointer to the class
 * @param   data:   user data
 *
 * @return  An error code.
 *
 * @ingroup OS_LoggerOutput
*/
typedef OS_Error_t
(*OS_LoggerOutput_update_t)(
    OS_LoggerOutput_Handle_t* self,
    void* data);

/**
 * @details OS_LoggerOutput_Handle_t contain the vtable to his class.
 *
 * @ingroup OS_LoggerOutput
*/
struct OS_LoggerOutput_Handle
{
    OS_LoggerNodeT_Handle_t   node;            //!< node interface
    OS_LoggerFormat_Handle_t* logFormat;       //!< layer for log format
    OS_LoggerOutput_update_t update; //!< Function ptr to update
};

/**
 * @brief  Constructs given observer object.
 *
 * @return An error code.
 *
 * @retval SEOS_ERROR_INVALID_PARAMETER - logFormat is NULL.
 * @retval SEOS_SUCCESS                 - Operation was successful.
 */
OS_Error_t
OS_LoggerOutput_ctor(
    OS_LoggerOutput_Handle_t* self,   //!< [in] This pointer.
    OS_LoggerFormat_Handle_t* logFormat,        //!< [in] Printing format.
    OS_LoggerOutput_update_t update   //!< [in] Update callback.
);

/**
 * @details %OS_LoggerOutput_update is an abstract function for the
 *          update function.
 *
 * @param   self:   pointer to the class
 * @param   data:   user data
 *
 * @return  An error code which is implementation specific.
 *
 * @ingroup OS_LoggerOutput
*/
OS_Error_t
OS_LoggerOutput_update(
    OS_LoggerOutput_Handle_t* self,
    void* data);
