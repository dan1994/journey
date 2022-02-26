#pragma once

#include <stddef.h>
#include <stdint.h>

#include <cstddef>
#include <memory>

namespace drivers::storage {

/**
 * Support operations on ATA drives.
 */

class Ata {
   public:
    // ATA usually supports two separate buses that are controlled by a
    // different set of registers. See
    // https://wiki.osdev.org/ATA_PIO_Mode#Registers.
    enum class Bus { PRIMARY, SECONDARY };

    // Each ATA bus supports two ports. The first part of the LBA address
    // specifies which port will be accessed.
    enum class Port : uint8_t { MASTER = 0xE0, SLAVE = 0xF0 };

    // ATA supports different modes for accessing disk data.
    enum class Mode { PIO };

    // Operations in ATA are done on sectors in a given size.
    constexpr static size_t SECTOR_SIZE_IN_BYTES = 512;

    /**
     * Ctor.
     */
    explicit Ata() = default;

    /**
     * Dtor.
     */
    virtual ~Ata() = default;

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

    // Deleted methods
    Ata(const Ata& other) = delete;
    Ata(Ata&& other) = delete;
    Ata& operator=(const Ata& other) = delete;
    Ata& operator=(Ata&& other) = delete;
};

std::unique_ptr<Ata> get_ata(Ata::Mode mode, Ata::Bus bus = Ata::Bus::PRIMARY,
                             Ata::Port port = Ata::Port::MASTER);

}  // namespace drivers::storage
