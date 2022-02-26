#include "drivers/storage/ata_pio.hpp"

#include <type_traits>

#include "drivers/io/ports.hpp"
#include "drivers/storage/ata.hpp"

namespace drivers::storage {

namespace io = drivers::io;

AtaPio::AtaPio(Bus bus, Port port)
    : Ata(bus, port), registers_(get_registers_by_bus(bus)) {}

void AtaPio::read_sectors(std::byte* buffer, size_t offset, size_t amount) {
    send_amount(amount);
    send_sector_offset(offset);
    send_command(Command::READ);

    for (size_t sectors_read = 0; sectors_read < amount; sectors_read++) {
        wait_for_buffer_to_be_ready();
        read_sector(buffer + (sectors_read * SECTOR_SIZE_IN_BYTES));
    }
}

void AtaPio::send_amount(size_t amount) {
    io::write_byte(registers_.selector_count, amount);
}

void AtaPio::send_sector_offset(size_t offset) {
    io::write_byte(registers_.selector_number_or_lba_low, offset);
    io::write_byte(registers_.cylinder_low_or_lba_mid, offset >> 8);
    io::write_byte(registers_.cylinder_high_or_lba_high, offset >> 16);
    io::write_byte(registers_.drive_or_head,
                   offset >> 24 | std::underlying_type_t<Port>(port_));
}

void AtaPio::send_command(Command command) {
    io::write_byte(registers_.status_or_command,
                   std::underlying_type_t<Command>(command));
}

void AtaPio::wait_for_buffer_to_be_ready() {
    while (!is_buffer_ready()) {
    }
}

bool AtaPio::is_buffer_ready() const {
    constexpr uint8_t PIO_DATA_AVAILABLE = 0x08;

    const uint8_t status = io::read_byte(registers_.status_or_command);

    return status & PIO_DATA_AVAILABLE;
}

void AtaPio::read_sector(std::byte* buffer) {
    constexpr size_t SECTOR_SIZE_IN_WORDS =
        SECTOR_SIZE_IN_BYTES / (sizeof(uint16_t) / sizeof(uint8_t));

    uint16_t* buffer_as_words = reinterpret_cast<uint16_t*>(buffer);

    for (size_t words_read = 0; words_read < SECTOR_SIZE_IN_WORDS;
         words_read++) {
        buffer_as_words[words_read] = io::read_word(registers_.data);
    }
}

AtaPio::Registers AtaPio::get_registers_by_bus(Bus bus) {
    if (bus == Bus::PRIMARY) {
        return {
            io::Port::PRIMARY_ATA_DATA,
            io::Port::PRIMARY_ATA_FEATURES_OR_ERROR,
            io::Port::PRIMARY_ATA_SELECTOR_COUNT,
            io::Port::PRIMARY_ATA_SELECTOR_NUMBER_OR_LBA_LOW,
            io::Port::PRIMARY_ATA_CYLINDER_LOW_OR_LBA_MID,
            io::Port::PRIMARY_ATA_CYLINDER_HIGH_OR_LBA_HIGH,
            io::Port::PRIMARY_ATA_DRIVE_OR_HEAD,
            io::Port::PRIMARY_ATA_STATUS_OR_COMMAND,
            io::Port::PRIMARY_ATA_ALTERNATE_STATUS_OR_DEVICE_CONTROL,
            io::Port::PRIMARY_ATA_DRIVE_ADDRESS,
        };
    }

    return {
        io::Port::SECONDARY_ATA_DATA,
        io::Port::SECONDARY_ATA_FEATURES_OR_ERROR,
        io::Port::SECONDARY_ATA_SELECTOR_COUNT,
        io::Port::SECONDARY_ATA_SELECTOR_NUMBER_OR_LBA_LOW,
        io::Port::SECONDARY_ATA_CYLINDER_LOW_OR_LBA_MID,
        io::Port::SECONDARY_ATA_CYLINDER_HIGH_OR_LBA_HIGH,
        io::Port::SECONDARY_ATA_DRIVE_OR_HEAD,
        io::Port::SECONDARY_ATA_STATUS_OR_COMMAND,
        io::Port::SECONDARY_ATA_ALTERNATE_STATUS_OR_DEVICE_CONTROL,
        io::Port::SECONDARY_ATA_DRIVE_ADDRESS,
    };
}

}  // namespace drivers::storage
