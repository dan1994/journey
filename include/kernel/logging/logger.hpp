#pragma once

#include <string>

namespace logging {
enum class Level { DEBUG, INFO, WARN, ERROR, FATAL };

void set_level(Level level);

void debug(const char *message);
void info(const char *message);
void warn(const char *message);
void error(const char *message);
void fatal(const char *message);
}  // namespace logging
