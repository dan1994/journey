#pragma once

#include <pair>
#include <type_traits>

namespace std {

template <typename T>
struct tuple_size;

template <size_t I, typename T>
struct tuple_element;

template <typename T1, typename T2>
struct tuple_size<std::pair<T1, T2>> : integral_constant<size_t, 2> {};

template <size_t I, typename T1, typename T2>
struct tuple_element<I, std::pair<T1, T2>> {
    static_assert(I < 2, "std::pair has only 2 elements!");
};

template <typename T1, typename T2>
struct tuple_element<0, std::pair<T1, T2>> {
    using type = T1;
};

template <typename T1, typename T2>
struct tuple_element<1, std::pair<T1, T2>> {
    using type = T2;
};

}  // namespace std
