#pragma once

namespace std {

template <typename T, T v>
struct integral_constant {
    using value_type = T;
    using type = integral_constant;

    static constexpr T value = v;

    constexpr operator value_type() const noexcept {
        return value;
    }

    constexpr value_type operator()() const noexcept {
        return value;
    }
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

}  // namespace std
