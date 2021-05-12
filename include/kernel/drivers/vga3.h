#pragma once

/**
 * This is the VGA mode 3 driver that includes Text Mode support.
 * See https://wiki.osdev.org/Text_UI for more info.
 */

#include <stddef.h>
#include <stdint.h>

typedef enum {
    VGA3_COLOR_BLACK,
    VGA3_COLOR_BLUE,
    VGA3_COLOR_GREEN,
    VGA3_COLOR_CYAN,
    VGA3_COLOR_RED,
    VGA3_COLOR_MAGENTA,
    VGA3_COLOR_BROWN,
    VGA3_COLOR_LIGHT_GRAY,
    VGA3_COLOR_DARK_GREY,
    VGA3_COLOR_LIGHT_BLUE,
    VGA3_COLOR_LIGHT_GREEN,
    VGA3_COLOR_LIGHT_CYAN,
    VGA3_COLOR_LIGHT_RED,
    VGA3_COLOR_LIGHT_MAGENTA,
    VGA3_COLOR_YELLOW,
    VGA3_COLOR_WHITE
} VGA3_COLOR;

/**
 * Output a string to the screen at the current position.
 *
 * @param string - The string
 * @param foreground - The text color
 * @param background - The background color
 */
void vga3__print(const char *string, VGA3_COLOR foreground,
                 VGA3_COLOR background);

/**
 * Clear the entire terminal.
 */
void vga3__clear();

/**
 * Output a character to the screen at the current position and move to the next
 * position.
 *
 * Advancing the position may include moving to a new line or scrolling.
 *
 * @param character - The character
 * @param foreground - The text color
 * @param background - The background color
 */
void _vga3__write_char(char character, VGA3_COLOR foreground,
                       VGA3_COLOR background);

/**
 * Scroll one line down.
 *
 * This is done by copying the data of each row to the previous row and
 * emptying the last row.
 */
void _vga3__scroll();

/**
 * Output a character to the screen at a given position.
 *
 * @param terminal_row - The row index
 * @param terminal_column - The column index
 * @param character - The character
 * @param foreground - The text color
 * @param background - The background color
 */
void _vga3__put_char(uint8_t row, uint8_t column, char character,
                     VGA3_COLOR foreground, VGA3_COLOR background);

/**
 * Get the linear offset of a character given its position.
 *
 * @param row - The row
 * @param column - The column
 */
size_t _vga3__character_offset(uint8_t row, uint8_t column);

/**
 * Create the binary representation of a character with a specific foreground
 * color, as understood by the text mode controller.
 *
 * @param character - The character
 * @param foreground - The text color
 * @param background - The background color
 */
uint16_t _vga3__make_char(char character, VGA3_COLOR foreground,
                          VGA3_COLOR background);
