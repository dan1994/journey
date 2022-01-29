#include "memory/paging/paging_instance.hpp"

#include <cassert>
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

const PageDirectory& PagingInstance::get_directory() const {
    return directory_;
}

void PagingInstance::map(const void* virtual_address,
                         const void* physical_address, const Flags& flags) {
    PageDirectoryEntry& pde = get_page_directory_entry(virtual_address);
    if (!pde.is_present()) {
        assertm(false, "Can't map address because of non-present page table");
    }

    PageTableEntry& pte = get_page_table_entry(virtual_address);

    pte.set_page_address(reinterpret_cast<const std::byte*>(physical_address));

    if (flags.access_type == AccessType::READ_WRITE) {
        pte.enable_writing();
        pde.enable_writing();
    } else {
        pte.disable_writing();
    }

    if (flags.priviledge_level == PriviledgeLevel::USER) {
        pte.enable_user_access();
        pde.enable_user_access();
    } else {
        pte.disable_user_access();
    }

    pte.mark_present();
}

PageDirectoryEntry& PagingInstance::get_page_directory_entry(
    const void* virtual_address) {
    const size_t offset = reinterpret_cast<unsigned int>(virtual_address) /
                          (PageTable::NUMBER_OF_ENTRIES * PAGE_SIZE_IN_BYTES);

    return directory_[offset];
}

PageTableEntry& PagingInstance::get_page_table_entry(
    const void* virtual_address) {
    const PageDirectoryEntry& pde = get_page_directory_entry(virtual_address);

    const size_t offset = reinterpret_cast<unsigned int>(virtual_address) /
                          PAGE_SIZE_IN_BYTES % PageTable::NUMBER_OF_ENTRIES;

    PageTable page_table = pde.get_page_table();
    return page_table[offset];
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
