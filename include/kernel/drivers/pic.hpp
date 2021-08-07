#pragma once

#include <stdint.h>

#include "io/io.hpp"

/**
 * The PIC is responsible for handling interrupts from other hardware components
 * such as the keyboard, mouse, etc.
 * See https://wiki.osdev.org/PIC for more info.
 */

// Forward declare to avoid circular header dependency
enum class Interrupt : uint8_t;

namespace drivers {

class ProgrammableInterruptController final {
   public:
    // The offset in the IDT where we put Programmable Interrupt Controller
    // interrupts.
    static constexpr uint8_t MASTER_OFFSET = 0x20;
    static constexpr uint8_t SLAVE_OFFSET = 0x70;

    enum class Id { MASTER, SLAVE, NONE };

    ProgrammableInterruptController() = delete;

    /**
     * Initialize the controllers, and remap them to the correct offsets in IDT.
     */
    static void init();

    /**
     * Alert the PIC controllers that an interrupt was handled.
     *
     * @param interrupt The interrupt that was handled
     */
    static void signal_end_of_interrupt(Interrupt interrupt);

   private:
    static void remap_master();
    static void remap_slave();
    static void remap(Io::Port command_port, Io::Port data_port,
                      uint8_t idt_offest, uint8_t connection_information);
    [[nodiscard]] static Id get_controller(Interrupt interrupt);

    static uint8_t idt_offset;
};

}  // namespace drivers
