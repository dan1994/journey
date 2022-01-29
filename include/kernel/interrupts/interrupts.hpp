#pragma once

/**
 * Implementation of the Interrupt Descriptor Table (IDT), that allows setting
 * up interrupt handlers to be called from user space or the kernel.
 * See https://wiki.osdev.org/IDT for more info.
 */

#include "drivers/interrupts/pic.hpp"
#include "interrupts/idt.hpp"

// The number used with the int instruction
enum class Interrupt : uint8_t {
    DIVIDE_BY_ZERO,
    PIC_TIMER = drivers::interrupts::Pic8259::MASTER_OFFSET,
    PIC_KEYBOARD,
    PIC_CASCADE,
    PIC_COM2,
    PIC_COM1,
    PIC_LPT2,
    PIC_FLOPPY,
    PIC_LPT1,
    PIC_CMOS_RTC = drivers::interrupts::Pic8259::SLAVE_OFFSET,
    PIC_CGA,
    PIC_UNUSED_0,
    PIC_UNUSED_1,
    PIC_PS2,
    PIC_FPU,
    PIC_HDD,
    PIC_UNUSED_2,
};

class Interrupts final {
   public:
    Interrupts() = delete;

    /**
     * Initializes the IDT register, and registers all interrupts.
     */
    static void init();

    /**
     * Enable all interrupts.
     */
    static void enable();

    /**
     * Disable all interrupts.
     */
    static void disable();

    /**
     * Enable the entry that corresponds to the interrupt.
     *
     * @param interrupt The number used to activate the isr.
     */
    static void enable(Interrupt interrupt);

    /**
     * Disable the entry that corresponds to the interrupt.
     *
     * @param interrupt The number used to activate the isr.
     */
    static void disable(Interrupt interrupt);

   private:
    // ISR = Interrupt Service Routine
    using Isr = void (*)(void *);

    // The ring from which an interrupt can be invoked
    enum class PriviledgeLevel : uint8_t { KERNEL, RING_1, RING_2, USER };

    // Whether the ISR is written in 16 or 32 bits
    enum class GateSize : uint8_t { BITS16, BITS32 };

    // Whether this is an task, interrupt or trap
    enum class GateType : uint8_t {
        TASK = 0x5,
        INTERRUPT,
        TRAP,
    };

    /**
     * Registers all tasks, interrupts and traps.
     */
    static void register_all();

    /**
     * Register a task on the given interrupt number.
     *
     * @param interrupt The number used to activate the task
     * @param dpl The ring from which the task can be invoked
     */
    static void register_task(Interrupt interrupt, PriviledgeLevel dpl);

    /**
     * Registers an interrupt on the given interrupt number.
     *
     * @param interrupt The number used to activate the interrupt
     * @param isr The routine that services this interrupt
     * @param dpl The ring from which the interrupt can be invoked
     * @param size Can be 16 or 32 bits
     */
    static void register_interrupt(Interrupt interrupt, Isr isr,
                                   PriviledgeLevel dpl, GateSize size);

    /**
     * Registers a trap on the given interrupt number.
     *
     * @param interrupt The number used to activate the trap
     * @param isr The routine that services this trap
     * @param dpl The ring from which the trap can be invoked
     * @param size Can be 16 or 32 bits
     */
    static void register_trap(Interrupt interrupt, Isr isr, PriviledgeLevel dpl,
                              GateSize size);

    /**
     * Registers a task, interrupt or trap on the given interrupt number.
     *
     * @param interrupt The number used to activate the interrupt handler
     * @param isr The address of the handler routine
     * @param dpl The ring from which the interrupt can be invoked
     * @param type The type of the interrupt
     */
    static void register_internal(Interrupt interrupt, Isr isr,
                                  PriviledgeLevel dpl, GateType type,
                                  GateSize size);

    // Maximum is 512, but since the int command can only get an 8 bit operand,
    // we can never invoke more than 256 interrupts.
    constexpr static size_t INTERRUPT_NUMBER = 256;

    // The IDT itself
    static IdtDescriptor interrupts_[INTERRUPT_NUMBER];
};
