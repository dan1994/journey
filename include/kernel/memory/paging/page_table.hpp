#pragma once

#include <stddef.h>
#include <stdint.h>

#include <cstddef>

#include "memory/paging/common_flags.hpp"

namespace memory::paging {

class PageTableEntry final {
   public:
    /**
     * Create an entry that points to page 0, accessible by the kernel only,
     * read only and not present in memory.
     */
    PageTableEntry();

    /**
     * Create an entry with the given parameters.
     * @param page_address The page to point to. The address is rounded down to
     * a multiple of page size.
     * @param priviledge_level Whether the user can access the page or only the
     * kernel.
     * @param access_type Whether the page is writeable or only readable.
     * @param present Wether the page is present in memory or not.
     */
    explicit PageTableEntry(const std::byte* page_address,
                            PriviledgeLevel priviledge_level,
                            AccessType access_type, Present present);

    PageTableEntry(const PageTableEntry&) = default;
    PageTableEntry(PageTableEntry&&) = default;
    PageTableEntry& operator=(const PageTableEntry&) = default;
    PageTableEntry& operator=(PageTableEntry&&) = default;

    ~PageTableEntry() = default;

    /**
     * Get the address of the page pointed to by this entry.
     * @return The address of the page.
     */
    std::byte* get_page_address() const;

    /**
     * Set the address of the page pointed to by this entry.
     * @param address The address of the page. The address is rounded down to
     * a multiple of page size.
     */
    void set_page_address(const std::byte* address);

    /**
     * Check whether the page was written to.
     * @return True iff the page was written to.
     */
    bool is_dirty() const;

    /**
     * Check whether the page was accessed.
     * @return True iff the page was accessed.
     */
    bool was_accessed() const;

    /**
     * Reset the page to seem it was not accessed.
     */
    void reset_accessed();

    /**
     * Check whether the page can be accessed by the user.
     * @return True iff the user can access the page.
     */
    bool can_user_access() const;

    /**
     * Allow usermode to access the page.
     */
    void enable_user_access();

    /**
     * Prevent usermode to access the page.
     */
    void disable_user_access();

    /**
     * Check if the page is writeable.
     * @return True iff the page is writeable.
     */
    bool is_writeable() const;

    /**
     * Make the page writeable.
     */
    void enable_writing();

    /**
     * Make the page read only.
     */
    void disable_writing();

    /**
     * Check whether the page is present in memory.
     */
    bool is_present() const;

    /**
     * Mark the page as present in memory.
     */
    void mark_present();

    /**
     * Mark the page as not present in memory.
     */
    void mark_not_present();

   private:
    uint32_t value_;
} __attribute__((packed));

class PageTable final {
   public:
    /**
     * Create a page table with all entries set to default configuration.
     */
    explicit PageTable() = default;

    ~PageTable() = default;

    /**
     * Get a pointer to the start of the table.
     * @return Pointer to the start of the table.
     */
    const PageTableEntry* entries() const;

    /**
     * Get a specific entry in the table.
     * @param offset The offset of the entry to get.
     * @return A reference to the entry.
     */
    const PageTableEntry& operator[](size_t offset) const;

    /**
     * Get a specific entry in the table.
     * @param offset The offset of the entry to get.
     * @return A reference to the entry.
     */
    PageTableEntry& operator[](size_t offset);

    PageTable(const PageTable&) = delete;
    PageTable(PageTable&&) = delete;
    PageTable& operator=(const PageTable&) = delete;
    PageTable& operator=(PageTable&&) = delete;

   private:
    static constexpr size_t NUMBER_OF_ENTRIES = 1024;
    PageTableEntry entries_[NUMBER_OF_ENTRIES];
} __attribute__((packed));

}  // namespace memory::paging
