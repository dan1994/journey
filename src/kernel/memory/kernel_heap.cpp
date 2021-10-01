#include "memory/kernel_heap.hpp"

memory::Heap& kernel_heap = get_kernel_heap();

memory::Heap& get_kernel_heap() {
    std::byte* base_address = reinterpret_cast<std::byte*>(0x100000);
    constexpr size_t MAX_SIZE = 100 * 1024 * 1024;
    constexpr size_t BLOCK_SIZE = 4 * 1024;

    static memory::Heap kernel_heap(base_address, MAX_SIZE, BLOCK_SIZE);

    return kernel_heap;
}
