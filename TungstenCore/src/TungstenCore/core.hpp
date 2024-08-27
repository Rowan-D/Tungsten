#pragma once

// std
#include <cstdint>
#include <limits>
#include <memory>
#include <vector>
#include <algorithm>
#include <fstream>

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

// spdlog
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "ansi.hpp"
#include "logging.hpp"

#ifdef W_DEBUG
#define W_ASSERT(check, ...) \
    do { \
        if (!(check)) { \
            W_LOG_CRITICAL_ERROR(__VA_ARGS__); \
        } \
    } while (0)
#else
#define W_ASSERT(check, ...)
#endif