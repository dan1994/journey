#pragma once

#include <type_traits_internal/integral_constant.hpp>

namespace std {

template <typename T>
struct is_enum : public integral_constant<bool, __is_enum(T)> {};

template <typename T>
inline constexpr bool is_enum_v = is_enum<T>::value;

template <typename Enum>
using underlying_type_t = __underlying_type(Enum);

}  // namespace std
