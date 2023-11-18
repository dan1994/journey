#include "drivers/storage/ata.hpp"

#include "drivers/io/ports.hpp"

namespace drivers::storage::ata {

void read_sectors(disk* disk, sector* buffer, size_t offset, size_t amount) {
    switch (disk->mode) {
        case Mode::PIO:
        default:
            pio::read_sectors(disk, buffer, offset, amount);
    }
}

}  // namespace drivers::storage::ata
