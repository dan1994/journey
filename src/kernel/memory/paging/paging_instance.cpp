#include "memory/paging/paging_instance.hpp"

#include <new>

#include "memory/paging/paging.hpp"
#include "utilities/memory.hpp"

namespace memory::paging {
PagingInstance::PagingInstance(
    const PageDirectoryEntry::Flags& page_directory_flags,
    const PageTableEntry::Flags& page_table_flags,
    PageTable::InitializationMode initialization_mode)
    : tables_(utilities::uninitialized_array_of<PageTable>(
          PageDirectory::NUMBER_OF_ENTRIES)),
      directory_(page_directory_flags, tables_) {
    for (size_t i = 0; i < PageDirectory::NUMBER_OF_ENTRIES; i++) {
        const size_t page_table_offset =
            i * PageTable::NUMBER_OF_ENTRIES * PAGE_SIZE_IN_BYTES;

        new (tables_ + i)
            PageTable(page_table_flags, initialization_mode, page_table_offset);
    }
}

PagingInstance::~PagingInstance() {
    delete[] tables_;
}

void PagingInstance::load() {
    load_page_directory(directory_.entries());
}

}  // namespace memory::paging
