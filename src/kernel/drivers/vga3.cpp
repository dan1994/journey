#include "kernel/drivers/vga3.hpp"

using namespace drivers;

size_t Vga3::row = 0;
size_t Vga3::column = 0;

volatile uint16_t *const Vga3::VIDEO_MEMORY =
    reinterpret_cast<volatile uint16_t *const>(0xb8000);

void Vga3::print(const char *string, Vga3Color foreground,
                 Vga3Color background) {
    while (*string) {
        Vga3::write_char(*(string++), foreground, background);
    }
}

void Vga3::clear() {
    for (row = 0; row < SCREEN_HEIGHT; row++) {
        for (column = 0; column < SCREEN_WIDTH; column++) {
            Vga3::put_char(row, column, ' ', Vga3Color::BLACK,
                           Vga3Color::BLACK);
        }
    }
    row = 0;
    column = 0;
}

void Vga3::write_char(char character, Vga3Color foreground,
                      Vga3Color background) {
    if (character == '\n') {
        row += 1;
        column = 0;
    } else {
        Vga3::put_char(row, column, character, foreground, background);
        column++;
    }

    if (column >= SCREEN_WIDTH) {
        column = 0;
        row++;
    }

    if (row >= SCREEN_HEIGHT) {
        Vga3::scroll();
        column = 0;
        row = SCREEN_HEIGHT - 1;
    }
}

void Vga3::scroll() {
    for (row = 0; row < SCREEN_HEIGHT - 1; row++) {
        for (column = 0; column < SCREEN_WIDTH; column++) {
            VIDEO_MEMORY[Vga3::character_offset(row, column)] =
                VIDEO_MEMORY[Vga3::character_offset(row + 1, column)];
        }
    }

    for (column = 0; column < SCREEN_WIDTH; column++) {
        Vga3::put_char(row, column, ' ', Vga3Color::BLACK, Vga3Color::BLACK);
    }
}

void Vga3::put_char(uint8_t row, uint8_t column, char character,
                    Vga3Color foreground, Vga3Color background) {
    VIDEO_MEMORY[Vga3::character_offset(row, column)] =
        Vga3::make_char(character, foreground, background);
}

size_t Vga3::character_offset(uint8_t row, uint8_t column) {
    return row * SCREEN_WIDTH + column;
}

uint16_t Vga3::make_char(char character, Vga3Color foreground,
                         Vga3Color background) {
    const uint8_t colors = (static_cast<uint8_t>(background) << 4) |
                           (static_cast<uint8_t>(foreground) & 0x0f);
    return (colors << 8) | character;
}
