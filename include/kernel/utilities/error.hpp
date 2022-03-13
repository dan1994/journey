#pragma once

#include <pair>
#include <string>

#include "utilities/macros.hpp"

class Error final {
   public:
    /**
     * Create a new error with the given message.
     *
     * @param message The error message.
     */
    Error(const char* message = Error::no_error_);

    /**
     * Convert the error to a bool.
     *
     * @return True iff the message is not empty.
     */
    [[nodiscard]] operator bool() const;

    /**
     * Get the message as a char array.
     *
     * @return The message.
     */
    [[nodiscard]] const char* message() const;

    /**
     * Get the message as a std::string.
     *
     * @return The message.
     */
    [[nodiscard]] std::string to_string() const;

   private:
    const char* message_;

    static const char* const no_error_;
};

template <typename T>
using WithError = std::pair<T, Error>;

#define WITH_LOCATION(message) __FILE__ ":" STRINGIZE(__LINE__) ": " message
