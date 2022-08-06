#include "drivers/io/ports.hpp"

extern "C" uint8_t insb(drivers::io::Port port);
extern "C" uint16_t insw(drivers::io::Port port);
extern "C" void outb(drivers::io::Port port, uint8_t value);
extern "C" void outw(drivers::io::Port port, uint16_t value);

namespace drivers::io {

uint8_t read_byte(Port port) {
    return insb(port);
}

uint16_t read_word(Port port) {
    return insw(port);
}

void write_byte(Port port, uint8_t value) {
    outb(port, value);
}

void write_word(Port port, uint16_t value) {
    outw(port, value);
}

void short_delay() {
    write_byte(Port::DEFAULT_UNUSED, 0);
}

}  // namespace drivers::io
