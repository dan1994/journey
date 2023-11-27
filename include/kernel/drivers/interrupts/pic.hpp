#pragma once

#include <stdint.h>

/**
 * The Programmable Interrupt Controller (PIC) is responsible for handling
 * interrupts from other hardware components such as the keyboard, mouse, etc.
 * See https://wiki.osdev.org/PIC for more info.
 */

namespace interrupts {
enum class Id : uint8_t;
}

namespace drivers::interrupts::pic8259 {

constexpr uint8_t MASTER_OFFSET = 0x20;
constexpr uint8_t SLAVE_OFFSET = 0x28;

void init();

/**
 * Alert the PIC controllers that an interrupt was handled.
 *
 * @param interrupt The interrupt that was handled
 */
void signal_end_of_interrupt(::interrupts::Id interrupt);

}  // namespace drivers::interrupts::pic8259
