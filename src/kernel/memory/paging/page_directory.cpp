#include "memory/paging/page_directory.hpp"

#include <cassert>
#include <type_traits>

#include "utilities/bitranges.hpp"

namespace memory::paging::directory {

constexpr size_t PAGE_TABLE_ADDRESS_MSB = 31;
constexpr size_t PAGE_TABLE_ADDRESS_LSB = 12;
constexpr size_t PAGE_SIZE_FLAG_OFFSET = 7;
constexpr size_t ACCESSED_FLAG_OFFSET = 5;
constexpr size_t CACHE_DISABLE_FLAG_OFFSET = 4;
constexpr size_t CACHE_WRITE_MODE_FLAG_OFFSET = 3;
constexpr size_t PRIVILEDGE_LEVEL_FLAG_OFFSET = 2;
constexpr size_t ACCESS_TYPE_FLAG_OFFSET = 1;
constexpr size_t PRESENT_FLAG_OFFSET = 0;

Entry make_entry(const PTE* page_table, const Flags& flags) {
    constexpr bool PAGE_SIZE_4KB = 0;
    constexpr bool WASNT_ACCESSED = 0;
    constexpr bool CACHE_DISABLED = 1;
    constexpr bool WRITE_THROUGH = 1;

    const uint32_t page_table_address_field =
        utilities::get_field(reinterpret_cast<uint32_t>(page_table),
                             PAGE_TABLE_ADDRESS_MSB, PAGE_TABLE_ADDRESS_LSB);

    assertm(page_table_address_field << PAGE_TABLE_ADDRESS_LSB ==
                reinterpret_cast<uint32_t>(page_table),
            "Page table address is not properly aligned");

    return page_table_address_field << PAGE_TABLE_ADDRESS_LSB |
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

PTE* get_pte_address(Entry entry) {
    const uint32_t address_in_tables = utilities::get_field(
        entry, PAGE_TABLE_ADDRESS_MSB, PAGE_TABLE_ADDRESS_LSB);

    return reinterpret_cast<PTE*>(address_in_tables << PAGE_TABLE_ADDRESS_LSB);
}

void set_pte_address(Entry* entry, const PTE* address) {
    const uint32_t address_in_tables =
        reinterpret_cast<uint32_t>(address) >> PAGE_TABLE_ADDRESS_LSB;

    *entry = utilities::set_field(*entry, PAGE_TABLE_ADDRESS_MSB,
                                  PAGE_TABLE_ADDRESS_LSB, address_in_tables);
}

bool was_accessed(Entry entry) {
    return utilities::get_flag(entry, ACCESSED_FLAG_OFFSET);
}

void reset_accessed(Entry* entry) {
    *entry = utilities::clear_flag(*entry, ACCESSED_FLAG_OFFSET);
}

bool can_user_access(Entry entry) {
    return utilities::get_flag(entry, PRIVILEDGE_LEVEL_FLAG_OFFSET);
}

void enable_user_access(Entry* entry) {
    *entry = utilities::set_flag(*entry, PRIVILEDGE_LEVEL_FLAG_OFFSET);
}

void disable_user_access(Entry* entry) {
    *entry = utilities::clear_flag(*entry, PRIVILEDGE_LEVEL_FLAG_OFFSET);
}

bool is_writeable(Entry entry) {
    return utilities::get_flag(entry, ACCESS_TYPE_FLAG_OFFSET);
}

void enable_writing(Entry* entry) {
    *entry = utilities::set_flag(*entry, ACCESS_TYPE_FLAG_OFFSET);
}

void disable_writing(Entry* entry) {
    *entry = utilities::clear_flag(*entry, ACCESS_TYPE_FLAG_OFFSET);
}

bool is_present(Entry entry) {
    return utilities::get_flag(entry, PRESENT_FLAG_OFFSET);
}

void mark_present(Entry* entry) {
    *entry = utilities::set_flag(*entry, PRESENT_FLAG_OFFSET);
}

void mark_not_present(Entry* entry) {
    *entry = utilities::clear_flag(*entry, PRESENT_FLAG_OFFSET);
}

}  // namespace memory::paging::directory
