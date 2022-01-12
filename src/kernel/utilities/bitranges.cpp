#include "utilities/bitranges.hpp"

namespace utilities {

bool get_flag(uint32_t value, size_t offset) {
    return (value & (1 << offset)) >> offset;
}

uint32_t set_flag(uint32_t value, size_t offset) {
    return value | (1 << offset);
}

uint32_t clear_flag(uint32_t value, size_t offset) {
    return value & ~(1 << offset);
}

uint32_t get_field(uint32_t value, size_t msb, size_t lsb) {
    return (value >> lsb) & ((1 << (msb - lsb + 1)) - 1);
}

uint32_t set_field(uint32_t value, size_t msb, size_t lsb,
                   uint32_t field_value) {
    const uint32_t mask = ((1 << (msb - lsb + 1)) - 1) << lsb;
    return (value & ~mask) | ((field_value << lsb) & mask);
}

}  // namespace utilities
