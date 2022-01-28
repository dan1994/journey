#include "memory/paging/page_table.hpp"

#include <cassert>
#include <cstring>
#include <new>
#include <type_traits>

#include "utilities/bitranges.hpp"
#include "utilities/memory.hpp"

namespace memory::paging {

constexpr PageTableEntry::Flags DEFAULT_FLAGS = {
    PriviledgeLevel::KERNEL, AccessType::READ_ONLY, Present::FALSE};

constexpr size_t PAGE_ADDRESS_MSB = 31;
constexpr size_t PAGE_ADDRESS_LSB = 12;
constexpr size_t GLOBAL_FLAG_OFFSET = 8;
constexpr size_t PAT_FLAG_OFFSET = 7;
constexpr size_t DIRTY_FLAG_OFFSET = 6;
constexpr size_t ACCESSED_FLAG_OFFSET = 5;
constexpr size_t CACHE_DISABLE_FLAG_OFFSET = 4;
constexpr size_t CACHE_WRITE_MODE_FLAG_OFFSET = 3;
constexpr size_t PRIVILEDGE_LEVEL_FLAG_OFFSET = 2;
constexpr size_t ACCESS_TYPE_FLAG_OFFSET = 1;
constexpr size_t PRESENT_FLAG_OFFSET = 0;

PageTableEntry::PageTableEntry() : PageTableEntry(0, DEFAULT_FLAGS) {}

PageTableEntry::PageTableEntry(const std::byte* page_address,
                               const Flags& flags) {
    constexpr bool NOT_GLOBAL = false;
    constexpr bool PAT_DISABLED = 0;
    constexpr bool NOT_DIRTY = 0;
    constexpr bool WASNT_ACCESSED = 0;
    constexpr bool CACHE_DISABLED = 1;
    constexpr bool WRITE_THROUGH = 1;

    const uint32_t page_address_field =
        utilities::get_field(reinterpret_cast<uint32_t>(page_address),
                             PAGE_ADDRESS_MSB, PAGE_ADDRESS_LSB);

    value_ = page_address_field << PAGE_ADDRESS_LSB |
             NOT_GLOBAL << GLOBAL_FLAG_OFFSET |
             PAT_DISABLED << PAT_FLAG_OFFSET | NOT_DIRTY << DIRTY_FLAG_OFFSET |
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

std::byte* PageTableEntry::get_page_address() const {
    const uint32_t address_in_pages =
        utilities::get_field(value_, PAGE_ADDRESS_MSB, PAGE_ADDRESS_LSB);

    return reinterpret_cast<std::byte*>(address_in_pages << PAGE_ADDRESS_LSB);
}

void PageTableEntry::set_page_address(const std::byte* address) {
    const uint32_t address_in_pages =
        reinterpret_cast<uint32_t>(address) >> PAGE_ADDRESS_LSB;

    value_ = utilities::set_field(value_, PAGE_ADDRESS_MSB, PAGE_ADDRESS_LSB,
                                  address_in_pages);
}

bool PageTableEntry::is_dirty() const {
    return utilities::get_flag(value_, DIRTY_FLAG_OFFSET);
}

bool PageTableEntry::was_accessed() const {
    return utilities::get_flag(value_, ACCESSED_FLAG_OFFSET);
}

void PageTableEntry::reset_accessed() {
    value_ = utilities::clear_flag(value_, ACCESSED_FLAG_OFFSET);
}

bool PageTableEntry::can_user_access() const {
    return utilities::get_flag(value_, PRIVILEDGE_LEVEL_FLAG_OFFSET);
}

void PageTableEntry::enable_user_access() {
    value_ = utilities::set_flag(value_, PRIVILEDGE_LEVEL_FLAG_OFFSET);
}

void PageTableEntry::disable_user_access() {
    value_ = utilities::clear_flag(value_, PRIVILEDGE_LEVEL_FLAG_OFFSET);
}

bool PageTableEntry::is_writeable() const {
    return utilities::get_flag(value_, ACCESS_TYPE_FLAG_OFFSET);
}

void PageTableEntry::enable_writing() {
    value_ = utilities::set_flag(value_, ACCESS_TYPE_FLAG_OFFSET);
}

void PageTableEntry::disable_writing() {
    value_ = utilities::clear_flag(value_, ACCESS_TYPE_FLAG_OFFSET);
}

bool PageTableEntry::is_present() const {
    return utilities::get_flag(value_, PRESENT_FLAG_OFFSET);
}

void PageTableEntry::mark_present() {
    value_ = utilities::set_flag(value_, PRESENT_FLAG_OFFSET);
}

void PageTableEntry::mark_not_present() {
    value_ = utilities::clear_flag(value_, PRESENT_FLAG_OFFSET);
}

PageTable::PageTable(const PageTableEntry::Flags& flags,
                     InitializationMode initialization_mode, size_t offset)
    : entries_(utilities::uninitialized_array_of<PageTableEntry>(
          NUMBER_OF_ENTRIES)) {
    for (size_t i = 0; i < NUMBER_OF_ENTRIES; i++) {
        const std::byte* const address =
            initialization_mode == InitializationMode::ZEROED
                ? 0
                : reinterpret_cast<std::byte*>(PAGE_SIZE_IN_BYTES * i + offset);

        new (entries_ + i) PageTableEntry{address, flags};
    }
}

PageTable::~PageTable() {
    delete[] entries_;
}

const PageTableEntry* PageTable::entries() const {
    return entries_;
}

const PageTableEntry& PageTable::operator[](size_t offset) const {
    assertm(offset < NUMBER_OF_ENTRIES,
            "Attempt to access PageTableEntry out of range");
    return entries_[offset];
}

PageTableEntry& PageTable::operator[](size_t offset) {
    assertm(offset < NUMBER_OF_ENTRIES,
            "Attempt to access PageTableEntry out of range");
    return entries_[offset];
}

}  // namespace memory::paging
