/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 * @defgroup SeosCryptoTls SEOS TLS API
 * @{
 *
 * @file SeosCryptoTls.h
 *
 * @brief SEOS TLS API library
 *
 */

#pragma once

#include "SeosCryptoApi.h"

#include "SeosTlsApi_Impl.h"

seos_err_t
SeosTlsApi_init(SeosTlsCtx*             ctx,
                const SeosTls_Config*   cfg);

seos_err_t
SeosTlsApi_handshake(SeosTlsCtx*        ctx);

seos_err_t
SeosTlsApi_write(SeosTlsCtx*            ctx,
                 const void*            data,
                 const size_t           dataSize);

seos_err_t
SeosTlsApi_read(SeosTlsCtx*             ctx,
                void*                   data,
                size_t*                 dataSize);

seos_err_t
SeosTlsApi_free(SeosTlsCtx*             ctx);

/** @} */
