#include "memory/heap_entry_table.hpp"

#include <string.h>

#include <cassert>

#include "utilities/enum.hpp"

namespace memory {

HeapEntryTable::HeapEntryTable(std::byte *table_start, size_t total_entries)
    : table_start(reinterpret_cast<Entry *>(table_start)),
      total_entries(total_entries) {
    memset(table_start, 0, total_entries * sizeof(Entry));
}

size_t HeapEntryTable::allocate(size_t entry_amount, HeapStatus &status) {
    Entry *const entries = get_available_entries(entry_amount, status);
    if (status != HeapStatus::SUCCESS) {
        return 0;
    }

    mark_entries_as_used(entries, entry_amount);

    return entries - table_start;
}

void HeapEntryTable::free(size_t entry_offset, HeapStatus &status) {
    if (entry_offset > total_entries) {
        assertm(false, "Trying to free an address out of heap range.");
        status = HeapStatus::CANT_FREE_ADDRESS_OUT_OF_HEAP_RANGE;
        return;
    }

    Entry *entry = &table_start[entry_offset];

    if ((*entry & Entry::FIRST) != Entry::FIRST) {
        assertm(false,
                "First entry of allocation to free is not marked as first.");
        status = HeapStatus::CANT_FREE_FROM_THE_MIDDLE_OF_AN_ALLOCATION;
        return;
    }

    while ((*entry & Entry::LAST) != Entry::LAST) {
        if ((*entry & Entry::USED) != Entry::USED) {
            assertm(false,
                    "Encountered a free entry in the middle of an allocation.");
            status = HeapStatus::FREE_ENTRY_IN_THE_MIDDLE_OF_AN_ALLOCATION;
        }
        *entry = Entry::FREE;
        entry++;
    }

    *entry = Entry::FREE;
}

HeapEntryTable::Entry *HeapEntryTable::get_available_entries(
    size_t entry_amount, HeapStatus &status) const {
    status = HeapStatus::SUCCESS;

    if (entry_amount == 0) {
        status = HeapStatus::CANT_ALLOCATE_ZERO_ENTRIES;
        return nullptr;
    }

    size_t contiguous_free_entries = 0;

    for (Entry *entry = table_start; entry < table_start + total_entries;
         entry++) {
        if (*entry == Entry::FREE) {
            contiguous_free_entries++;
        } else {
            contiguous_free_entries = 0;
        }

        if (contiguous_free_entries == entry_amount) {
            return entry - (entry_amount - 1);
        }
    }

    assertm(false, "Out of memory.");
    status = HeapStatus::NOT_ENOUGH_CONTIGUOUS_MEMORY;
    return nullptr;
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

}  // namespace memory
