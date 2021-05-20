#include "io/io.hpp"

extern "C" uint8_t insb(Io::Port port);
extern "C" uint16_t insw(Io::Port port);
extern "C" void outb(Io::Port port, uint8_t value);
extern "C" void outw(Io::Port port, uint16_t value);

uint8_t Io::read_byte(Port port) {
    return insb(port);
}

uint16_t Io::read_word(Port port) {
    return insw(port);
}

void Io::write_byte(Port port, uint8_t value) {
    outb(port, value);
}

void Io::write_word(Port port, uint16_t value) {
    outw(port, value);
}
