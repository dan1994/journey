#pragma once

#include "drivers/storage/ata.hpp"

namespace drivers::storage {

class Disk {
   public:
    /**
     * Ctor.
     */
    explicit Disk() = default;

    /**
     * Dtor.
     */
    virtual ~Disk() = default;

    /**
     * Read a given amount of sectors into memory.
     *
     * TODO: [32] Replace with
     * `std::vector<byte> read(size_t offset, size_t amount)`.
     *
     * @param buffer The buffer to read data into.
     * @param offset The sector to start reading from.
     * @param amount The amount of sectors to read.
     */
    virtual void read_sectors(std::byte* buffer, size_t offset,
                              size_t amount) = 0;

    // Deleted methods.
    Disk(const Disk&) = delete;
    Disk(Disk&&) = delete;
    Disk& operator=(const Disk&) = delete;
    Disk& operator=(Disk&&) = delete;
};

/**
 * Discover and populate a list of disks connected to the CPU.
 *
 * TODO: [32] Replace with `std::vector<std::unique_ptr<Disk>>
 * discover_disks()`.
 *
 * @param disks An array to populate.
 * @param max_disk_amount The maximum amount of disks to discover.
 */
void discover_disks(std::unique_ptr<Disk>* disks, size_t max_disk_amount);

}  // namespace drivers::storage
