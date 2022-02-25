#include "drivers/display/vga3.hpp"
#include "interrupts/interrupts.hpp"
#include "logging/logger.hpp"
#include "memory/paging/paging.hpp"

extern "C" void main() {
    drivers::display::Vga3::clear();

    Logger::info("Initializing Journey...");

    Interrupts::init();
    Logger::debug("Initialized interrupts...");

    memory::paging::PagingInstance kernel_paging(
        {
            memory::paging::PriviledgeLevel::KERNEL,
            memory::paging::AccessType::READ_WRITE,
            memory::paging::Present::TRUE,
        },
        {
            memory::paging::PriviledgeLevel::KERNEL,
            memory::paging::AccessType::READ_WRITE,
            memory::paging::Present::TRUE,
        },
        memory::paging::PageTable::InitializationMode::LINEAR);

    memory::paging::load(kernel_paging);
    memory::paging::enable_paging();
    Logger::debug("Initialized paging...");

    Logger::warn("Kernel finished running. Going into infinite loop...");
}
