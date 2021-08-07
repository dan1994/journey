#include "interrupts/isr.hpp"

#include "drivers/vga3.hpp"
#include "interrupts/pic.hpp"

using namespace drivers;

#define ISR_CALLBACK(NAME, BODY)                 \
    extern "C" void isr_##NAME(void *reserved) { \
        __asm__("cli;");                         \
                                                 \
        BODY;                                    \
                                                 \
        __asm__("sti;");                         \
    }

#define PIC_ISR_CALLBACK(NAME, BODY, ID) \
    ISR_CALLBACK(                        \
        NAME, BODY;                      \
        ProgrammableInterruptController::signal_end_of_interrupt(ID);)

ISR_CALLBACK(divide_by_zero, { Vga3::print("Divide by zero error!\n"); })

PIC_ISR_CALLBACK(
    keyboard_press, { Vga3::print("Key was pressed!\n"); },
    Interrupts::Id::PIC_KEYBOARD)
