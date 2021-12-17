#pragma once

/**
 * This is the VGA mode 3 driver that includes Text Mode support.
 * See https://wiki.osdev.org/Text_UI for more info.
 */

#include <stddef.h>
#include <stdint.h>

namespace drivers {

class Vga3 final {
   public:
    Vga3() = delete;

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

    /**
     * Output a string to the screen at the current position.
     *
     * @param string The string
     * @param foreground The text color
     * @param background The background color
     */
    static void print(const char *string, Color foreground = Color::LIGHT_GRAY,
                      Color background = Color::BLACK);

    /**
     * Output a character to the screen at the current position and move to the
     * next position.
     *
     * Advancing the position may include moving to a new line or scrolling.
     *
     * @param character The character
     * @param foreground The text color
     * @param background The background color
     */
    static void print(char character, Color foreground = Color::LIGHT_GRAY,
                      Color background = Color::BLACK);

    /**
     * Clear the entire terminal.
     */
    static void clear();

   private:
    /**
     * Scroll one line down.
     *
     * This is done by copying the data of each row to the previous row and
     * emptying the last row.
     */
    static void scroll();

    /**
     * Output a character to the screen at a given position.
     *
     * @param terminal_row The row index
     * @param terminal_column The column index
     * @param character The character
     * @param foreground The text color
     * @param background The background color
     */
    static void put_char(uint8_t row, uint8_t column, char character,
                         Color foreground, Color background);

    /**
     * Get the linear offset of a character given its position.
     *
     * @param row The row
     * @param column The column
     * @return The linear offset matching the row and column
     */
    static size_t character_offset(uint8_t row, uint8_t column);

    /**
     * Create the binary representation of a character with a specific
     * foreground color, as understood by the text mode controller.
     *
     * @param character The character
     * @param foreground The text color
     * @param background The background color
     * @return The reperesentation in memory of the character with the given
     * colors
     */
    static uint16_t make_char(char character, Color foreground,
                              Color background);

    static size_t row_;
    static size_t column_;

    static volatile uint16_t *const VIDEO_MEMORY;

    // Measured in characters
    constexpr static size_t SCREEN_WIDTH = 80;
    constexpr static size_t SCREEN_HEIGHT = 25;
};

}  // namespace drivers
