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

    constexpr size_t HEAP_BLOCK_SIZE = 4096;
    constexpr size_t HEAP_SIZE = 100 * 1024 * 1024;
    constexpr size_t HEAP_BLOCKS = HEAP_SIZE / HEAP_BLOCK_SIZE;
    memory::allocation::block_heap::block_heap heap_implementation =
        memory::allocation::block_heap::make_block_heap(
            reinterpret_cast<uint8_t *>(memory::Layout::KERNEL_HEAP),
            reinterpret_cast<memory::allocation::block_heap::block_metadata *>(
                memory::Layout::KERNEL_HEAP_ENTRY_TABLE),
            HEAP_BLOCK_SIZE, HEAP_BLOCKS);

    memory::allocation::allocator heap =
        memory::allocation::block_heap::make_allocator(&heap_implementation);

    auto [kernel, make_error] = make(&heap);
    errors::log(make_error);

    logging::info("Finalizing...");

    error destroy_error = destroy(&kernel);
    errors::log(destroy_error);

    logging::warn("Kernel finished running. Going into infinite loop...");
}
