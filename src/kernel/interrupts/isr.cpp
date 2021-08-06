#include "interrupts/isr.hpp"

#include "drivers/vga3.hpp"
#include "interrupts/pic.hpp"

using namespace drivers;

extern "C" void isr_divide_by_zero(void *reserved) {
    Vga3::print("Divide by zero error!\n");
}

extern "C" void isr_keyboard_press(void *reserved) {
    Vga3::print("Key was pressed!\n");
    ProgrammableInterruptController::signal_end_of_interrupt(
        Interrupts::Id::PIC_KEYBOARD);
}
