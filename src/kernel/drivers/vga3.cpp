#include "drivers/vga3.hpp"

#include <type_traits>

#include "memory/layout.hpp"

using namespace drivers;

size_t Vga3::row_ = 0;
size_t Vga3::column_ = 0;

volatile uint16_t *const Vga3::VIDEO_MEMORY =
    reinterpret_cast<volatile uint16_t *const>(memory::Layout::VIDEO);

void Vga3::print(const char *string, Color foreground, Color background) {
    while (*string) {
        print(*(string++), foreground, background);
    }
}

void Vga3::print(char character, Color foreground, Color background) {
    if (character == '\n') {
        row_ += 1;
        column_ = 0;
    } else {
        put_char(row_, column_, character, foreground, background);
        column_++;
    }

    if (column_ >= SCREEN_WIDTH) {
        column_ = 0;
        row_++;
    }

    if (row_ >= SCREEN_HEIGHT) {
        scroll();
        column_ = 0;
        row_ = SCREEN_HEIGHT - 1;
    }
}

void Vga3::clear() {
    for (row_ = 0; row_ < SCREEN_HEIGHT; row_++) {
        for (column_ = 0; column_ < SCREEN_WIDTH; column_++) {
            put_char(row_, column_, ' ', Color::BLACK, Color::BLACK);
        }
    }
    row_ = 0;
    column_ = 0;
}

void Vga3::scroll() {
    for (row_ = 0; row_ < SCREEN_HEIGHT - 1; row_++) {
        for (column_ = 0; column_ < SCREEN_WIDTH; column_++) {
            VIDEO_MEMORY[character_offset(row_, column_)] =
                VIDEO_MEMORY[character_offset(row_ + 1, column_)];
        }
    }

    for (column_ = 0; column_ < SCREEN_WIDTH; column_++) {
        put_char(row_, column_, ' ', Color::BLACK, Color::BLACK);
    }
}

void Vga3::put_char(uint8_t row, uint8_t column, char character,
                    Color foreground, Color background) {
    VIDEO_MEMORY[character_offset(row, column)] =
        make_char(character, foreground, background);
}

size_t Vga3::character_offset(uint8_t row, uint8_t column) {
    return row * SCREEN_WIDTH + column;
}

uint16_t Vga3::make_char(char character, Color foreground, Color background) {
    const uint8_t colors =
        (static_cast<std::underlying_type_t<Color>>(background) << 4) |
        (static_cast<std::underlying_type_t<Color>>(foreground) & 0x0f);
    return (colors << 8) | character;
}
