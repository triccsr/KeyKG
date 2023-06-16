
#include <bits/chrono.h>
#include <cstdint>
int64_t millis()
{
    int64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch())
            .count();
    return ms; 
}

// Get time stamp in microseconds.
int64_t micros()
{
    int64_t us = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now().time_since_epoch())
            .count();
    return us; 
}

// Get time stamp in nanoseconds.
int64_t nanos()
{
    int64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    return ns; 
}