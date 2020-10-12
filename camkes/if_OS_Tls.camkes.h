/* Copyright (C) 2019-2020, Hensoldt Cyber GmbH */

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