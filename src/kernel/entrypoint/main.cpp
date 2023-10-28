#include "drivers/display/vga3.hpp"
#include "kernel/kernel.hpp"
#include "logging/logger.hpp"

extern "C" void main() {
    drivers::display::vga3::clear();
    logging::set_level(logging::Level::DEBUG);

    logging::info("Initializing Journey...");

    [[maybe_unused]] Kernel& kernel = Kernel::get_kernel();

    logging::warn("Kernel finished running. Going into infinite loop...");
}
