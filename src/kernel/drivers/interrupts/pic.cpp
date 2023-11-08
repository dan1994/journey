#include "drivers/interrupts/pic.hpp"

#include "drivers/io/ports.hpp"

namespace drivers::interrupts::pic8259 {

enum class Id { MASTER, SLAVE, NONE };

static void remap_master();
static void remap_slave();
static void remap(drivers::io::Port command_port, drivers::io::Port data_port,
                  uint8_t idt_offest, uint8_t connection_information);
[[nodiscard]] static Id get_controller(::interrupts::Id interrupt);

void init() {
    remap_master();
    remap_slave();
}

void remap_master() {
    constexpr uint8_t CASCADE_THROUGH_IRQ2 = 1 << 2;
    remap(io::Port::MASTER_PIC_COMMAND, io::Port::MASTER_PIC_DATA,
          MASTER_OFFSET, CASCADE_THROUGH_IRQ2);
}

void remap_slave() {
    constexpr uint8_t CASCADE_IDENTITY = 2;
    remap(io::Port::SLAVE_PIC_COMMAND, io::Port::SLAVE_PIC_DATA, SLAVE_OFFSET,
          CASCADE_IDENTITY);
}

void remap(io::Port command_port, io::Port data_port, uint8_t idt_offset,
           uint8_t connection_information) {
    // See https://wiki.osdev.org/PIC for more info

    constexpr uint8_t WITH_FOURTH_INITIALIZATION_WORD = 0x1;
    constexpr uint8_t INITIALIZATION_BIT = 0x10;
    constexpr uint8_t INITIALIZATION_WORD_1 =
        INITIALIZATION_BIT | WITH_FOURTH_INITIALIZATION_WORD;

    constexpr uint8_t MODE_8086 = 0x1;
    constexpr uint8_t INITIALIZATION_WORD_4 = MODE_8086;

    const uint8_t saved_masks = io::read_byte(data_port);

    io::write_byte(command_port, INITIALIZATION_WORD_1);
    io::short_delay();
    io::write_byte(data_port, idt_offset);
    io::short_delay();
    io::write_byte(data_port, connection_information);
    io::short_delay();
    io::write_byte(data_port, INITIALIZATION_WORD_4);
    io::short_delay();

    io::write_byte(data_port, saved_masks);
}

void signal_end_of_interrupt(::interrupts::Id interrupt) {
    constexpr uint8_t END_OF_INTERRUPT = 0x20;

    const Id controller = get_controller(interrupt);
    if (controller == Id::NONE) {
        return;
    }

    const io::Port port = controller == Id::MASTER
                              ? io::Port::MASTER_PIC_COMMAND
                              : io::Port::SLAVE_PIC_COMMAND;

    io::write_byte(port, END_OF_INTERRUPT);
    if (controller == Id::SLAVE) {
        io::write_byte(io::Port::MASTER_PIC_COMMAND, END_OF_INTERRUPT);
    }
}

Id get_controller(::interrupts::Id interrupt) {
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

}  // namespace drivers::interrupts::pic8259
