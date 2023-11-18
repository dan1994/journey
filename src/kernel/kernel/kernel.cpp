#include "kernel/kernel.hpp"

#include <utility>

#include "interrupts/idt.hpp"
#include "logging/logger.hpp"
#include "memory/allocation/allocator.hpp"
#include "memory/layout.hpp"

memory::paging::Paging initialize_kernel_paging(allocator *allocator);

kernel make() {
    constexpr size_t HEAP_BLOCK_SIZE = 4096;
    constexpr size_t HEAP_SIZE = 100 * 1024 * 1024;
    constexpr size_t HEAP_BLOCKS = HEAP_SIZE / HEAP_BLOCK_SIZE;
    memory::allocation::block_heap::block_heap heap_implementation =
        memory::allocation::block_heap::make_block_heap(
            reinterpret_cast<uint8_t *>(memory::Layout::KERNEL_HEAP),
            reinterpret_cast<memory::allocation::block_heap::block_metadata *>(
                memory::Layout::KERNEL_HEAP_ENTRY_TABLE),
            HEAP_BLOCK_SIZE, HEAP_BLOCKS);

    allocator heap =
        memory::allocation::block_heap::make_allocator(&heap_implementation);

    drivers::storage::ata::disk boot_disk{
        .bus = drivers::storage::ata::Bus::PRIMARY,
        .port = drivers::storage::ata::Port::MASTER,
        .mode = drivers::storage::ata::Mode::PIO};

    interrupts::init();
    logging::debug("Initialized interrupts...");

    memory::paging::Paging kernel_paging = initialize_kernel_paging(&heap);

    memory::paging::load(kernel_paging);
    memory::paging::enable_paging();
    logging::debug("Initialized paging...");

    return kernel{
        .heap_implementation = heap_implementation,
        .heap = heap,
        .boot_disk = boot_disk,
        .kernel_paging = std::move(kernel_paging),
    };
}

memory::paging::Paging initialize_kernel_paging(allocator *allocator) {
    WithError<memory::paging::Paging> paging_err = memory::paging::Paging::make(
        allocator,
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
        memory::paging::Paging::InitializationMode::LINEAR);

    return std::move(paging_err.first);
}
