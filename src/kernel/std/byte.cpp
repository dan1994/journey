#include <std/byte.hpp>

namespace std {

constexpr byte& operator|=(byte& l, byte r) noexcept {
    return l = l | r;
}

constexpr byte& operator&=(byte& l, byte r) noexcept {
    return l = l & r;
}
constexpr byte& operator^=(byte& l, byte r) noexcept {
    return l = l ^ r;
}

constexpr byte operator|(byte l, byte r) noexcept {
    return byte(static_cast<unsigned int>(l) | static_cast<unsigned int>(r));
}

constexpr byte operator&(byte l, byte r) noexcept {
    return byte(static_cast<unsigned int>(l) & static_cast<unsigned int>(r));
}

constexpr byte operator^(byte l, byte r) noexcept {
    return byte(static_cast<unsigned int>(l) ^ static_cast<unsigned int>(r));
}

constexpr byte operator~(byte b) noexcept {
    return byte(~static_cast<unsigned int>(b));
}

}  // namespace std
