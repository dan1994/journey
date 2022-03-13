#pragma once

#include "memory/paging/constants.hpp"
#include "memory/paging/page_directory.hpp"
#include "memory/paging/page_table.hpp"
#include "utilities/error.hpp"

namespace memory::paging {

class PagingInstance final {
   public:
    // Whether to initialize the table with addresses zeroed out or rising
    // linearly.
    enum class InitializationMode { ZEROED, LINEAR };

    struct Flags {
        PriviledgeLevel priviledge_level;
        AccessType access_type;
    };

    /**
     * Initialize a page directory and its page tables with the given
     * configurations.
     * @param directory_flags Flags to be passed to the page directory entries.
     * @param table_flags Flags to be passed to the page tables entries.
     * @param initialization_mode Whether to initialize with 1:1 mapping or have
     * all page tables point to page 0.
     */
    explicit PagingInstance(const directory::Flags& directory_flags,
                            const table::Flags& table_flags,
                            InitializationMode initialization_mode);

    ~PagingInstance();

    /**
     * Get the page directory.
     * @return a const reference to the page table.
     */
    const void* get_directory() const;

    /**
     * Map a virtual address to a physical address. Addresses will be aligned.
     * @param virtual_address The virtual address to map.
     * @param physical_address The physical address to map to.
     * @param flags Flags to set for the page.
     * @return Error if occured.
     */
    [[nodiscard]] Error map(const void* virtual_address,
                            const void* physical_address, const Flags& flags);

   private:
    [[nodiscard]] static size_t get_directory_offset(
        const void* virtual_address);
    [[nodiscard]] static size_t get_table_offset(const void* virtual_address);

    directory::Entry* directory_;
    table::Entry** tables_;
};

}  // namespace memory::paging
