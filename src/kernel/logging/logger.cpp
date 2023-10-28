#include "logging/logger.hpp"

#include "drivers/display/vga3.hpp"

using namespace drivers::display;

namespace logging {

static void log(const char *message, Level log_level);
static void print_level(Level log_level);

static Level current_level;

void set_level(Level new_log_level) {
    current_level = new_log_level;
}

void debug(const char *message) {
    log(message, Level::DEBUG);
}

void info(const char *message) {
    log(message, Level::INFO);
}

void warn(const char *message) {
    log(message, Level::WARN);
}

void error(const char *message) {
    log(message, Level::ERROR);
}

void fatal(const char *message) {
    log(message, Level::FATAL);
}

void log(const char *message, Level log_level) {
    if (log_level < current_level) {
        return;
    }

    print_level(log_level);
    vga3::print(message);
    vga3::print("\n");
}

void print_level(Level log_level) {
    const char *level_string = nullptr;
    vga3::Color level_color = vga3::Color::WHITE;

    switch (log_level) {
        case Level::DEBUG:
            level_string = "DEBUG: ";
            level_color = vga3::Color::LIGHT_BLUE;
            break;
        case Level::INFO:
            level_string = "INFO: ";
            level_color = vga3::Color::GREEN;
            break;
        case Level::WARN:
            level_string = "WARN: ";
            level_color = vga3::Color::YELLOW;
            break;
        case Level::ERROR:
            level_string = "ERROR: ";
            level_color = vga3::Color::RED;
            break;
        case Level::FATAL:
            level_string = "FATAL: ";
            level_color = vga3::Color::LIGHT_RED;
            break;
        default:
            return;
    }

    vga3::print(level_string, level_color);
}

}  // namespace logging
