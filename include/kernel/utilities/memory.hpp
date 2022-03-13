#pragma once

#include <stddef.h>

namespace utilities {

template <typename T>
[[nodiscard]] T* uninitialized_array_of(size_t size) {
    return reinterpret_cast<T*>(new unsigned char[sizeof(T) * size]);
}

}  // namespace utilities
