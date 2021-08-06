#include <string.h>

void *memset(void *destination, int value, size_t size) {
    char *const destination_char = static_cast<char *>(destination);

    for (size_t i = 0; i < size; i++) {
        destination_char[i] = value;
    }

    return destination;
}

void *memcpy(void *destination, const void *source, size_t size) {
    char *const destination_char = static_cast<char *>(destination);
    const char *const source_char = static_cast<const char *>(source);

    for (size_t i = 0; i < size; i++) {
        destination_char[i] = source_char[i];
    }

    return destination;
}
