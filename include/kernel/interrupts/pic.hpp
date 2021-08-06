#pragma once

#include <stdint.h>

#include "interrupts/interrupts.hpp"
#include "io/io.hpp"

/**
 * The PIC is responsible for handling interrupts from other hardware components
 * such as the keyboard, mouse, etc.
 * See https://wiki.osdev.org/PIC for more info.
 */

class ProgrammableInterruptController final {
   public:
    enum class Id { MASTER, SLAVE };

    ProgrammableInterruptController() = delete;

    /**
     * Initialize and tell a PIC controller its offset in the IDT.
     *
     * @param controller Master or slave controller
     * @param idt_offset The offset in the IDT where interrupts from this PIC
     * start
     */
    static void remap(Id controller, uint8_t idt_offset);

    /**
     * Alert the PIC controllers that an interrupt was handled.
     *
     * @param interrupt The interrupt that was handled
     */
    static void signal_end_of_interrupt(Interrupts::Id interrupt);

   private:
    [[nodiscard]] static Io::Port get_command_port(Id controller);
    [[nodiscard]] static Io::Port get_data_port(Id controller);
    [[nodiscard]] static Id get_controller(Interrupts::Id interrupt);

    static uint8_t idt_offset;
};
