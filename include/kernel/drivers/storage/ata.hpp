#pragma once

#include <stddef.h>
#include <stdint.h>

#include <cstddef>

namespace drivers::storage {

/**
 * Support operations on ATA drives.
 */

class Ata final {
   public:
    // ATA usually supports two separate buses that are controlled by a
    // different set of registers. See
    // https://wiki.osdev.org/ATA_PIO_Mode#Registers.
    // TODO: support secondary port.
    enum class Bus { PRIMARY, /*SECONDARY*/ };

    // Each ATA bus supports two ports. The first part of the LBA address
    // specifies which port will be accessed.
    enum class Port : uint8_t { MASTER = 0xE0, SLAVE = 0xF0 };

    // ATA supports different modes for accessing disk data.
    enum class Mode { PIO };

    // Operations in ATA are done on sectors in a given size.
    constexpr static size_t SECTOR_SIZE_IN_BYTES = 512;

    /**
     * Ctor.
     *
     * @param bus The bus to communicate on.
     * @param port The port to communicate on.
     * @param mode How to talk to the ATA drive.
     */
    Ata(Bus bus = Bus::PRIMARY, Port port = Port::MASTER,
        Mode mode = Mode::PIO);

    /**
     * Read a given amount of sectors into memory.
     *
     * @param buffer The buffer to read data into.
     * @param offset The sector to start reading from.
     * @param amount The amount of sectors to read.
     *
     * TODO [32]: Replace with
     * `std::vector<byte> read(size_t offset, size_t amount)`
     */
    void read_sectors(std::byte* buffer, size_t offset, size_t amount);

   private:
    enum class Command { READ = 0x20 };

    void send_amount(size_t amount);
    void send_sector_offset(size_t offset);
    void send_command(Command command);
    void wait_for_buffer_to_be_ready();
    bool is_buffer_ready() const;
    void read_sector(std::byte* buffer);

    const Port port_;
};

}  // namespace drivers::storage
