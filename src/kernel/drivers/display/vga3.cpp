#include "drivers/display/vga3.hpp"

#include <type_traits>

#include "memory/layout.hpp"

namespace drivers::display::vga3 {

volatile uint16_t *const VIDEO_MEMORY =
    reinterpret_cast<volatile uint16_t *const>(memory::Layout::VIDEO);

constexpr static size_t SCREEN_WIDTH_IN_CHARACTERS = 80;
constexpr static size_t SCREEN_HEIGHT_IN_CHARACTERS = 25;

static size_t row = 0;
static size_t column = 0;

static bool is_non_printable(char character);
static void scroll();
static void put_char(uint8_t row, uint8_t column, char character,
                     Color foreground, Color background);
static size_t character_offset(uint8_t row, uint8_t column);
static uint16_t make_char(char character, Color foreground, Color background);

void print(const char *string, Color foreground, Color background) {
    while (*string) {
        print(*(string++), foreground, background);
    }
}

void print(char character, Color foreground, Color background) {
    if (is_non_printable(character)) {
        character = ' ';
    }

    if (character == '\n') {
        row += 1;
        column = 0;
    } else {
        put_char(row, column, character, foreground, background);
        column++;
    }

    if (column >= SCREEN_WIDTH_IN_CHARACTERS) {
        column = 0;
        row++;
    }

    if (row >= SCREEN_HEIGHT_IN_CHARACTERS) {
        scroll();
        column = 0;
        row = SCREEN_HEIGHT_IN_CHARACTERS - 1;
    }
}

void clear() {
    for (row = 0; row < SCREEN_HEIGHT_IN_CHARACTERS; row++) {
        for (column = 0; column < SCREEN_WIDTH_IN_CHARACTERS; column++) {
            put_char(row, column, ' ', Color::BLACK, Color::BLACK);
        }
    }
    row = 0;
    column = 0;
}

bool is_non_printable(char character) {
    constexpr char LAST_NON_PRINTABLE_CHARACTER = 31;
    return character <= LAST_NON_PRINTABLE_CHARACTER && character != '\n';
}

void scroll() {
    for (row = 0; row < SCREEN_HEIGHT_IN_CHARACTERS - 1; row++) {
        for (column = 0; column < SCREEN_WIDTH_IN_CHARACTERS; column++) {
            VIDEO_MEMORY[character_offset(row, column)] =
                VIDEO_MEMORY[character_offset(row + 1, column)];
        }
    }

    for (column = 0; column < SCREEN_WIDTH_IN_CHARACTERS; column++) {
        put_char(row, column, ' ', Color::BLACK, Color::BLACK);
    }
}

void put_char(uint8_t row, uint8_t column, char character, Color foreground,
              Color background) {
    VIDEO_MEMORY[character_offset(row, column)] =
        make_char(character, foreground, background);
}

size_t character_offset(uint8_t row, uint8_t column) {
    return row * SCREEN_WIDTH_IN_CHARACTERS + column;
}

uint16_t make_char(char character, Color foreground, Color background) {
    const uint8_t colors =
        (static_cast<std::underlying_type_t<Color>>(background) << 4) |
        (static_cast<std::underlying_type_t<Color>>(foreground) & 0x0f);
    return (colors << 8) | character;
}

}  // namespace drivers::display::vga3
