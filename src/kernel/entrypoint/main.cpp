#include "drivers/display/vga3.hpp"
#include "kernel/kernel.hpp"
#include "logging/logger.hpp"
#include "memory/allocation/allocator.hpp"
#include "memory/allocation/block_heap.hpp"
#include "memory/layout.hpp"

extern "C" void main() {
    drivers::display::vga3::clear();
    logging::set_level(logging::Level::DEBUG);

    logging::info("Initializing Journey...");

    kernel kernel = make();

    logging::warn("Kernel finished running. Going into infinite loop...");
}
