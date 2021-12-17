#pragma once

class Logger final {
   public:
    enum class Level { DEBUG, INFO, WARN, ERROR, FATAL };

    Logger() = delete;

    /**
     * Log a message with debug level.
     * @param message The message to log.
     */
    static void debug(const char *message);

    /**
     * Log a message with info level.
     * @param message The message to log.
     */
    static void info(const char *message);

    /**
     * Log a message with warn level.
     * @param message The message to log.
     */
    static void warn(const char *message);

    /**
     * Log a message with error level.
     * @param message The message to log.
     */
    static void error(const char *message);

    /**
     * Log a message with fatal level.
     * @param message The message to log.
     */
    static void fatal(const char *message);

    /**
     * Change the current log level of the logger. Messages below the current
     * level will not be displayed.
     * @param new_log_level The log level to set.
     */
    static void set_log_level(Level new_log_level);

   private:
    static void log(const char *message, Level log_level);
    static void print_level(Level log_level);

    static Level current_level_;
};
