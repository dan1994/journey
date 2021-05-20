#include "drivers/vga3.hpp"
#include "interrupts/interrupts.hpp"

using namespace drivers;

extern "C" void kernel_main() {
    Vga3::clear();
    Vga3::print("Initializing Journey...\n");

    Interrupts::init();
    Vga3::print("Initialized interrupts...\n");
}
