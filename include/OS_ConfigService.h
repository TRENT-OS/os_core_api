/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

#include "SeosError.h"

#include "OS_ConfigServiceDataTypes.h"
#include "OS_ConfigServiceInstanceStore.h"

OS_ConfigServiceInstanceStore_t*
OS_ConfigService_getInstances(void);

seos_err_t
OS_ConfigService_createHandle(
    OS_ConfigServiceHandle_HandleKind_t handleKind,
    unsigned int id,
    OS_ConfigServiceHandle_t* handle);

seos_err_t
OS_ConfigService_domainEnumeratorInit(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

seos_err_t
OS_ConfigService_domainEnumeratorClose(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

seos_err_t
OS_ConfigService_domainEnumeratorReset(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

seos_err_t
OS_ConfigService_domainEnumeratorIncrement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

seos_err_t
OS_ConfigService_domainEnumeratorGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Domain_t* domain);

seos_err_t
OS_ConfigService_parameterEnumeratorInit(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* domainEnumerator,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

seos_err_t
OS_ConfigService_parameterEnumeratorClose(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

seos_err_t
OS_ConfigService_parameterEnumeratorReset(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

seos_err_t
OS_ConfigService_parameterEnumeratorIncrement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

seos_err_t
OS_ConfigService_parameterEnumeratorGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Parameter_t* parameter);

void
OS_ConfigService_domainGetName(
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_DomainName_t* domainName);

seos_err_t
OS_ConfigService_domainCreateParameterEnumerator(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* parameterEnumerator);

seos_err_t
OS_ConfigService_domainGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_Parameter_t* parameter);

void
OS_ConfigService_parameterGetName(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterName_t* parameterName);

void
OS_ConfigService_parameterGetType(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterType_t* parameterType);

size_t
OS_ConfigService_parameterGetSize(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter);

seos_err_t
OS_ConfigService_parameterGetValue(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied);

seos_err_t
OS_ConfigService_parameterGetValueAsU32(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint32_t* value);

seos_err_t
OS_ConfigService_parameterGetValueAsU64(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint64_t* value);

seos_err_t
OS_ConfigService_parameterGetValueAsString(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    char* buffer,
    size_t bufferLength);

seos_err_t
OS_ConfigService_parameterGetValueAsBlob(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    void* buffer,
    size_t bufferLength);

seos_err_t
OS_ConfigService_parameterSetValue(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void const* buffer,
    size_t bufferLength);

seos_err_t
OS_ConfigService_parameterSetValueAsU32(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint32_t value);

seos_err_t
OS_ConfigService_parameterSetValueAsU64(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint64_t value);

seos_err_t
OS_ConfigService_parameterSetValueAsString(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    char const* buffer,
    size_t bufferLength);

seos_err_t
OS_ConfigService_parameterSetValueAsBlob(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void const* buffer,
    size_t bufferLength);

seos_err_t
OS_ConfigService_parameterGetValueFromDomainName(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainName_t const* domainName,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied);


