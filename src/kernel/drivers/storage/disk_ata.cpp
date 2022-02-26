#include "drivers/storage/disk_ata.hpp"

namespace drivers::storage {

DiskAta::DiskAta(Ata::Bus bus, Ata::Port port)
    : bus_(bus), port_(port), mode_(Ata::Mode::PIO) {}

void DiskAta::read_sectors(std::byte* buffer, size_t offset, size_t amount) {
    std::unique_ptr<Ata> reader = get_ata(mode_, bus_, port_);
    reader->read_sectors(buffer, offset, amount);
}

void DiskAta::set_mode(Ata::Mode mode) {
    mode_ = mode;
}

}  // namespace drivers::storage
