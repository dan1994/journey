#include "memory/kernel_heap.hpp"

#include "memory/layout.hpp"

memory::Heap* kernel_heap = &get_kernel_heap();

memory::Heap& get_kernel_heap() {
    std::byte* heap_address =
        reinterpret_cast<std::byte*>(memory::Layout::KERNEL_HEAP);
    std::byte* entry_table_address =
        reinterpret_cast<std::byte*>(memory::Layout::KERNEL_HEAP_ENTRY_TABLE);
    constexpr size_t MAX_SIZE = 100 * 1024 * 1024;
    constexpr size_t BLOCK_SIZE = 4 * 1024;

    static memory::Heap internal_kernel_heap(heap_address, entry_table_address,
                                             MAX_SIZE, BLOCK_SIZE);
    kernel_heap = &internal_kernel_heap;

    return internal_kernel_heap;
}
