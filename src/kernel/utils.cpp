#include "kernel/utils.hpp"

#include "kernel/drivers/vga3.hpp"

using namespace drivers;

void print(const char *string) {
    Vga3::print(string, Vga3::Color::LIGHT_GRAY, Vga3::Color::BLACK);
}

void clear() {
    Vga3::clear();
}
