// sleep_patch.h
#pragma once

#ifdef __cplusplus
#include "mbed.h"
#include <chrono>
using namespace std::chrono_literals;

/*
 * Patch layer for LPC1768 (Mbed OS 6):
 * -------------------------------------
 * In some environments, ThisThread::sleep_for(...) waits roughly 10× longer
 * than expected due to an RTOS timing mismatch.
 *
 * Replace ThisThread::sleep_for() with an accurate version using wait_us().
 */
namespace rtos { namespace ThisThread {

    template<class Rep, class Period>
    inline void sleep_for(std::chrono::duration<Rep, Period> d) {
        using namespace std::chrono;
        wait_us((int)duration_cast<microseconds>(d).count());
    }

}}

#endif  // __cplusplus