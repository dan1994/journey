#pragma once

#include <type_traits>

template <typename T>
concept Enum = std::is_enum_v<T>;

/**
 * Bitwise or operation between enum class values.
 *
 * @param a First value
 * @param b Second value
 * @return The bitwise or computation of their values cast to the enum class
 * type.
 */
template <Enum E>
[[nodiscard]] E operator|(E a, E b) {
    return static_cast<E>(static_cast<std::underlying_type_t<E>>(a) |
                          static_cast<std::underlying_type_t<E>>(b));
}

/**
 * Bitwise and operation between enum class values.
 *
 * @param a First value
 * @param b Second value
 * @return The bitwise and computation of their values cast to the enum class
 * type.
 */
template <Enum E>
[[nodiscard]] E operator&(E a, E b) {
    return static_cast<E>(static_cast<std::underlying_type_t<E>>(a) &
                          static_cast<std::underlying_type_t<E>>(b));
}
