#include <stddef.h>

#include <cassert>

char line_number_as_char_ptr[8] = {0};

void line_number_to_char_ptr(size_t line_number) {
    const int MAX_NUMBER_CHARACTERS = sizeof(line_number_as_char_ptr) - 1;
    const int MAX_INDEX_FOR_NUMBER_CHARACTERS = MAX_NUMBER_CHARACTERS - 1;

    int i = MAX_INDEX_FOR_NUMBER_CHARACTERS;

    for (; i >= 0 && line_number > 0; i--) {
        line_number_as_char_ptr[i] = '0' + (line_number % 10);
        line_number /= 10;
    }
    i++;

    int j = 0;
    for (; i <= MAX_INDEX_FOR_NUMBER_CHARACTERS; i++, j++) {
        line_number_as_char_ptr[j] = line_number_as_char_ptr[i];
    }

    for (; j <= MAX_INDEX_FOR_NUMBER_CHARACTERS + 1; j++) {
        line_number_as_char_ptr[j] = '\0';
    }
}
