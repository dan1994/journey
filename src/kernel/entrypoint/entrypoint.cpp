#include "kernel/drivers/vga3.hpp"

using namespace drivers;

extern "C" void kernel_start() {
    Vga3::clear();
    Vga3::print("Initializing DanOS...\n");
}
