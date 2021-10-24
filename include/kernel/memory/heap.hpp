#pragma once

#include <stddef.h>

#include <cstddef>

#include "memory/heap_entry_table.hpp"

/**
 * A block based heap implementation. The heap is broken into contiguous blocks,
 * and allocation can only be done on entire blocks.
 */

namespace memory {

class Heap final {
   public:
    /**
     * Initializes a heap with its entry table in a certain memory region.
     *
     * @param heap_start The start address of the heap.
     * @param max_size The size allocated to both the heap and entry table.
     * @param block_size The size of blocks in the heap.
     */
    explicit Heap(std::byte *heap_start, size_t max_size, size_t block_size);

    /**
     * Initializes a heap in a certain memory region, with the entry table at a
     * different region.
     *
     * @param heap_start The start address of the heap.
     * @param entry_table_start The start address of the entry table.
     * @param max_size The size allocated to both the heap and entry table.
     * @param block_size The size of blocks in the heap.
     */
    explicit Heap(std::byte *heap_start, std::byte *entry_table_start,
                  size_t max_size, size_t block_size);

    ~Heap() = default;

    /**
     * Allocates enough blocks to accomodate the requested number of bytes.
     *
     * @param bytes The amount of bytes to allocate.
     * @param status [OUT] Whether the operation succeeded or failed and why.
     * @return The starting address of the allocation.
     */
    void *allocate(size_t bytes, HeapStatus &status);

    /**
     * Frees an allocation at a given address.
     *
     * @param address The address of the start of the allocation.
     * @param status [OUT] Whether the operation succeeded or failed and why.
     */
    void free(const void *address, HeapStatus &status);

    Heap(const Heap &) = delete;
    Heap(Heap &&) = delete;
    Heap &operator=(const Heap &) = delete;
    Heap &operator=(Heap &&) = delete;

   private:
    /**
     * Rounds an address up to a multiple of block size.
     *
     * @param address The input address.
     * @param block_size The size of a block.
     * @return The rounded address.
     */
    static std::byte *round_up_to_nearest_block_size(std::byte *address,
                                                     size_t block_size);

    /**
     * Initialize an entry table that occupies the same memory region as the
     * heap blocks.
     *
     * @param heap_start The start address of the heap.
     * @param max_size The size allocated to both the heap and entry table.
     * @param block_size The size of blocks in the heap.
     * @return The entry table.
     */
    static HeapEntryTable initialize_internal_entry_table(std::byte *heap_start,
                                                          size_t max_size,
                                                          size_t block_size);

    /**
     * Initialize an entry table that occupies a different memory region from
     * the heap blocks.
     *
     * @param heap_start The start address of the heap.
     * @param entry_table_start The start address of the entry table.
     * @param max_size The size allocated to both the heap and entry table.
     * @param block_size The size of blocks in the heap.
     * @return The entry table.
     */
    static HeapEntryTable initialize_external_entry_table(
        std::byte *heap_start, std::byte *entry_table_start, size_t max_size,
        size_t block_size);

    const size_t block_size_;
    std::byte *const memory_pool_;
    HeapEntryTable entry_table_;
};

}  // namespace memory
