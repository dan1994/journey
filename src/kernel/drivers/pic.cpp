#include "drivers/pic.hpp"

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

namespace drivers {

void Pic8259::init() {
    remap_master();
    remap_slave();
}

void Pic8259::remap_master() {
    constexpr uint8_t CASCADE_THROUGH_IRQ2 = 1 << 2;
    remap(Io::Port::MASTER_PIC_COMMAND, Io::Port::MASTER_PIC_DATA,
          MASTER_OFFSET, CASCADE_THROUGH_IRQ2);
}

void Pic8259::remap_slave() {
    constexpr uint8_t CASCADE_IDENTITY = 2;
    remap(Io::Port::SLAVE_PIC_COMMAND, Io::Port::SLAVE_PIC_DATA, SLAVE_OFFSET,
          CASCADE_IDENTITY);
}

void Pic8259::remap(Io::Port command_port, Io::Port data_port,
                    uint8_t idt_offset, uint8_t connection_information) {
    constexpr uint8_t INITIALIZATION_WORD = ICW1_INIT | ICW1_ICW4;
    constexpr uint8_t EXTRA_INFORMATION = ICW4_8086;

    const uint8_t saved_masks = Io::read_byte(data_port);

    Io::write_byte(command_port, INITIALIZATION_WORD);
    Io::write_byte(data_port, idt_offset);
    Io::write_byte(data_port, connection_information);
    Io::write_byte(data_port, EXTRA_INFORMATION);

    Io::write_byte(data_port, saved_masks);
}

void Pic8259::signal_end_of_interrupt(Interrupt interrupt) {
    constexpr uint8_t END_OF_INTERRUPT = 0x20;

    const Id controller = get_controller(interrupt);
    if (controller == Id::NONE) {
        return;
    }

    const Io::Port port = controller == Id::MASTER
                              ? Io::Port::MASTER_PIC_COMMAND
                              : Io::Port::SLAVE_PIC_COMMAND;

    Io::write_byte(port, END_OF_INTERRUPT);
}

Pic8259::Id Pic8259::get_controller(Interrupt interrupt) {
    constexpr uint8_t INTERRUPTS_PER_CONTROLLER = 8;

    const uint8_t interrupt_number = static_cast<uint8_t>(interrupt);

    if (interrupt_number >= MASTER_OFFSET &&
        interrupt_number < MASTER_OFFSET + INTERRUPTS_PER_CONTROLLER) {
        return Id::MASTER;
    }

    if (interrupt_number >= SLAVE_OFFSET &&
        interrupt_number < SLAVE_OFFSET + INTERRUPTS_PER_CONTROLLER) {
        return Id::SLAVE;
    }

    return Id::NONE;
}

}  // namespace drivers
