#include "logging/logger.hpp"

#include "drivers/display/vga3.hpp"

using namespace drivers::display;

void Logger::debug(const std::string &message) {
    log(message, Level::DEBUG);
}

void Logger::debug(const char *message) {
    log(message, Level::DEBUG);
}

void Logger::info(const std::string &message) {
    log(message, Level::INFO);
}

void Logger::info(const char *message) {
    log(message, Level::INFO);
}

void Logger::warn(const std::string &message) {
    log(message, Level::WARN);
}

void Logger::warn(const char *message) {
    log(message, Level::WARN);
}

void Logger::error(const std::string &message) {
    log(message, Level::ERROR);
}

void Logger::error(const char *message) {
    log(message, Level::ERROR);
}

void Logger::fatal(const std::string &message) {
    log(message, Level::FATAL);
}

void Logger::fatal(const char *message) {
    log(message, Level::FATAL);
}

void Logger::set_log_level(Level new_log_level) {
    current_level_ = new_log_level;
}

void Logger::log(const std::string &message, Level log_level) {
    if (log_level < current_level_) {
        return;
    }

    print_level(log_level);
    Vga3::print(message);
    Vga3::print("\n");
}

void Logger::log(const char *message, Level log_level) {
    if (log_level < current_level_) {
        return;
    }

    print_level(log_level);
    Vga3::print(message);
    Vga3::print("\n");
}

void Logger::print_level(Level log_level) {
    const char *level_string = nullptr;
    Vga3::Color level_color = Vga3::Color::WHITE;

    switch (log_level) {
        case Level::DEBUG:
            level_string = "DEBUG: ";
            level_color = Vga3::Color::LIGHT_BLUE;
            break;
        case Level::INFO:
            level_string = "INFO: ";
            level_color = Vga3::Color::GREEN;
            break;
        case Level::WARN:
            level_string = "WARN: ";
            level_color = Vga3::Color::YELLOW;
            break;
        case Level::ERROR:
            level_string = "ERROR: ";
            level_color = Vga3::Color::RED;
            break;
        case Level::FATAL:
            level_string = "FATAL: ";
            level_color = Vga3::Color::LIGHT_RED;
            break;
        default:
            return;
    }

    Vga3::print(level_string, level_color);
}

Logger::Level Logger::current_level_ = Logger::Level::INFO;
