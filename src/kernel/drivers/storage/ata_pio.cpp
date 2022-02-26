#include "drivers/storage/ata_pio.hpp"

#include <type_traits>

#include "drivers/io/ports.hpp"
#include "drivers/storage/ata.hpp"

namespace drivers::storage {

namespace io = drivers::io;

AtaPio::AtaPio(Bus bus, Port port) : Ata(bus, port) {}

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
    io::write_byte(io::Port::PRIMARY_ATA_SELECTOR_COUNT, amount);
}

void AtaPio::send_sector_offset(size_t offset) {
    io::write_byte(io::Port::PRIMARY_ATA_SELECTOR_NUMBER_OR_LBA_LOW, offset);
    io::write_byte(io::Port::PRIMARY_ATA_CYLINDER_LOW_OR_LBA_MID, offset >> 8);
    io::write_byte(io::Port::PRIMARY_ATA_CYLINDER_HIGH_OR_LBA_HIGH,
                   offset >> 16);
}

void AtaPio::send_command(Command command) {
    io::write_byte(io::Port::PRIMARY_ATA_STATUS_OR_COMMAND,
                   std::underlying_type_t<Command>(command));
}

void AtaPio::wait_for_buffer_to_be_ready() {
    while (!is_buffer_ready()) {
    }
}

bool AtaPio::is_buffer_ready() const {
    constexpr uint8_t PIO_DATA_AVAILABLE = 0x08;

    const uint8_t status =
        io::read_byte(io::Port::PRIMARY_ATA_STATUS_OR_COMMAND);

    return status & PIO_DATA_AVAILABLE;
}

void AtaPio::read_sector(std::byte* buffer) {
    constexpr size_t SECTOR_SIZE_IN_WORDS =
        SECTOR_SIZE_IN_BYTES / (sizeof(uint16_t) / sizeof(uint8_t));

    uint16_t* buffer_as_words = reinterpret_cast<uint16_t*>(buffer);

    for (size_t words_read = 0; words_read < SECTOR_SIZE_IN_WORDS;
         words_read++) {
        buffer_as_words[words_read] = io::read_word(io::Port::PRIMARY_ATA_DATA);
    }
}

}  // namespace drivers::storage
