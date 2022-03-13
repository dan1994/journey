#pragma once

#include <stddef.h>
#include <stdint.h>

#include <cstddef>

#include "memory/paging/constants.hpp"
#include "utilities/error.hpp"

namespace memory::paging {

class PageTableEntry final {
   public:
    struct Flags {
        PriviledgeLevel priviledge_level;
        AccessType access_type;
        Present present;
    };

    /**
     * Create an entry that points to page 0, accessible by the kernel only,
     * read only and not present in memory.
     */
    explicit PageTableEntry();

    /**
     * Create an entry with the given parameters.
     * @param page_address The page to point to. The address is rounded down to
     * a multiple of page size.
     * @param flags The flags to apply to the page.
     */
    explicit PageTableEntry(const std::byte* page_address, const Flags& flags);

    PageTableEntry(const PageTableEntry&) = default;
    PageTableEntry(PageTableEntry&&) = default;
    PageTableEntry& operator=(const PageTableEntry&) = default;
    PageTableEntry& operator=(PageTableEntry&&) = default;

    ~PageTableEntry() = default;

    /**
     * Get the address of the page pointed to by this entry.
     * @return The address of the page.
     */
    [[nodiscard]] std::byte* get_page_address() const;

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
    [[nodiscard]] bool is_dirty() const;

    /**
     * Check whether the page was accessed.
     * @return True iff the page was accessed.
     */
    [[nodiscard]] bool was_accessed() const;

    /**
     * Reset the page to seem it was not accessed.
     */
    void reset_accessed();

    /**
     * Check whether the page can be accessed by the user.
     * @return True iff the user can access the page.
     */
    [[nodiscard]] bool can_user_access() const;

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
    [[nodiscard]] bool is_writeable() const;

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
     * @return True iff the page is present in memory.
     */
    [[nodiscard]] bool is_present() const;

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
    // Whether to initialize the table with addresses zeroed out or rising
    // linearly.
    enum class InitializationMode { ZEROED, LINEAR };

    /**
     * Create a page table with all entries set to given configuration.
     * @param flags The flags to apply to all entries.
     * @param initialization_mode How to initialize the page addresses.
     * Supported modes are zeroed and linear initialization.
     * @param offset The address to start from in linear initialization mode.
     */
    explicit PageTable(
        const PageTableEntry::Flags& flags,
        InitializationMode initialization_mode = InitializationMode::ZEROED,
        size_t offset = 0);

    /**
     * Wrap an existing PTE array with the PageTable construct. Doesn't assume
     * ownership over array memory, so it will not be deleted when destructor is
     * called.
     * @param entries The PTE array.
     */
    explicit PageTable(PageTableEntry* entries);

    ~PageTable();

    /**
     * Get a pointer to the start of the table.
     * @return Pointer to the start of the table.
     */
    [[nodiscard]] const PageTableEntry* entries() const;

    /**
     * Get a specific entry in the table.
     * @param offset The offset of the entry to get.
     * @return A reference to the entry.
     */
    [[nodiscard]] WithError<const PageTableEntry&> operator[](
        size_t offset) const;

    /**
     * Get a specific entry in the table.
     * @param offset The offset of the entry to get.
     * @return A reference to the entry.
     */
    [[nodiscard]] WithError<PageTableEntry&> operator[](size_t offset);

    PageTable(const PageTable&) = delete;
    PageTable(PageTable&&) = delete;
    PageTable& operator=(const PageTable&) = delete;
    PageTable& operator=(PageTable&&) = delete;

    static constexpr size_t NUMBER_OF_ENTRIES = 1024;

   private:
    PageTableEntry* const entries_;
    const bool owns_memory_;
};

}  // namespace memory::paging
