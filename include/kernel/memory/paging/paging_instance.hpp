#pragma once

#include "memory/paging/page_directory.hpp"
#include "memory/paging/page_table.hpp"

namespace memory::paging {

class PagingInstance final {
   public:
    struct Flags {
        PriviledgeLevel priviledge_level;
        AccessType access_type;
    };

    /**
     * Initialize a page directory and its page tables with the given
     * configurations.
     * @param page_directory_flags Flags to be passed to the page directory
     * entries.
     * @param page_table_flags Flags to be passed to the page tables entries.
     * @param initialization_mode Whether to initialize with 1:1 mapping or have
     * all page tables point to page 0.
     */
    explicit PagingInstance(
        const PageDirectoryEntry::Flags& page_directory_flags,
        const PageTableEntry::Flags& page_table_flags,
        PageTable::InitializationMode initialization_mode);

    ~PagingInstance();

    /**
     * Get the page directory.
     * @return a const reference to the page table.
     */
    const PageDirectory& get_directory() const;

    /**
     * Map a virtual address to a physical address. Addresses will be aligned.
     * @param virtual_address The virtual address to map.
     * @param physical_address The physical address to map to.
     * @param flags Flags to set for the page.
     */
    [[nodiscard]] void map(const void* virtual_address,
                           const void* physical_address, const Flags& flags);

   private:
    [[nodiscard]] PageDirectoryEntry& get_page_directory_entry(
        const void* virtual_address);
    [[nodiscard]] PageTableEntry& get_page_table_entry(
        const void* virtual_address);

    [[nodiscard]] static PageTable* initialize_page_tables(
        const PageTableEntry::Flags& flags,
        PageTable::InitializationMode initialization_mode);

    PageTable* const tables_;
    PageDirectory directory_;
};

}  // namespace memory::paging
