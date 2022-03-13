#include "memory/heap/heap_entry_table.hpp"

#include <cstring>

#include "utilities/enum.hpp"

namespace memory::heap {

HeapEntryTable::HeapEntryTable(std::byte *table_start, size_t total_entries)
    : table_start_(reinterpret_cast<Entry *>(table_start)),
      total_entries_(total_entries) {
    std::memset(table_start_, 0, total_entries_ * sizeof(Entry));
}

WithError<size_t> HeapEntryTable::allocate(size_t entry_amount) {
    const auto [entries, error] = get_available_entries(entry_amount);
    if (error) {
        return {0, error};
    }

    mark_entries_as_used(entries, entry_amount);

    const size_t offset = entries - table_start_;

    return {offset, nullptr};
}

Error HeapEntryTable::free(size_t entry_offset) {
    if (entry_offset > total_entries_) {
        return WITH_LOCATION("Trying to free an address out of heap range.");
    }

    Entry *entry = &table_start_[entry_offset];

    if ((*entry & Entry::FIRST) != Entry::FIRST) {
        return WITH_LOCATION(
            "First entry of allocation to free is not marked as first.");
    }

    while ((*entry & Entry::LAST) != Entry::LAST) {
        if ((*entry & Entry::USED) != Entry::USED) {
            return WITH_LOCATION(
                "Encountered a free entry in the middle of an allocation.");
        }
        *entry = Entry::FREE;
        entry++;
    }

    *entry = Entry::FREE;

    return nullptr;
}

WithError<HeapEntryTable::Entry *> HeapEntryTable::get_available_entries(
    size_t entry_amount) const {
    if (entry_amount == 0) {
        return {nullptr, WITH_LOCATION("Can't allocate zero entries")};
    }

    size_t contiguous_free_entries = 0;

    for (Entry *entry = table_start_; entry < table_start_ + total_entries_;
         entry++) {
        if (*entry == Entry::FREE) {
            contiguous_free_entries++;
        } else {
            contiguous_free_entries = 0;
        }

        if (contiguous_free_entries == entry_amount) {
            return {entry - (entry_amount - 1), nullptr};
        }
    }

    return {nullptr, WITH_LOCATION("Out of memory")};
}

void HeapEntryTable::mark_entries_as_used(Entry *entries, size_t entry_amount) {
    if (entry_amount == 1) {
        *entries = Entry::FIRST | Entry::LAST;
        return;
    }

    for (size_t i = 0; i < entry_amount; i++) {
        entries[i] = i == 0                  ? Entry::FIRST
                     : i == entry_amount - 1 ? Entry::LAST
                                             : Entry::USED;
    }
}

}  // namespace memory::heap
