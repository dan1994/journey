#include "memory/heap/heap.hpp"

namespace memory::heap {

Heap::Heap(std::byte *heap_start, size_t max_size, size_t block_size)
    : block_size_(block_size),
      memory_pool_(round_up_to_nearest_block_size(heap_start, block_size)),
      entry_table_(
          initialize_internal_entry_table(heap_start, max_size, block_size)) {}

Heap::Heap(std::byte *heap_start, std::byte *entry_table_start, size_t max_size,
           size_t block_size)
    : block_size_(block_size),
      memory_pool_(round_up_to_nearest_block_size(heap_start, block_size)),
      entry_table_(initialize_external_entry_table(
          heap_start, entry_table_start, max_size, block_size)) {}

WithError<void *> Heap::allocate(size_t bytes) {
    const size_t blocks_to_allocate = (bytes + block_size_ - 1) / block_size_;

    const auto [block_offset, error] =
        entry_table_.allocate(blocks_to_allocate);
    if (error) {
        return {nullptr, error};
    }

    return {memory_pool_ + (block_offset * block_size_), nullptr};
}

Error Heap::free(const void *address) {
    if (address < memory_pool_) {
        return WITH_LOCATION("Trying to free an address out of heap range.");
    }

    const size_t block_offset =
        (static_cast<const std::byte *>(address) - memory_pool_) / block_size_;

    return entry_table_.free(block_offset);
}

std::byte *Heap::round_up_to_nearest_block_size(std::byte *address,
                                                size_t block_size) {
    return reinterpret_cast<std::byte *>(
        ((reinterpret_cast<size_t>(address) + block_size - 1) / block_size) *
        block_size);
}

HeapEntryTable Heap::initialize_internal_entry_table(std::byte *heap_start,
                                                     size_t max_size,
                                                     size_t block_size) {
    std::byte *const memory_pool =
        round_up_to_nearest_block_size(heap_start, block_size);

    const size_t corrected_max_size = max_size - (memory_pool - heap_start);

    const size_t entry_amount =
        corrected_max_size / (block_size + HeapEntryTable::entry_size_);

    std::byte *const entry_table_start =
        memory_pool + entry_amount * block_size;

    return HeapEntryTable(entry_table_start, entry_amount);
}

HeapEntryTable Heap::initialize_external_entry_table(
    std::byte *heap_start, std::byte *entry_table_start, size_t max_size,
    size_t block_size) {
    std::byte *const memory_pool =
        round_up_to_nearest_block_size(heap_start, block_size);

    const size_t corrected_max_size = max_size - (memory_pool - heap_start);

    const size_t entry_amount = corrected_max_size / block_size;

    return HeapEntryTable(entry_table_start, entry_amount);
}

}  // namespace memory::heap
