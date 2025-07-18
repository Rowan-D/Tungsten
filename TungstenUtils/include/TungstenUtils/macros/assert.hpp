#ifndef TUNGSTEN_UTILS_ASSERT_HPP
#define TUNGSTEN_UTILS_ASSERT_HPP

#include "debugBreak.hpp"

#define W_ASSERT(check, ...) \
    do \
    { \
        if (!(check)) \
        { \
            W_LOG_CRITICAL_ERROR(__VA_ARGS__); \
            DEBUG_BREAK(); \
        } \
    } \
    while (false)

#endif