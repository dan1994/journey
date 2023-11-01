#include "interrupts/isr.hpp"

#include "drivers/interrupts/pic.hpp"
#include "interrupts/idt.hpp"
#include "interrupts/interrupts.hpp"
#include "logging/logger.hpp"

#define ISR(NAME, BODY)                          \
    extern "C" void isr_##NAME(void *reserved) { \
        DISABLE_INTERRUPTS();                    \
        BODY ENABLE_INTERRUPTS();                \
    }

#define PIC_ISR(NAME, BODY, ID) \
    ISR(NAME, BODY drivers::interrupts::Pic8259::signal_end_of_interrupt(ID);)

ISR(divide_by_zero, logging::error("Divide by zero error!");)

PIC_ISR(acknowledge_interrupt, , interrupts::Id::PIC_TIMER);

PIC_ISR(keyboard_press, logging::info("Key was pressed!\n");
        , interrupts::Id::PIC_KEYBOARD)
