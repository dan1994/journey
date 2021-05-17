#pragma once

/**
 * Implementation of the Interrupt Descriptor Table (IDT), that allows setting
 * up interrupt handlers to be called from user space or the kernel.
 * See https://wiki.osdev.org/IDT for more info.
 */

#include "kernel/interrupts/idt.hpp"

class Interrupts final {
   public:
    // ISR = Interrupt Service Routine
    using Isr = void (*)(void *);

    Interrupts() = delete;

    enum class PriviledgeLevel : uint8_t { KERNEL, RING_1, RING_2, USER };

    enum class GateType : uint8_t {
        TASK16 = 0x5,
        INTERRUPT16,
        TRAP16,
        INTERRUPT32 = 0xe,
        TRAP32
    };

    /**
     * Initializes the IDT register. Must be called for interrupts to work.
     */
    static void init();

    /**
     * Registers an interrupt handler with the interrupt number.
     *
     * @param interrupt_number The number used to activate the interrupt handler
     * @param handler_address The address of the handler routine
     * @param dpl The ring from which the interrupt can be invoked
     * @param gate_type The type of the interrupt
     */
    static void register_interrupt(uint8_t interrupt_number,
                                   Isr handler_address, PriviledgeLevel dpl,
                                   GateType gate_type);

    /**
     * Enable the interrupt handler attached to the given interrupt number.
     *
     * @param interrupt_number The number used to activate the interrupt handler
     */
    static void enable(uint8_t interrupt_number);

    /**
     * Disable the interrupt handler attached to the given interrupt number.
     *
     * @param interrupt_number The number used to activate the interrupt handler
     */
    static void disable(uint8_t interrupt_number);

   private:
    // Maximum is 512, but we can actually
    constexpr static size_t INTERRUPT_NUMBER = 256;

    // The IDT itself
    static IdtDescriptor interrupts[INTERRUPT_NUMBER];
};
