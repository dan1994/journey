#pragma once

#include <type_traits_internal/integral_constant.hpp>
#include <type_traits_internal/type_manipulation.hpp>

namespace std {

// is_same
template <class T, class U>
struct is_same : false_type {};

template <class T>
struct is_same<T, T> : true_type {};

template <class T, class U>
inline constexpr bool is_same_v = is_same<T, U>::value;

// is_convertible
template <class T>
struct is_void : is_same<void, typename remove_cv<T>::type> {};

template <class T>
inline constexpr bool is_void_v = is_void<T>::value;

namespace detail {

template <class T>
struct type_identity {
    using type = T;
};

template <class T>
auto try_add_lvalue_reference(int) -> type_identity<T&>;
template <class T>
auto try_add_lvalue_reference(...) -> type_identity<T>;

template <class T>
auto try_add_rvalue_reference(int) -> type_identity<T&&>;
template <class T>
auto try_add_rvalue_reference(...) -> type_identity<T>;

}  // namespace detail

template <class T>
struct add_lvalue_reference : decltype(detail::try_add_lvalue_reference<T>(0)) {
};

template <class T>
struct add_rvalue_reference : decltype(detail::try_add_rvalue_reference<T>(0)) {
};

template <class T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

template <class T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

template <class T>
add_rvalue_reference_t<T> declval();

namespace detail {

template <class T>
auto test_returnable(int)
    -> decltype(void(static_cast<T (*)()>(nullptr)), true_type{});
template <class>
auto test_returnable(...) -> false_type;

template <class From, class To>
auto test_implicitly_convertible(int)
    -> decltype(void(declval<void (&)(To)>()(declval<From>())), true_type{});
template <class, class>
auto test_implicitly_convertible(...) -> false_type;

}  // namespace detail

template <class From, class To>
struct is_convertible
    : integral_constant<
          bool,
          (decltype(detail::test_returnable<To>(0))::
               value&& decltype(detail::test_implicitly_convertible<From, To>(
                   0))::value) ||
              (is_void_v<From> && is_void_v<To>)> {};

template <class From, class To>
inline constexpr bool is_convertible_v = is_convertible<From, To>::value;

}  // namespace std
