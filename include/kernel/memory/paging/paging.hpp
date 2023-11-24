#pragma once

#include "memory/allocation/allocator.hpp"
#include "memory/paging/page_directory.hpp"
#include "memory/paging/page_table.hpp"
#include "memory/paging/shared.hpp"
#include "utilities/error.hpp"

namespace memory::paging {

struct flags {
    PriviledgeLevel priviledge_level;
    AccessType access_type;
};

struct paging {
    allocator* allocator_;
    directory::Entry* directory;
    table::Entry** tables;
};

with_error<paging> make(allocator* allocator,
                        const directory::Flags& directory_flags,
                        const table::Flags& table_flags);

error destroy(paging* paging);

[[nodiscard]] error map(paging* paging, const void* virtual_address,
                        const void* physical_address, const flags& flags);

/**
 * Enable paging in the processor.
 * WARNING: A page directory must be loaded before enabling paging. Otherwise
 * the system will panic.
 */
void enable();

void disable();

/**
 * Make the paging mechanism use the given paging instance.
 * @param instance The paging instance to use.
 */
void load(const paging instance);

}  // namespace memory::paging
