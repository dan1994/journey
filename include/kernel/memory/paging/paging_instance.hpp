#pragma once

#include "memory/paging/page_directory.hpp"
#include "memory/paging/page_table.hpp"

namespace memory::paging {

class PagingInstance final {
   public:
    explicit PagingInstance(
        const PageDirectoryEntry::Flags& page_directory_flags,
        const PageTableEntry::Flags& page_table_flags,
        PageTable::InitializationMode initialization_mode);

    ~PagingInstance();

    void load();

   private:
    static PageTable* initialize_page_tables(
        const PageTableEntry::Flags& flags,
        PageTable::InitializationMode initialization_mode);

    PageTable* tables_;
    PageDirectory directory_;
};

}  // namespace memory::paging
