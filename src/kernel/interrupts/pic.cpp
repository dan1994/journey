#include "interrupts/pic.hpp"

// These defines are from https://wiki.osdev.org/PIC
#define ICW1_ICW4 0x01      /* ICW4 (not) needed */
#define ICW1_SINGLE 0x02    /* Single (cascade) mode */
#define ICW1_INTERVAL4 0x04 /* Call address interval 4 (8) */
#define ICW1_LEVEL 0x08     /* Level triggered (edge) mode */
#define ICW1_INIT 0x10      /* Initialization - required! */

#define ICW4_8086 0x01       /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO 0x02       /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE 0x08  /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C /* Buffered mode/master */
#define ICW4_SFNM 0x10       /* Special fully nested (not) */

void ProgrammableInterruptController::remap(Id controller, uint8_t idt_offset) {
    const Io::Port command_port = get_command_port(controller);
    const Io::Port data_port = get_data_port(controller);

    Io::write_byte(command_port, ICW1_INIT | ICW1_ICW4);
    Io::write_byte(data_port, idt_offset);
    Io::write_byte(data_port, ICW4_8086);

    ProgrammableInterruptController::idt_offset = idt_offset;
}

void ProgrammableInterruptController::signal_end_of_interrupt(
    Interrupts::Id interrupt) {
    constexpr uint8_t END_OF_INTERRUPT = 0x20;

    const Io::Port port = get_command_port(get_controller(interrupt));

    Io::write_byte(port, END_OF_INTERRUPT);
}

Io::Port ProgrammableInterruptController::get_command_port(Id controller) {
    return controller == Id::MASTER ? Io::Port::MASTER_PIC_COMMAND
                                    : Io::Port::SLAVE_PIC_COMMAND;
}

Io::Port ProgrammableInterruptController::get_data_port(Id controller) {
    return controller == Id::MASTER ? Io::Port::MASTER_PIC_DATA
                                    : Io::Port::SLAVE_PIC_DATA;
}

ProgrammableInterruptController::Id
ProgrammableInterruptController::get_controller(Interrupts::Id interrupt) {
    constexpr uint8_t LAST_MASTER_PIC_INTERRUPT = 0x8;

    return static_cast<uint8_t>(interrupt) - idt_offset <=
                   LAST_MASTER_PIC_INTERRUPT
               ? Id::MASTER
               : Id::SLAVE;
}

uint8_t ProgrammableInterruptController::idt_offset = 0;
