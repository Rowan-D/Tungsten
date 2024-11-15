#pragma once

#define DO_NOTHING() ((void)0)

#define ENFORCE_SEMICOLON(x) \
    do { \
        x \
    } while (0)

#ifdef _WIN32
    #include <intrin.h>
    #define DEBUG_BREAK() __debugbreak()
#elif __linux__
    #include <csignal>
    #define DEBUG_BREAK() raise(SIGTRAP)
#else
    #define DEBUG_BREAK() DO_NOTHING()
#endif
