#include "kernel/utils.hpp"

#include "kernel/drivers/vga3.hpp"

using namespace drivers;

void print(const char *string) {
    Vga3::print(string, Vga3Color::LIGHT_GRAY, Vga3Color::BLACK);
}

void clear() {
    Vga3::clear();
}
