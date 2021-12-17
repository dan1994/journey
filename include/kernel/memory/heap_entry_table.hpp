#pragma once

#include <stddef.h>

#include <cstddef>

#include "memory/heap_status.hpp"

/**
 * The heap entry table marks which blocks have been allocated and which are
 * free. Each entry table is relevant to a single heap.
 */

namespace memory {

class HeapEntryTable final {
   private:
    enum class Entry : unsigned char;

   public:
    /**
     * @param table_start The address at which the entry table begins.
     * @param total_entries The size of the heap in blocks.
     */
    explicit HeapEntryTable(std::byte *table_start, size_t total_entries);

    ~HeapEntryTable() = default;

    /**
     * Allocates a given amount of free contiguous blocks.
     *
     * @param entry_amount The amount of entries to allocate.
     * @param status [OUT] Whether the operation succeeded or failed and why.
     * @return The offset of the first block in the allocation.
     */
    size_t allocate(size_t entry_amount, HeapStatus &status);

    /**
     * Frees the blocks starting from a given offset.
     *
     * @param entry_offset The offset of the first block in the allocation.
     * @param status [OUT] Whether the operation succeeded or failed and why.
     */
    void free(size_t entry_offset, HeapStatus &status);

    static constexpr size_t entry_size_ = sizeof(Entry);

    HeapEntryTable(const HeapEntryTable &) = delete;
    HeapEntryTable(HeapEntryTable &&) = delete;
    HeapEntryTable &operator=(const HeapEntryTable &) = delete;
    HeapEntryTable &operator=(HeapEntryTable &&) = delete;

   private:
    /**
     * Meanings of different entry values.
     */
    enum class Entry : unsigned char {
        FREE = 0,
        USED = 1,
        FIRST = 3,
        LAST = 5,
    };

    /**
     * Finds a contiguous block of free entries.
     *
     * @param entry_amount The amount of entries to allocate.
     * @param status [OUT] Whether the operation succeeded or failed and why.
     * @return The offset of the first block in the allocation.
     */
    Entry *get_available_entries(size_t entry_amount, HeapStatus &status) const;

    /**
     * Marks entries as allocated. The first entry is marked as FIRST, the last
     * as LAST and the rest as USED.
     *
     * @param entry The first entry in the allocation.
     * @param entry_amount The amount of entries in the allocation.
     */
    void mark_entries_as_used(Entry *entry, size_t entry_amount);

    Entry *const table_start_;
    const size_t total_entries_;
};

}  // namespace memory