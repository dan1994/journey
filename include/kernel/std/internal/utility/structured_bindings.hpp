#pragma once

#include <stddef.h>

namespace std {

template <typename T>
struct tuple_size;

template <size_t I, typename T>
struct tuple_element;

}  // namespace std
