#pragma once

#include <stddef.h>

#include <cstddef>

#include "memory/heap_entry_table.hpp"

namespace memory {

class Heap final {
   public:
    explicit Heap(std::byte *heap_start, size_t max_size, size_t block_size);
    ~Heap() = default;

    void *allocate(size_t bytes);
    void free(const void *address);

    Heap(const Heap &) = delete;
    Heap(Heap &&) = delete;
    Heap &operator=(const Heap &) = delete;
    Heap &operator=(Heap &&) = delete;

   private:
    const size_t block_size;
    std::byte *const memory_pool;
    HeapEntryTable entry_table;

    static HeapEntryTable initialize_entry_table(std::byte *heap_start,
                                                 size_t max_size,
                                                 size_t block_size);

    static std::byte *round_up_to_nearest_block_size(std::byte *address,
                                                     size_t block_size);
};

}  // namespace memory
