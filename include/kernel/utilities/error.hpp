#pragma once

#include <pair>

class Error final {
   public:
    Error(const char* message = Error::no_error_);

    operator bool() const;
    const char* to_string() const;

   private:
    const char* message_;

    static const char* const no_error_;
};

template <typename T>
using WithError = std::pair<T, Error>;
