#include "utilities/error.hpp"

Error::Error(const char *message) : message_(message) {
    if (message_ == nullptr) {
        message_ = Error::no_error_;
    }
}

Error::operator bool() const {
    return *message_ != 0;
}

const char *Error::to_string() const {
    return message_;
}

const char *const Error::no_error_ = "";
