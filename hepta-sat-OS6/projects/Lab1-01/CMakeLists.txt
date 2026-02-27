# Copyright (c) 2025 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os CACHE INTERNAL "")
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")
get_filename_component(APP_TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)

include(${MBED_PATH}/tools/cmake/app.cmake)

project(${APP_TARGET})

# --- Lab1-01 ---
set(APP_SOURCES
    main.cpp
)

set(HEPTA_USE_SD OFF)
# ---------------

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/hepta_common.cmake)