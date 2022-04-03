#pragma once

#include <stddef.h>
#include <stdint.h>

#include "memory/paging/shared.hpp"

namespace memory::paging::table {

constexpr size_t ENTRY_NUM = 1024;
using Entry = uint32_t;

struct Flags {
    PriviledgeLevel priviledge_level;
    AccessType access_type;
    Present present;
};

/**
 * Create a new PTE with the given logical address and flags.
 *
 * @param page_address The logical address the entry should point to.
 * @param flags The PTE flags.
 * @return A new entry.
 */
[[nodiscard]] Entry make_entry(const void* page_address, const Flags& flags);

/**
 * Get the logical address of the page pointed to by this entry.
 *
 * @param entry The PTE.
 * @return The logical address of the page.
 */
[[nodiscard]] const void* get_page_address(Entry entry);

/**
 * Set the logical address of the page pointed to by this entry.
 *
 * @param entry The PTE.
 * @param address The logical address of the page. The address is rounded down
 * to a multiple of page size.
 */
void set_page_address(Entry* entry, const void* address);

/**
 * Check whether the page was written to.
 *
 * @param entry The PTE.
 * @return True iff the page was written to.
 */
[[nodiscard]] bool is_dirty(Entry entry);

/**
 * Check whether the page was accessed.
 *
 * @param entry The PTE.
 * @return True iff the page was accessed.
 */
[[nodiscard]] bool was_accessed(Entry entry);

/**
 * Reset the page to seem it was not accessed.
 *
 * @param entry The PTE.
 */
void reset_accessed(Entry* entry);

/**
 * Check whether the page can be accessed by the user.
 *
 * @param entry The PTE.
 * @return True iff the user can access the page.
 */
[[nodiscard]] bool can_user_access(Entry entry);

/**
 * Allow usermode access to the page.
 *
 * @param entry The PTE.
 */
void enable_user_access(Entry* entry);

/**
 * Prevent usermode access to the page.
 *
 * @param entry The PTE.
 */
void disable_user_access(Entry* entry);

/**
 * Check if the page is writeable.
 *
 * @param entry The PTE.
 * @return True iff the page is writeable.
 */
[[nodiscard]] bool is_writeable(Entry entry);

/**
 * Make the page writeable.
 *
 * @param entry The PTE.
 */
void enable_writing(Entry* entry);

/**
 * Make the page read only.
 *
 * @param entry The PTE.
 */
void disable_writing(Entry* entry);

/**
 * Check whether the page is present in memory.
 *
 * @param entry The PTE.
 * @return True iff the page is present in memory.
 */
[[nodiscard]] bool is_present(Entry entry);

/**
 * Mark the page as present in memory.
 *
 * @param entry The PTE.
 */
void mark_present(Entry* entry);

/**
 * Mark the page as not present in memory.
 *
 * @param entry The PTE.
 */
void mark_not_present(Entry* entry);

}  // namespace memory::paging::table
