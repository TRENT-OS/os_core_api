/* Copyright (C) 2019-2020, Hensoldt Cyber GmbH */

#pragma once

#define IF_OS_ENTROPY_CAMKES    \
    size_t read(                \
        in size_t len);