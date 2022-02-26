#include "drivers/display/vga3.hpp"
#include "kernel/kernel.hpp"
#include "logging/logger.hpp"

extern "C" void main() {
    drivers::display::Vga3::clear();

    Logger::info("Initializing Journey...");

    [[maybe_unused]] Kernel& kernel = Kernel::get_kernel();

    Logger::warn("Kernel finished running. Going into infinite loop...");
}
