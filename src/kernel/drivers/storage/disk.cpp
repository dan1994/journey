#include "drivers/storage/disk.hpp"

#include "drivers/storage/disk_ata.hpp"

namespace drivers::storage {

void discover_disks(std::unique_ptr<Disk>* disks, size_t max_disk_amount) {
    if (max_disk_amount >= 1) {
        disks[0] =
            std::make_unique<DiskAta>(Ata::Bus::PRIMARY, Ata::Port::MASTER);
    }

    for (size_t i = 1; i < max_disk_amount; i++) {
        disks[i].reset(nullptr);
    }
}

}  // namespace drivers::storage
