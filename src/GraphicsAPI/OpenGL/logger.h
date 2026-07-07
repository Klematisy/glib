#pragma once

#include <string>

#define TO_STR(x) std::to_string(x)

#define LOGINF(msg) (void(0))
#define LOGWARN(msg) (void(0))
#define LOGERR(msg) (void(0))

#ifdef GL_LOGGER_EXISTS
#include "Logger/logger.h"
#endif
