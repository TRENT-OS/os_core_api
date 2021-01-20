/*
 * Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#define IF_OS_ENTROPY_CAMKES    \
    size_t read(                \
        in size_t len);