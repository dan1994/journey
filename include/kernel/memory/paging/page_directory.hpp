#pragma once

#include <stddef.h>
#include <stdint.h>

#include <cstddef>

#include "memory/paging/constants.hpp"
#include "memory/paging/page_table.hpp"

namespace memory::paging {

class PageDirectoryEntry final {
   public:
    struct Flags {
        PriviledgeLevel priviledge_level;
        AccessType access_type;
        Present present;
    };

    /**
     * Create an entry with the given parameters.
     * @param page_table The page table to point to.
     * @param flags The flags to apply to the page table.
     */
    explicit PageDirectoryEntry(const PageTable& page_table,
                                const Flags& flags);

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
     * Create a page directory with all entries set to given configuration.
     * Assumes the page tables are contiguous in memory.
     * @param flags The flags to apply to each entry.
     * @param page_tables A pointer to the an array of page tables.
     */
    explicit PageDirectory(const PageDirectoryEntry::Flags& flags,
                           const PageTable* page_tables);

    ~PageDirectory();

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

    static constexpr size_t NUMBER_OF_ENTRIES = 1024;

   private:
    PageDirectoryEntry* entries_;
};

}  // namespace memory::paging
