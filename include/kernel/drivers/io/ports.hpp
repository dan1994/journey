#pragma once

#include <stdint.h>

/**
 * Convenient IO port access. Port numbers will be filled into the Port enum as
 * needed.
 */

namespace drivers::io {

enum class Port : uint16_t {
    MASTER_PIC_COMMAND = 0x20,
    MASTER_PIC_DATA,
    SLAVE_PIC_COMMAND = 0xa0,
    SLAVE_PIC_DATA
};

/**
 * Read a single byte from the given port.
 * @param port The port to read from.
 * @return The value read from the port.
 */
uint8_t read_byte(Port port);

/**
 * Read a word (2 bytes) from the given port.
 * @param port The port to read from.
 * @return The value read from the port.
 */
uint16_t read_word(Port port);

/**
 * Write a single byte to the given port.
 * @param port The port to write to.
 * @param value The value to write to the port.
 */
void write_byte(Port port, uint8_t value);

/**
 * Write a word (2 bytes) to the given port.
 * @param port The port to write to.
 * @param value The value to write to the port.
 */
void write_word(Port port, uint16_t value);

}  // namespace drivers::io
