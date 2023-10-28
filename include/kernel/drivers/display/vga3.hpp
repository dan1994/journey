#pragma once

/**
 * This is the VGA mode 3 driver that includes Text Mode support.
 * See https://wiki.osdev.org/Text_UI for more info.
 */

#include <stddef.h>
#include <stdint.h>

#include <string>

namespace drivers::display::vga3 {

enum class Color : uint8_t {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHT_GRAY,
    DARK_GREY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    YELLOW,
    WHITE
};

void print(const char *string, Color foreground = Color::LIGHT_GRAY,
           Color background = Color::BLACK);

void print(char character, Color foreground = Color::LIGHT_GRAY,
           Color background = Color::BLACK);

void clear();

}  // namespace drivers::display::vga3
