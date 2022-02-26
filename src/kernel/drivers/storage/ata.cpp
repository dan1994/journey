#include "drivers/storage/ata.hpp"

#include <type_traits>

#include "drivers/io/ports.hpp"
#include "drivers/storage/ata_pio.hpp"

namespace drivers::storage {

namespace io = drivers::io;

Ata::Ata(Bus bus, Port port) : port_(port) {}

std::unique_ptr<Ata> get_ata(Ata::Mode mode, Ata::Bus bus, Ata::Port port) {
    switch (mode) {
        case Ata::Mode::PIO:
        default:
            return std::make_unique<AtaPio>(bus, port);
    }
}

}  // namespace drivers::storage
