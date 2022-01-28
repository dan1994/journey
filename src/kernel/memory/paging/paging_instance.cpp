#include "memory/paging/paging_instance.hpp"

#include <new>

#include "memory/paging/paging.hpp"
#include "utilities/memory.hpp"

namespace memory::paging {
PagingInstance::PagingInstance(
    const PageDirectoryEntry::Flags& page_directory_flags,
    const PageTableEntry::Flags& page_table_flags,
    PageTable::InitializationMode initialization_mode)
    : tables_(initialize_page_tables(page_table_flags, initialization_mode)),
      directory_(page_directory_flags, tables_) {}

PagingInstance::~PagingInstance() {
    delete[] tables_;
}

void PagingInstance::load() {
    load_page_directory(directory_.entries());
}

PageTable* PagingInstance::initialize_page_tables(
    const PageTableEntry::Flags& page_table_flags,
    PageTable::InitializationMode initialization_mode) {
    PageTable* tables = utilities::uninitialized_array_of<PageTable>(
        PageDirectory::NUMBER_OF_ENTRIES);

    for (size_t i = 0; i < PageDirectory::NUMBER_OF_ENTRIES; i++) {
        const size_t page_table_offset =
            i * PageTable::NUMBER_OF_ENTRIES * PAGE_SIZE_IN_BYTES;

        new (tables + i)
            PageTable(page_table_flags, initialization_mode, page_table_offset);
    }

    return tables;
}

}  // namespace memory::paging
