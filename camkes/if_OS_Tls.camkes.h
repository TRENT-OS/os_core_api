/*
 * Copyright (C) 2019-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
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