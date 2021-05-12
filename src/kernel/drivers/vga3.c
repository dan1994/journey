#include "kernel/drivers/vga3.h"

static volatile uint16_t *const VIDEO_MEMORY = (uint16_t *)0xb8000;
static const size_t SCREEN_WIDTH = 80;
static const size_t SCREEN_HEIGHT = 25;

static uint8_t row = 0;
static uint8_t column = 0;

void vga3__print(const char *string, VGA3_COLOR foreground,
                 VGA3_COLOR background) {
    while (*string) {
        _vga3__write_char(*(string++), foreground, background);
    }
}

void vga3__clear() {
    for (row = 0; row < SCREEN_HEIGHT; row++) {
        for (column = 0; column < SCREEN_WIDTH; column++) {
            _vga3__put_char(row, column, ' ', VGA3_COLOR_BLACK,
                            VGA3_COLOR_BLACK);
        }
    }
    row = 0;
    column = 0;
}

void _vga3__write_char(char character, VGA3_COLOR foreground,
                       VGA3_COLOR background) {
    if (character == '\n') {
        row += 1;
        column = 0;
    } else {
        _vga3__put_char(row, column, character, foreground, background);
        column++;
    }

    if (column >= SCREEN_WIDTH) {
        column = 0;
        row++;
    }

    if (row >= SCREEN_HEIGHT) {
        _vga3__scroll();
        column = 0;
        row = SCREEN_HEIGHT - 1;
    }
}

void _vga3__scroll() {
    for (row = 0; row < SCREEN_HEIGHT - 1; row++) {
        for (column = 0; column < SCREEN_WIDTH; column++) {
            VIDEO_MEMORY[_vga3__character_offset(row, column)] =
                VIDEO_MEMORY[_vga3__character_offset(row + 1, column)];
        }
    }

    for (column = 0; column < SCREEN_WIDTH; column++) {
        _vga3__put_char(row, column, ' ', VGA3_COLOR_BLACK, VGA3_COLOR_BLACK);
    }
}

void _vga3__put_char(uint8_t row, uint8_t column, char character,
                     VGA3_COLOR foreground, VGA3_COLOR background) {
    VIDEO_MEMORY[_vga3__character_offset(row, column)] =
        _vga3__make_char(character, foreground, background);
}

size_t _vga3__character_offset(uint8_t row, uint8_t column) {
    return row * SCREEN_WIDTH + column;
}

uint16_t _vga3__make_char(char character, VGA3_COLOR foreground,
                          VGA3_COLOR background) {
    return ((foreground | background << 4) << 8) | character;
}
