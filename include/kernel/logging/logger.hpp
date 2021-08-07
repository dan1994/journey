#pragma once

class Logger final {
   public:
    enum class Level { DEBUG, INFO, WARN, ERROR, FATAL };

    Logger() = delete;

    static void debug(const char *message);
    static void info(const char *message);
    static void warn(const char *message);
    static void error(const char *message);
    static void fatal(const char *message);

    static void set_log_level(Level new_log_level);

   private:
    static void log(const char *message, Level log_level);

    static Level log_level;
};
