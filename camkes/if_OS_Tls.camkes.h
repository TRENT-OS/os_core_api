/*
 * Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#define IF_OS_TLS_CAMKES        \
    OS_Error_t handshake();     \
    OS_Error_t write(           \
        inout size_t dataSize   \
    );                          \
    OS_Error_t read(            \
        inout size_t dataSize   \
    );                          \
    OS_Error_t reset();