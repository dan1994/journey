#pragma once

#include <internal/type_traits/concepts.hpp>
#include <internal/type_traits/integral_constant.hpp>

namespace std {

// is_union
template <typename _Tp>
struct is_union : public integral_constant<bool, __is_union(_Tp)> {};

template <class T>
inline constexpr bool is_union_v = is_union<T>::value;

// is_class
namespace detail {

template <class T>
std::integral_constant<bool, !std::is_union<T>::value> test(int T::*);

template <class>
std::false_type test(...);

}  // namespace detail

template <class T>
struct is_class : decltype(detail::test<T>(nullptr)) {};

// is_base_of
namespace details {
template <typename B>
std::true_type test_pre_ptr_convertible(const volatile B*);
template <typename>
std::false_type test_pre_ptr_convertible(const volatile void*);

template <typename, typename>
auto test_pre_is_base_of(...) -> std::true_type;
template <typename B, typename D>
auto test_pre_is_base_of(int)
    -> decltype(test_pre_ptr_convertible<B>(static_cast<D*>(nullptr)));
}  // namespace details

template <typename Base, typename Derived>
struct is_base_of
    : std::integral_constant<
          bool,
          std::is_class<Base>::value &&
              std::is_class<Derived>::
                  value&& decltype(details::test_pre_is_base_of<Base, Derived>(
                      0))::value> {};

template <class Base, class Derived>
inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;

// derived_from concept
template <class Derived, class Base>
concept derived_from = std::is_base_of_v<Base, Derived> &&
    std::is_convertible_v<const volatile Derived*, const volatile Base*>;

}  // namespace std
