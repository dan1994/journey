#pragma once

#include <stdint.h>

/**
 * Convinient IO port access. Port numbers will be filled into the Port enum as
 * needed.
 */

class Io final {
   public:
    enum class Port : uint16_t {
        MASTER_PIC_COMMAND = 0x20,
        MASTER_PIC_DATA,
        SLAVE_PIC_COMMAND = 0xa0,
        SLAVE_PIC_DATA
    };

    Io() = delete;

    static uint8_t read_byte(Port port);
    static uint16_t read_word(Port port);
    static void write_byte(Port port, uint8_t value);
    static void write_word(Port port, uint16_t value);
};
