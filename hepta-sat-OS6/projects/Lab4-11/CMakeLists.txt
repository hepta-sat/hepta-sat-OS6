# Copyright (c) 2025 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os CACHE INTERNAL "")
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")
get_filename_component(APP_TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)

include(${MBED_PATH}/tools/cmake/app.cmake)

project(${APP_TARGET})

# --- Lab4-11 ---
add_subdirectory(HEPTA_CDH)
add_subdirectory(HEPTA_EPS)
add_subdirectory(HEPTA_SENSOR)

set(APP_SOURCES
    main.cpp
    HEPTA_CDH/hepta_cdh.cpp
    HEPTA_EPS/hepta_eps.cpp
    HEPTA_EPS/HeptaBattery/HeptaBattery.cpp 
    HEPTA_EPS/HeptaBattery/PowerControl/EthernetPowerControl.cpp
    HEPTA_SENSOR/hepta_sensor.cpp
    HEPTA_SENSOR/Hepta9axis/Hepta9axis.cpp
    HEPTA_SENSOR/HeptaCamera_GPS/HeptaCamera_GPS.cpp
    HEPTA_SENSOR/HeptaCamera_GPS/SerialBuffered.cpp
    HEPTA_SENSOR/HeptaTemp/HeptaTemp.cpp
)

set(APP_LIBS hepta_cdh hepta_eps hepta_sensor)

set(HEPTA_USE_SD ON)
# ---------------

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/hepta_common.cmake)

target_compile_options(${APP_TARGET} PRIVATE
    -include ${CMAKE_CURRENT_SOURCE_DIR}/sleep_patch.h
)