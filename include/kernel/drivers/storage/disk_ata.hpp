#pragma once

#include "drivers/storage/ata.hpp"
#include "drivers/storage/disk.hpp"

namespace drivers::storage {

class DiskAta final : public Disk {
   public:
    /**
     * Ctor.
     *
     * @param bus The bus to communicate on.
     * @param port The port to communicate on.
     */
    explicit DiskAta(Ata::Bus bus = Ata::Bus::PRIMARY,
                     Ata::Port port = Ata::Port::MASTER);

    /**
     * Read a given amount of sectors into memory.
     *
     * @param buffer [OUT] The buffer to read data into.
     * @param offset The sector to start reading from.
     * @param amount The amount of sectors to read.
     */
    void read_sectors(std::byte* buffer, size_t offset, size_t amount);

    /**
     * Set the mode of interaction with the disk.
     *
     * @param mode One of the supported ATA modes.
     */
    void set_mode(Ata::Mode mode);

   private:
    const Ata::Bus bus_;
    const Ata::Port port_;
    Ata::Mode mode_;
};

}  // namespace drivers::storage
