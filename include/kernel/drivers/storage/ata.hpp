#pragma once

#include <stddef.h>
#include <stdint.h>

namespace drivers::storage::ata {

/**
 * Support operations on ATA drives.
 */

// ATA usually supports two separate buses that are controlled by a
// different set of registers. See
// https://wiki.osdev.org/ATA_PIO_Mode#Registers.
enum class Bus { PRIMARY, SECONDARY };

// Each ATA bus supports two ports. The first part of the LBA address
// specifies which port will be accessed.
enum class Port : uint8_t { MASTER = 0xE0, SLAVE = 0xF0 };

// ATA supports different modes for accessing disk data.
enum class Mode { PIO };

struct disk {
    Bus bus;
    Port port;
    Mode mode;
};

constexpr size_t SECTOR_SIZE_IN_BYTES = 512;
using sector = uint8_t[SECTOR_SIZE_IN_BYTES];

void read_sectors(disk* disk, sector* buffer, size_t offset, size_t amount);

namespace pio {
void read_sectors(disk* disk, sector* buffer, size_t offset, size_t amount);
}

}  // namespace drivers::storage::ata
