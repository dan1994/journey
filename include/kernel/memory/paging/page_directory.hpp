#pragma once

#include <stddef.h>
#include <stdint.h>

#include <cstddef>

#include "memory/paging/common_flags.hpp"
#include "memory/paging/page_table.hpp"

namespace memory::paging {

class PageDirectoryEntry final {
   public:
    /**
     * Create an entry that points to page table 0, accessible by the kernel
     * only, read only and not present in memory.
     */
    explicit PageDirectoryEntry() = default;

    /**
     * Create an entry with the given parameters.
     * @param page_table_address The page table to point to.
     * @param priviledge_level Whether the user can access the page or only the
     * kernel.
     * @param access_type Whether the page is writeable or only readable.
     * @param present Wether the page is present in memory or not.
     */
    explicit PageDirectoryEntry(const PageTable* page_table_address,
                                PriviledgeLevel priviledge_level,
                                AccessType access_type, Present present);

    PageDirectoryEntry(const PageDirectoryEntry&) = default;
    PageDirectoryEntry(PageDirectoryEntry&&) = default;
    PageDirectoryEntry& operator=(const PageDirectoryEntry&) = default;
    PageDirectoryEntry& operator=(PageDirectoryEntry&&) = default;

    ~PageDirectoryEntry() = default;

    /**
     * Get the address of the page table pointed to by this entry.
     * @return The address of the page table.
     */
    PageTable* get_page_table_address() const;

    /**
     * Set the address of the page table pointed to by this entry.
     * @param address The address of the page table.
     */
    void set_page_table_address(const PageTable* address);

    /**
     * Check whether the page table was accessed.
     * @return True iff the page table was accessed.
     */
    bool was_accessed() const;

    /**
     * Reset the page table to seem it was not accessed.
     */
    void reset_accessed();

    /**
     * Check whether the page table can be accessed by the user.
     * @return True iff the user can access the page table.
     */
    bool can_user_access() const;

    /**
     * Allow usermode to access the page table.
     */
    void enable_user_access();

    /**
     * Prevent usermode to access the page table.
     */
    void disable_user_access();

    /**
     * Check if the page table is writeable.
     * @return True iff the page table is writeable.
     */
    bool is_writeable() const;

    /**
     * Make the page table writeable.
     */
    void enable_writing();

    /**
     * Make the page table read only.
     */
    void disable_writing();

    /**
     * Check whether the page table is present in memory.
     */
    bool is_present() const;

    /**
     * Mark the page table as present in memory.
     */
    void mark_present();

    /**
     * Mark the page table as not present in memory.
     */
    void mark_not_present();

   private:
    uint32_t value_;
} __attribute__((packed));

class PageDirectory final {
   public:
    /**
     * Create a page directory with all entries set to default configuration.
     */
    explicit PageDirectory();

    ~PageDirectory() = default;

    /**
     * Get a pointer to the start of the table.
     * @return Pointer to the start of the table.
     */
    const PageDirectoryEntry* entries() const;

    /**
     * Get a specific entry in the table.
     * @param offset The offset of the entry to get.
     * @return A reference to the entry.
     */
    const PageDirectoryEntry& operator[](size_t offset) const;

    /**
     * Get a specific entry in the table.
     * @param offset The offset of the entry to get.
     * @return A reference to the entry.
     */
    PageDirectoryEntry& operator[](size_t offset);

    PageDirectory(const PageDirectory&) = delete;
    PageDirectory(PageDirectory&&) = delete;
    PageDirectory& operator=(const PageDirectory&) = delete;
    PageDirectory& operator=(PageDirectory&&) = delete;

   private:
    static constexpr size_t NUMBER_OF_ENTRIES = 1024;
    PageDirectoryEntry entries_[NUMBER_OF_ENTRIES];
} __attribute__((packed));

}  // namespace memory::paging
