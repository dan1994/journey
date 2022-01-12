#pragma once

#include <stddef.h>
#include <stdint.h>

namespace utilities {

/**
 * Get the value of a bit flag in a given value.
 * @param value The given value.
 * @param offset The offset of the flag in the value.
 * @return The value of the bit in the given offset.
 */
bool get_flag(uint32_t value, size_t offset);

/**
 * Set the value of a bit flag in a given value to 1.
 * @param value The given value.
 * @param offset The offset of the flag in the value.
 * @return The value after the change.
 */
uint32_t set_flag(uint32_t value, size_t offset);

/**
 * Set the value of a bit flag in a given value to 0.
 * @param value The given value.
 * @param offset The offset of the flag in the value.
 * @return The value after the change.
 */
uint32_t clear_flag(uint32_t value, size_t offset);

/**
 * Get the value of a bit field in a given value.
 * @param value The given value.
 * @param msb The offset of the most significant bit in the value.
 * @param lsb The offset of the most significant bit in the value.
 * @return The value of the field between the given offsets.
 */
uint32_t get_field(uint32_t value, size_t msb, size_t lsb);

/**
 * Set the value of a bit field in a given value.
 * @param value The given value.
 * @param msb The offset of the most significant bit in the value.
 * @param lsb The offset of the most significant bit in the value.
 * @return The value after the change.
 */
uint32_t set_field(uint32_t value, size_t msb, size_t lsb,
                   uint32_t field_value);

}  // namespace utilities
