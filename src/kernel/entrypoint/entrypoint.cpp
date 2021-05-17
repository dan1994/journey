#include "kernel/drivers/vga3.hpp"
#include "kernel/interrupts/interrupts.hpp"
#include "kernel/interrupts/isr.hpp"

using namespace drivers;

extern "C" void kernel_start() {
    Vga3::clear();
    Vga3::print("Initializing DanOS...\n");

    Interrupts::init();
    Interrupts::register_interrupt(0, isr_divide_by_zero,
                                   Interrupts::PriviledgeLevel::KERNEL,
                                   Interrupts::GateType::INTERRUPT32);

    asm("int $0");
}
