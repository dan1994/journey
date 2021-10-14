#pragma once

#include <type_traits>

template <typename T>
concept Enum = std::is_enum_v<T>;

template <Enum E>
E operator|(E a, E b) {
    return static_cast<E>(static_cast<std::underlying_type_t<E>>(a) |
                          static_cast<std::underlying_type_t<E>>(b));
}

template <Enum E>
E operator&(E a, E b) {
    return static_cast<E>(static_cast<std::underlying_type_t<E>>(a) &
                          static_cast<std::underlying_type_t<E>>(b));
}
