#include "interrupts/isr.hpp"

#include "drivers/vga3.hpp"
#include "interrupts/interrupts.hpp"
#include "interrupts/pic.hpp"

using namespace drivers;

#define ISR(NAME, BODY)                          \
    extern "C" void isr_##NAME(void *reserved) { \
        Interrupts::disable();                   \
        BODY;                                    \
        Interrupts::enable();                    \
    }

#define PIC_ISR(NAME, BODY, ID) \
    ISR(NAME, BODY;             \
        ProgrammableInterruptController::signal_end_of_interrupt(ID);)

ISR(divide_by_zero, { Vga3::print("Divide by zero error!\n"); })

PIC_ISR(acknowledge_interrupt, {}, Interrupt::PIC_TIMER);

PIC_ISR(
    keyboard_press, { Vga3::print("Key was pressed!\n"); },
    Interrupt::PIC_KEYBOARD)
