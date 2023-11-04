#pragma once

// Declares an interrupt method. Note that this declaration points to an
// assembly wrapper that calls a C function without the _wrapper suffix. The C
// functions are intentionally not declared here to avoid using them instead of
// the wrappers.
#define INTERRUPT_DECLARATION(NAME) extern "C" void isr_##NAME##_wrapper()

INTERRUPT_DECLARATION(divide_by_zero);

// Programmable interrupt controller interrupts
INTERRUPT_DECLARATION(pic_timer);
INTERRUPT_DECLARATION(pic_hdd);
INTERRUPT_DECLARATION(pic_keyboard);
