#pragma once

#include <type_traits>

namespace std {

template <class T>
constexpr std::remove_reference_t<T>&& move(T&& t);

template <class T>
constexpr typename remove_reference<T>::type&& move(T&& param) {
    using ReturnType = typename remove_reference<T>::type&&;
    return static_cast<ReturnType>(param);
}

}  // namespace std
