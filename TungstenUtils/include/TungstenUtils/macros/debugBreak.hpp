#ifndef TUNGSTEN_UTILS_DEBUG_BREAK_HPP
#define TUNGSTEN_UTILS_DEBUG_BREAK_HPP

#if defined(_MSC_VER)
    #define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
    #include <signal.h>
    #define DEBUG_BREAK() raise(SIGTRAP)
#else
    #include "noOp.hpp"
    #define DEBUG_BREAK() W_NO_OP()  // fallback: no-op
#endif

#endif