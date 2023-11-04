#include "drivers/interrupts/pic.hpp"
#include "interrupts/idt.hpp"
#include "logging/logger.hpp"

/**
 * This file should contain all ISR methods.
 * It's compiled with the `-mgeneral-regs-only` flag which is required for the
 * correct generation of ISR methods. See
 * https://wiki.osdev.org/Interrupt_Service_Routines#GCC_.2F_G.2B.2B for more
 * info.
 */

extern "C" void isr_divide_by_zero() {
    logging::error("divide by zero");
}

extern "C" void isr_pic_timer() {
    drivers::interrupts::Pic8259::signal_end_of_interrupt(
        interrupts::Id::PIC_TIMER);
}

extern "C" void isr_pic_hdd() {
    drivers::interrupts::Pic8259::signal_end_of_interrupt(
        interrupts::Id::PIC_HDD);
}

extern "C" void isr_pic_keyboard() {
    logging::info("key was pressed");
    drivers::interrupts::Pic8259::signal_end_of_interrupt(
        interrupts::Id::PIC_KEYBOARD);
}
