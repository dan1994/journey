#include "drivers/vga3.hpp"
#include "interrupts/interrupts.hpp"
#include "logging/logger.hpp"
#include "memory/paging/paging.hpp"
#include "memory/paging/paging_instance.hpp"

using namespace drivers;

extern "C" void main() {
    Vga3::clear();

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

    kernel_paging.load();
    enable_paging();
    Logger::debug("Initialized paging...");

    Logger::info("Kernel finished running. Going into infinite loop...");
}
