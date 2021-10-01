#pragma once

#include <stddef.h>

#include <cstddef>

#include "memory/heap_status.hpp"

namespace memory {

class HeapEntryTable final {
   private:
    enum class Entry : unsigned char;

   public:
    explicit HeapEntryTable(std::byte *table_start, size_t total_entries);
    ~HeapEntryTable() = default;

    size_t allocate(size_t entry_amount, HeapStatus &status);
    void free(size_t entry_offset);

    size_t size() const;

    static constexpr size_t entry_size = sizeof(Entry);

    HeapEntryTable(const HeapEntryTable &) = delete;
    HeapEntryTable(HeapEntryTable &&) = delete;
    HeapEntryTable &operator=(const HeapEntryTable &) = delete;
    HeapEntryTable &operator=(HeapEntryTable &&) = delete;

   private:
    enum class Entry : unsigned char {
        FREE = 0,
        USED = 1,
        FIRST = 3,
        LAST = 5,
        FIRST_AND_LAST = 7,
    };

    Entry *get_available_entries(size_t entry_amount, HeapStatus &status) const;
    void mark_entries_as_used(Entry *entry, size_t entry_amount);

    Entry *const table_start;
    const size_t total_entries;
};

}  // namespace memory