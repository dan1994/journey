#include "memory/paging/page_directory.hpp"

#include <cassert>
#include <cstring>
#include <new>
#include <type_traits>

#include "utilities/bitranges.hpp"
#include "utilities/memory.hpp"

namespace memory::paging {

constexpr size_t PAGE_TABLE_ADDRESS_MSB = 31;
constexpr size_t PAGE_TABLE_ADDRESS_LSB = 12;
constexpr size_t PAGE_SIZE_FLAG_OFFSET = 7;
constexpr size_t ACCESSED_FLAG_OFFSET = 5;
constexpr size_t CACHE_DISABLE_FLAG_OFFSET = 4;
constexpr size_t CACHE_WRITE_MODE_FLAG_OFFSET = 3;
constexpr size_t PRIVILEDGE_LEVEL_FLAG_OFFSET = 2;
constexpr size_t ACCESS_TYPE_FLAG_OFFSET = 1;
constexpr size_t PRESENT_FLAG_OFFSET = 0;

PageDirectoryEntry::PageDirectoryEntry(const PageTable& page_table,
                                       const Flags& flags) {
    constexpr bool PAGE_SIZE_4KB = 0;
    constexpr bool WASNT_ACCESSED = 0;
    constexpr bool CACHE_DISABLED = 1;
    constexpr bool WRITE_THROUGH = 1;

    const uint32_t page_table_address_field =
        utilities::get_field(reinterpret_cast<uint32_t>(page_table.entries()),
                             PAGE_TABLE_ADDRESS_MSB, PAGE_TABLE_ADDRESS_LSB);

    assertm(page_table_address_field << PAGE_TABLE_ADDRESS_LSB ==
                reinterpret_cast<uint32_t>(page_table.entries()),
            "Page table address is not properly aligned");

    value_ = page_table_address_field << PAGE_TABLE_ADDRESS_LSB |
             PAGE_SIZE_4KB << PAGE_SIZE_FLAG_OFFSET |
             WASNT_ACCESSED << ACCESSED_FLAG_OFFSET |
             CACHE_DISABLED << CACHE_DISABLE_FLAG_OFFSET |
             WRITE_THROUGH << CACHE_WRITE_MODE_FLAG_OFFSET |
             std::underlying_type_t<PriviledgeLevel>(flags.priviledge_level)
                 << PRIVILEDGE_LEVEL_FLAG_OFFSET |
             std::underlying_type_t<AccessType>(flags.access_type)
                 << ACCESS_TYPE_FLAG_OFFSET |
             std::underlying_type_t<Present>(flags.present)
                 << PRESENT_FLAG_OFFSET;
}

PageTable PageDirectoryEntry::get_page_table() const {
    const uint32_t address_in_tables = utilities::get_field(
        value_, PAGE_TABLE_ADDRESS_MSB, PAGE_TABLE_ADDRESS_LSB);

    PageTableEntry* entries = reinterpret_cast<PageTableEntry*>(
        address_in_tables << PAGE_TABLE_ADDRESS_LSB);

    return PageTable(entries);
}

void PageDirectoryEntry::set_page_table_address(const PageTable* address) {
    const uint32_t address_in_tables =
        reinterpret_cast<uint32_t>(address) >> PAGE_TABLE_ADDRESS_LSB;

    value_ = utilities::set_field(value_, PAGE_TABLE_ADDRESS_MSB,
                                  PAGE_TABLE_ADDRESS_LSB, address_in_tables);
}

bool PageDirectoryEntry::was_accessed() const {
    return utilities::get_flag(value_, ACCESSED_FLAG_OFFSET);
}

void PageDirectoryEntry::reset_accessed() {
    value_ = utilities::clear_flag(value_, ACCESSED_FLAG_OFFSET);
}

bool PageDirectoryEntry::can_user_access() const {
    return utilities::get_flag(value_, PRIVILEDGE_LEVEL_FLAG_OFFSET);
}

void PageDirectoryEntry::enable_user_access() {
    value_ = utilities::set_flag(value_, PRIVILEDGE_LEVEL_FLAG_OFFSET);
}

void PageDirectoryEntry::disable_user_access() {
    value_ = utilities::clear_flag(value_, PRIVILEDGE_LEVEL_FLAG_OFFSET);
}

bool PageDirectoryEntry::is_writeable() const {
    return utilities::get_flag(value_, ACCESS_TYPE_FLAG_OFFSET);
}

void PageDirectoryEntry::enable_writing() {
    value_ = utilities::set_flag(value_, ACCESS_TYPE_FLAG_OFFSET);
}

void PageDirectoryEntry::disable_writing() {
    value_ = utilities::clear_flag(value_, ACCESS_TYPE_FLAG_OFFSET);
}

bool PageDirectoryEntry::is_present() const {
    return utilities::get_flag(value_, PRESENT_FLAG_OFFSET);
}

void PageDirectoryEntry::mark_present() {
    value_ = utilities::set_flag(value_, PRESENT_FLAG_OFFSET);
}

void PageDirectoryEntry::mark_not_present() {
    value_ = utilities::clear_flag(value_, PRESENT_FLAG_OFFSET);
}

PageDirectory::PageDirectory(const PageDirectoryEntry::Flags& flags,
                             const PageTable* page_tables)
    : entries_(utilities::uninitialized_array_of<PageDirectoryEntry>(
          PageDirectory::NUMBER_OF_ENTRIES)) {
    for (size_t i = 0; i < NUMBER_OF_ENTRIES; i++) {
        new (entries_ + i) PageDirectoryEntry{page_tables[i], flags};
    }
}

PageDirectory::~PageDirectory() {
    delete[] entries_;
}

const PageDirectoryEntry* PageDirectory::entries() const {
    return entries_;
}

WithError<const PageDirectoryEntry&> PageDirectory::operator[](
    size_t offset) const {
    if (offset < NUMBER_OF_ENTRIES) {
        return {
            entries_[0],
            WITH_LOCATION("Attempt to access PageDirectoryEntry out of range")};
    }
    return {entries_[offset], nullptr};
}

WithError<PageDirectoryEntry&> PageDirectory::operator[](size_t offset) {
    if (offset < NUMBER_OF_ENTRIES) {
        return {
            entries_[0],
            WITH_LOCATION("Attempt to access PageDirectoryEntry out of range")};
    }
    return {entries_[offset], nullptr};
}

}  // namespace memory::paging
