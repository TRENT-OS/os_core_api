/* Copyright (C) 2019-2020, Hensoldt Cyber GmbH */

/**
 * @file
 * @defgroup OS_ConfigService OS ConfigService API
 * @{
 * @brief ConfigService API library
 */

#pragma once

/* Includes ------------------------------------------------------------------*/
#include "OS_Error.h"
#include "OS_Dataport.h"

#include "OS_ConfigServiceDataTypes.h"

typedef struct
{
    /* this is the dataport that the user will pass to this client library when
     * calling OS_ConfigService_createHandleRemote(). The dataport is need by
     * client to know where to retrieve the data from
     */
    OS_Dataport_t dataport;
}
OS_ConfigService_ClientCtx_t;

/**
 * @brief Returns a pointer to a ConfigService library instance.
 *
 * @return Depending if this is called from a server or client component, it
 * will either return a pointer to a client or a server library instance. Apart
 * from the naming, there is no difference for these types of instances.
 */
OS_ConfigServiceLib_t*
OS_ConfigService_getInstance(void);

/**
 * @brief Creates a handle to a ConfigService library local instance.
 *
 * @param handle handle to the ConfigService library instance.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, invalid id, etc.)
 */
OS_Error_t
OS_ConfigService_createHandleLocal(
    OS_ConfigServiceHandle_t* handle);

/**
 * @brief Creates a handle to a ConfigService library remote instance.
 *
 * @param clientCtx a pointer to the client context. User must define it.
 * @param handle handle to the ConfigService library instance.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, invalid id, etc.)
 */
OS_Error_t
OS_ConfigService_createHandleRemote(
    OS_ConfigService_ClientCtx_t* clientCtx,
    OS_ConfigServiceHandle_t* handle);

/**
 * @brief Initializes a DomainEnumerator object by setting its enumerator index
 * to 0.
 *
 * @param handle handle to the ConfigService library instance.
 * @param enumerator pointer to a DomainEnumerator object.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, invalid id, etc.)
 */
OS_Error_t
OS_ConfigService_domainEnumeratorInit(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

/**
 * @brief Closes a DomainEnumerator object.
 *
 * @param handle handle to the ConfigService library instance.
 * @param enumerator pointer to a DomainEnumerator object.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_domainEnumeratorClose(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

/**
 * @brief Resets the DomainEnumerator objects enumerator index to 0.
 *
 * @param handle handle to the ConfigService library instance.
 * @param enumerator pointer to a DomainEnumerator object.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_domainEnumeratorReset(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

/**
 * @brief Increments the DomainEnumerator objects enumerator index by 1.
 *
 * @param handle handle to the ConfigService library instance.
 * @param enumerator pointer to a DomainEnumerator object.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_domainEnumeratorIncrement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

/**
 * @brief Assigns the passed Domain object pointer to the Domain object
 * associated with the DomainEnumerator object in the backend.
 *
 * @param handle handle to the ConfigService library instance.
 * @param enumerator pointer to a DomainEnumerator object.
 * @param domain pointer to a Domain object.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_domainEnumeratorGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Domain_t* domain);

/**
 * @brief Initializes a ParameterEnumerator object by pointing it to the first
 * parameter of the given domain.
 *
 * @param handle handle to the ConfigService library instance.
 * @param domainEnumerator pointer to a DomainEnumerator object.
 * @param enumerator pointer to a ParameterEnumerator object.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_GENERIC if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.), if no parameter is set for the given domain or if an
 *  error occurred during data retrieval
 */
OS_Error_t
OS_ConfigService_parameterEnumeratorInit(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* domainEnumerator,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

/**
 * @brief Closes a ParameterEnumerator object.
 *
 * @param handle handle to the ConfigService library instance.
 * @param enumerator pointer to a ParameterEnumerator object.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_parameterEnumeratorClose(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

/**
 * @brief Resets the ParameterEnumerator objects enumerator index to 0.
 *
 * @param handle handle to the ConfigService library instance.
 * @param enumerator pointer to a ParameterEnumerator object.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_parameterEnumeratorReset(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

/**
 * @brief Increments the ParameterEnumerator objects enumerator index by 1.
 *
 * @param handle handle to the ConfigService library instance.
 * @param enumerator pointer to a ParameterEnumerator object.
 *
 * @return an error code
 * @retval OS_SUCCESS if no
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_parameterEnumeratorIncrement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

/**
 * @brief Assigns the passed Parameter object pointer to the Parameter object
 * associated with the ParameterEnumerator object in the backend.
 *
 * @param handle handle to the ConfigService library instance.
 * @param enumerator pointer to a ParameterEnumerator object.
 * @param parameter pointer to a Parameter object.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_GENERIC if no element is found for the given
 * ParameterEnumerator or if an error occurred during data retrieval
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_parameterEnumeratorGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Parameter_t* parameter);

/**
 * @brief Extracts the domain name from the given domain.
 *
 * @param domain pointer to a Domain object.
 * @param domainName pointer to a DomainName object.
 */
void
OS_ConfigService_domainGetName(
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_DomainName_t* domainName);

/**
 * @brief Creates a ParameterEnumerator pointing to the parameter with the
 * given name in the given domain.
 *
 * @param handle handle to the ConfigService library instance.
 * @param domain pointer to a Domain object.
 * @param parameterName pointer to a ParameterName object.
 * @param parameterEnumerator pointer to a ParameterEnumerator object.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_GENERIC if no parameter is set for the given domain or if
 * an error occurred during data retrieval
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_domainCreateParameterEnumerator(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* parameterEnumerator);

/**
 * @brief Extracts parameter by name in a given domain.
 *
 * @param handle handle to the ConfigService library instance.
 * @param domain pointer to a Domain object.
 * @param parameterName pointer to a ParameterName object.
 * @param parameter pointer to a Parameter object.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_GENERIC if parameter could not be found or is not set for
 * the given domain or if an error occurred during data retrieval
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_domainGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_Parameter_t* parameter);

/**
 * @brief Extracts the parameter name of passed Parameter object.
 *
 * @param parameter pointer to a Parameter object.
 * @param parameterName pointer to a ParameterName object.
 */
void
OS_ConfigService_parameterGetName(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterName_t* parameterName);

/**
 * @brief Extracts the parameter type of passed Parameter object.
 *
 * @param parameter pointer to a Parameter object.
 * @param parameterType pointer to a ParameterType object.
 */
void
OS_ConfigService_parameterGetType(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterType_t* parameterType);

/**
 * @brief Extracts the parameter type of passed Parameter object.
 *
 * @param parameter pointer to a Parameter object.
 * @return size of the parameter
 */
size_t
OS_ConfigService_parameterGetSize(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter);

/**
 * @brief Extracts the parameter value - raw format.
 *
 * @param handle handle to the ConfigService library instance.
 * @param parameter pointer to a Parameter object.
 * @param buffer buffer to copy the parameter value into.
 * @param bufferLength length of the buffer.
 * @param bytesCopied pointer to place the extracted value length into.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_GENERIC if the passed buffer is too small for the
 * parameter size or an error occurred during data retrieval and no bytes were
 * copied to the buffer.
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_parameterGetValue(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied);

/**
 * @brief Extracts the parameter value - uint32 format.
 *
 * @param handle handle to the ConfigService library instance.
 * @param parameter pointer to a Parameter object.
 * @param value pointer to a uint32 variable to copy the value into.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_parameterGetValueAsU32(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint32_t* value);

/**
 * @brief Extracts the parameter value - uint64 format.
 *
 * @param handle handle to the ConfigService library instance.
 * @param parameter pointer to a Parameter object.
 * @param value pointer to a uint64 variable to copy the value into.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_parameterGetValueAsU64(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint64_t* value);

/**
 * @brief Extracts the parameter value - string format.
 *
 * @param handle handle to the ConfigService library instance.
 * @param parameter pointer to a Parameter object.
 * @param buffer buffer to copy the parameter value into.
 * @param bufferLength length of the buffer.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_GENERIC if the passed buffer is too small for the
 * parameter size or an error occurred during data retrieval and no bytes were
 * copied to the buffer.
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_parameterGetValueAsString(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    char* buffer,
    size_t bufferLength);

/**
 * @brief Extracts the parameter value - blob format.
 *
 * @param handle handle to the ConfigService library instance.
 * @param parameter pointer to a Parameter object.
 * @param buffer buffer to copy the parameter value into.
 * @param bufferLength length of the buffer.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_GENERIC if the passed buffer is too small for the
 * parameter size or an error occurred during data retrieval and no bytes were
 * copied to the buffer.
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_parameterGetValueAsBlob(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    void* buffer,
    size_t bufferLength);

/**
 * @brief Extracts the parameter value from a specific domain and parameter name
 * - raw format.
 *
 * @param handle handle to the ConfigService library instance.
 * @param domainName pointer to a DomainName object.
 * @param parameterName pointer to a ParameterName object.
 * @param parameterType pointer to a ParameterType object.
 * @param buffer buffer to copy the parameter value into.
 * @param bufferLength length of the buffer.
 * @param bytesCopied pointer to place the extracted value length into.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_GENERIC if the passed buffer is too small for the
 * parameter size, the passed parameter type does not match the type in the
 * backend or an error occurred during data retrieval and no bytes were
 * copied to the buffer.
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_parameterGetValueFromDomainName(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainName_t const* domainName,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied);

/**
 * @brief Set the parameter value - raw format.
 *
 * @param handle handle to the ConfigService library instance.
 * @param enumerator pointer to a ParameterEnumerator object.
 * @param parameterType pointer to a ParameterType object.
 * @param buffer buffer to copy the parameter value into.
 * @param bufferLength length of the buffer.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_GENERIC if the value to write is larger than the space
 * reserved for the parameter to write to in the backend or no writing rights are
 * set for the parameter or no element could be retrieved to write to for the
 * passed ParameterEnumerator or an error occurred during the write process
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_parameterSetValue(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void const* buffer,
    size_t bufferLength);

/**
 * @brief Set the parameter value - uint32 format.
 *
 * @param handle handle to the ConfigService library instance.
 * @param enumerator pointer to a ParameterEnumerator object.
 * @param value value that should be set for the parameter.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_GENERIC if the value to write is larger than the space
 * reserved for the parameter to write to in the backend or no writing rights are
 * set for the parameter or no element could be retrieved to write to for the
 * passed ParameterEnumerator or an error occurred during the write process
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_parameterSetValueAsU32(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint32_t value);

/**
 * @brief Set the parameter value - uint64 format.
 *
 * @param handle handle to the ConfigService library instance.
 * @param enumerator pointer to a ParameterEnumerator object.
 * @param value value that should be set for the parameter.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_GENERIC if the value to write is larger than the space
 * reserved for the parameter to write to in the backend or no writing rights are
 * set for the parameter or no element could be retrieved to write to for the
 * passed ParameterEnumerator or an error occurred during the write process
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_parameterSetValueAsU64(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint64_t value);

/**
 * @brief Set the parameter value - string format.
 *
 * @param handle handle to the ConfigService library instance.
 * @param enumerator pointer to a ParameterEnumerator object.
 * @param parameterType pointer to a ParameterType object.
 * @param buffer buffer to copy the parameter value into.
 * @param bufferLength length of the buffer.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_GENERIC if the value to write is larger than the space
 * reserved for the parameter to write to in the backend or no writing rights are
 * set for the parameter or no element could be retrieved to write to for the
 * passed ParameterEnumerator or an error occurred during the write process
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_parameterSetValueAsString(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    char const* buffer,
    size_t bufferLength);

/**
 * @brief Set the parameter value - blob format.
 *
 * @param handle handle to the ConfigService library instance.
 * @param enumerator pointer to a ParameterEnumerator object.
 * @param parameterType pointer to a ParameterType object.
 * @param buffer buffer to copy the parameter value into.
 * @param bufferLength length of the buffer.
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_GENERIC if the value to write is larger than the space
 * reserved for the parameter to write to in the backend or no writing rights are
 * set for the parameter or no element could be retrieved to write to for the
 * passed ParameterEnumerator or an error occurred during the write process
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, etc.)
 */
OS_Error_t
OS_ConfigService_parameterSetValueAsBlob(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void const* buffer,
    size_t bufferLength);

/** @} */
