#include "kernel/utils.h"

#include "kernel/drivers/vga3.h"

void print(const char *string) {
    vga3__print(string, VGA3_COLOR_LIGHT_GRAY, VGA3_COLOR_BLACK);
}

void clear() {
    vga3__clear();
}
