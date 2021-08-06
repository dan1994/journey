#pragma once

/**
 * This file and its corresponding source should contain all ISR methods.
 * The source is compiled with the `-mgeneral-regs-only` flag which is required
 * for the correct generation of ISR methods.
 * See https://wiki.osdev.org/Interrupt_Service_Routines#GCC_.2F_G.2B.2B for
 * more info.
 */

extern "C" void isr_divide_by_zero(void *reserved) __attribute__((interrupt));
extern "C" void isr_keyboard_press(void *reserved) __attribute__((interrupt));
