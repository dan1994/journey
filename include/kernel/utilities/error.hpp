#pragma once

#include <stddef.h>

#include <pair>

#include "utilities/macros.hpp"

namespace errors {

constexpr size_t MAX_DEPTH = 10;

struct error {
    const char* messages[MAX_DEPTH];
};

error nil();
error make(const char* message);
bool set(error error);
void enrich(error* error, const char* message);
void log(error error);

template <typename T>
using with_error = std::pair<T, error>;

}  // namespace errors

using errors::error;
using errors::with_error;

#define WITH_LOCATION(message) __FILE__ ":" STRINGIZE(__LINE__) ": " message
