#include "drivers/vga3.hpp"
#include "interrupts/interrupts.hpp"
#include "interrupts/isr.hpp"

using namespace drivers;

extern "C" void kernel_main() {
    Vga3::clear();
    Vga3::print("Initializing Journey...\n");

    Interrupts::init();
    Interrupts::register_interrupt(0, isr_divide_by_zero,
                                   Interrupts::PriviledgeLevel::KERNEL,
                                   Interrupts::GateSize::BITS32);

    asm("int $0");
}
