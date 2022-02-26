#pragma once

#include "drivers/storage/ata.hpp"

namespace drivers::storage {

/**
 * Support operations on ATA drives.
 */

class AtaPio final : public Ata {
   public:
    /**
     * Ctor.
     *
     * @param bus The bus to communicate on.
     * @param port The port to communicate on.
     */
    AtaPio(Bus bus = Bus::PRIMARY, Port port = Port::MASTER);

    /**
     * Read a given amount of sectors into memory.
     *
     * @param buffer The buffer to read data into.
     * @param offset The sector to start reading from.
     * @param amount The amount of sectors to read.
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
};

}  // namespace drivers::storage
