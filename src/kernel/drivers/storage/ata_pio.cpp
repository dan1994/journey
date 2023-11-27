#include <type_traits>

#include "drivers/io/ports.hpp"
#include "drivers/storage/ata.hpp"

namespace drivers::storage::ata::pio {

namespace io = drivers::io;

enum class Command { READ = 0x20 };

struct registers {
    io::Port data;
    io::Port features_or_error;
    io::Port selector_count;
    io::Port selector_number_or_lba_low;
    io::Port cylinder_low_or_lba_mid;
    io::Port cylinder_high_or_lba_high;
    io::Port drive_or_head;
    io::Port status_or_command;
    io::Port alternate_status_or_device_control;
    io::Port drive_address;
};

static registers primary_registers = {
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

static registers secondary_registers = {
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

static void send_amount(const registers& registers, size_t amount);
static void send_sector_offset(const registers& registers, Port port,
                               size_t offset);
static void send_command(const registers& registers, Command command);
static void wait_for_buffer_to_be_ready(const registers& registers);
[[nodiscard]] static bool is_buffer_ready(const registers& registers);
static void read_sector(const registers& registers, sector* buffer);
[[nodiscard]] static const registers& get_registers_by_bus(Bus bus);

void read_sectors(disk* disk, sector* buffer, size_t offset, size_t amount) {
    const registers& registers = get_registers_by_bus(disk->bus);
    send_amount(registers, amount);
    send_sector_offset(registers, disk->port, offset);
    send_command(registers, Command::READ);

    for (size_t sectors_read = 0; sectors_read < amount; sectors_read++) {
        wait_for_buffer_to_be_ready(registers);
        read_sector(registers, buffer + sectors_read);
    }
}

void send_amount(const registers& registers, size_t amount) {
    io::write_byte(registers.selector_count, amount);
}

void send_sector_offset(const registers& registers, Port port, size_t offset) {
    io::write_byte(registers.selector_number_or_lba_low, offset);
    io::write_byte(registers.cylinder_low_or_lba_mid, offset >> 8);
    io::write_byte(registers.cylinder_high_or_lba_high, offset >> 16);
    io::write_byte(registers.drive_or_head,
                   offset >> 24 | std::underlying_type_t<Port>(port));
}

void send_command(const registers& registers, Command command) {
    io::write_byte(registers.status_or_command,
                   std::underlying_type_t<Command>(command));
}

void wait_for_buffer_to_be_ready(const registers& registers) {
    while (!is_buffer_ready(registers)) {
    }
}

bool is_buffer_ready(const registers& registers) {
    constexpr uint8_t PIO_DATA_AVAILABLE = 0x08;

    const uint8_t status = io::read_byte(registers.status_or_command);

    return status & PIO_DATA_AVAILABLE;
}

void read_sector(const registers& registers, sector* buffer) {
    constexpr size_t SECTOR_SIZE_IN_WORDS =
        SECTOR_SIZE_IN_BYTES / (sizeof(uint16_t) / sizeof(uint8_t));

    uint16_t* buffer_as_words = reinterpret_cast<uint16_t*>(buffer);

    for (size_t words_read = 0; words_read < SECTOR_SIZE_IN_WORDS;
         words_read++) {
        buffer_as_words[words_read] = io::read_word(registers.data);
    }
}

const registers& get_registers_by_bus(Bus bus) {
    return (bus == Bus::PRIMARY) ? primary_registers : secondary_registers;
}

}  // namespace drivers::storage::ata::pio
