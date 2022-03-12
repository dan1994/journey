#pragma once

namespace std {

enum class byte : unsigned char {};

template <class IntegerType>
constexpr IntegerType to_integer(byte b) noexcept;

template <class IntegerType>
constexpr byte& operator<<=(byte& b, IntegerType shift) noexcept;
template <class IntegerType>
constexpr byte& operator>>=(byte& b, IntegerType shift) noexcept;

template <class IntegerType>
constexpr byte operator<<(byte b, IntegerType shift) noexcept;
template <class IntegerType>
constexpr byte operator>>(byte b, IntegerType shift) noexcept;

constexpr byte& operator|=(byte& l, byte r) noexcept;
constexpr byte& operator&=(byte& l, byte r) noexcept;
constexpr byte& operator^=(byte& l, byte r) noexcept;

constexpr byte operator|(byte l, byte r) noexcept;
constexpr byte operator&(byte l, byte r) noexcept;
constexpr byte operator^(byte l, byte r) noexcept;
constexpr byte operator~(byte b) noexcept;

template <class IntegerType>
constexpr IntegerType to_integer(byte b) noexcept {
    return IntegerType(b);
}

template <class IntegerType>
constexpr byte& operator<<=(byte& b, IntegerType shift) noexcept {
    return b = b << shift;
}

template <class IntegerType>
constexpr byte& operator>>=(byte& b, IntegerType shift) noexcept {
    return b = b >> shift;
}

template <class IntegerType>
constexpr byte operator<<(byte b, IntegerType shift) noexcept {
    return std::byte(static_cast<unsigned int>(b) << shift);
}

template <class IntegerType>
constexpr byte operator>>(byte b, IntegerType shift) noexcept {
    return std::byte(static_cast<unsigned int>(b) >> shift);
}

}  // namespace std
