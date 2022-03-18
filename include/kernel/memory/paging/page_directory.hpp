#pragma once

#include <stddef.h>
#include <stdint.h>

#include "memory/paging/page_table.hpp"
#include "memory/paging/shared.hpp"

namespace memory::paging::directory {

using PTE = memory::paging::table::Entry;

constexpr size_t ENTRY_NUM = 1024;
using Entry = uint32_t;

struct Flags {
    PriviledgeLevel priviledge_level;
    AccessType access_type;
    Present present;
};

/**
 * Create a new PDE with the given page table address and flags.
 *
 * @param pte The address of the PTE the entry should point to.
 * @param flags The PDE flags.
 * @return A new entry.
 */
[[nodiscard]] Entry make_entry(const PTE* table, const Flags& flags);

/**
 * Get the address of the page table pointed to by this entry.
 *
 * @param entry The PDE.
 * @return The page table address.
 */
[[nodiscard]] PTE* get_pte_address(Entry entry);

/**
 * Set the address of the page table pointed to by this entry.
 *
 * @param entry The PDE.
 * @param address The address of the page table.
 */
void set_pte_address(Entry* entry, const PTE* address);

/**
 * Check whether the page table was accessed.
 *
 * @param entry The PDE.
 * @return True iff the page table was accessed.
 */
[[nodiscard]] bool was_accessed(Entry entry);

/**
 * Reset the page table to seem it was not accessed.
 *
 * @param entry The PDE.
 */
void reset_accessed(Entry* entry);

/**
 * Check whether the page table can be accessed by the user.
 *
 * @param entry The PDE.
 * @return True iff the user can access the page table.
 */
[[nodiscard]] bool can_user_access(Entry entry);

/**
 * Allow usermode access to the page table.
 *
 * @param entry The PDE.
 */
void enable_user_access(Entry* entry);

/**
 * Prevent usermode access to the page table.
 *
 * @param entry The PDE.
 */
void disable_user_access(Entry* entry);

/**
 * Check if the page table is writeable.
 *
 * @param entry The PDE.
 * @return True iff the page table is writeable.
 */
[[nodiscard]] bool is_writeable(Entry entry);

/**
 * Make the page table writeable.
 *
 * @param entry The PDE.
 */
void enable_writing(Entry* entry);

/**
 * Make the page table read only.
 *
 * @param entry The PDE.
 */
void disable_writing(Entry* entry);

/**
 * Check whether the page table is present in memory.
 *
 * @param entry The PDE.
 * @return True iff the page table is present in memory.
 */
[[nodiscard]] bool is_present(Entry entry);

/**
 * Mark the page table as present in memory.
 *
 * @param entry The PDE.
 */
void mark_present(Entry* entry);

/**
 * Mark the page table as not present in memory.
 *
 * @param entry The PDE.
 */
void mark_not_present(Entry* entry);

}  // namespace memory::paging::directory
