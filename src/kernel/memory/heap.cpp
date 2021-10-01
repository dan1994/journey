#include "memory/heap.hpp"

namespace memory {

Heap::Heap(std::byte *heap_start, size_t max_size, size_t block_size)
    : block_size(block_size),
      memory_pool(round_up_to_nearest_block_size(heap_start, block_size)),
      entry_table(initialize_entry_table(heap_start, max_size, block_size)) {}

void *Heap::allocate(size_t bytes) {
    const size_t blocks_to_allocate = (bytes + block_size - 1) / block_size;

    HeapStatus status;
    const size_t block_offset =
        entry_table.allocate(blocks_to_allocate, status);

    if (status != HeapStatus::SUCCESS) {
        return nullptr;
    }

    return memory_pool + (block_offset * block_size);
}

void Heap::free(const void *address) {
    const size_t block_offset =
        (static_cast<const std::byte *>(address) - memory_pool) / block_size;

    entry_table.free(block_offset);
}

HeapEntryTable Heap::initialize_entry_table(std::byte *heap_start,
                                            size_t max_size,
                                            size_t block_size) {
    std::byte *const memory_pool =
        round_up_to_nearest_block_size(heap_start, block_size);

    const size_t corrected_max_size = max_size - (memory_pool - heap_start);

    const size_t entry_amount =
        corrected_max_size / (block_size + HeapEntryTable::entry_size);

    std::byte *const entry_table_start =
        memory_pool + entry_amount * block_size;

    return HeapEntryTable(entry_table_start, entry_amount);
}

std::byte *Heap::round_up_to_nearest_block_size(std::byte *address,
                                                size_t block_size) {
    return reinterpret_cast<std::byte *>(
        ((reinterpret_cast<size_t>(address) + block_size - 1) / block_size) *
        block_size);
}

}  // namespace memory
