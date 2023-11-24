#include "memory/paging/paging.hpp"

#include <stddef.h>

#include <cstddef>
#include <utility>

#include "utilities/bitranges.hpp"

extern "C" void enable_paging();
extern "C" void disable_paging();
extern "C" void load_page_directory(const void* page_directory);

namespace memory::paging {

size_t get_directory_offset(const void* virtual_address);
size_t get_table_offset(const void* virtual_address);

with_error<paging> make(allocator* allocator,
                        const directory::Flags& directory_flags,
                        const table::Flags& table_flags) {
    paging paging{.allocator_ = allocator};

    auto [directory_allocation, directory_error] =
        try_malloc(allocator, directory::ENTRY_NUM * sizeof(directory::Entry));
    if (errors::set(directory_error)) {
        errors::enrich(&directory_error, "allocate page directory");
        return {paging, directory_error};
    }

    directory::Entry* const directory =
        reinterpret_cast<directory::Entry*>(directory_allocation);
    paging.directory = directory;

    auto [tables_allocation, tables_error] =
        try_malloc(allocator, directory::ENTRY_NUM * sizeof(table::Entry*));
    if (errors::set(tables_error)) {
        errors::enrich(&tables_error, "allocate page tables");
        return {paging, tables_error};
    }

    table::Entry** const tables =
        reinterpret_cast<table::Entry**>(tables_allocation);
    paging.tables = tables;

    const std::byte* address = 0;

    for (size_t directory_index = 0; directory_index < directory::ENTRY_NUM;
         directory_index++) {
        auto [table, table_error] =
            try_malloc(allocator, table::ENTRY_NUM * sizeof(table::Entry));
        if (errors::set(table_error)) {
            errors::enrich(&table_error, "allocate page table");
            return {paging, table_error};
        }

        tables[directory_index] = reinterpret_cast<table::Entry*>(table);

        for (size_t table_index = 0; table_index < table::ENTRY_NUM;
             table_index++) {
            tables[directory_index][table_index] =
                table::make_entry(address, table_flags);

            address += PAGE_SIZE_IN_BYTES;
        }

        directory[directory_index] =
            directory::make_entry(tables[directory_index], directory_flags);
    }

    return {paging, errors::nil()};
}

error destroy(paging* paging) {
    error first = errors::nil();

    if (paging->tables != nullptr) {
        for (size_t i = 0; i < directory::ENTRY_NUM; i++) {
            if (paging->tables[i] != nullptr) {
                const error temp =
                    try_free(paging->allocator_, paging->tables[i]);
                if (errors::set(temp) && !errors::set(first)) {
                    first = temp;
                }
            }
        }

        const error temp = try_free(paging->allocator_, paging->tables);
        if (errors::set(temp) && !errors::set(first)) {
            first = temp;
        }
    }

    if (paging->directory != nullptr) {
        const error temp = try_free(paging->allocator_, paging->directory);
        if (errors::set(temp) && !errors::set(first)) {
            first = temp;
        }
    }

    return first;
}

error map(paging* paging, const void* virtual_address,
          const void* physical_address, const flags& flags) {
    const size_t directory_offset = get_directory_offset(virtual_address);
    directory::Entry* const pde = &paging->directory[directory_offset];
    if (!directory::is_present(*pde)) {
        return errors::make(WITH_LOCATION("non-present page table"));
    }

    const size_t table_offset = get_table_offset(virtual_address);
    table::Entry* const pte = &paging->tables[directory_offset][table_offset];

    table::set_page_address(pte, physical_address);

    if (flags.access_type == AccessType::READ_WRITE) {
        table::enable_writing(pte);
        directory::enable_writing(pde);
    } else {
        table::disable_writing(pte);
    }

    if (flags.priviledge_level == PriviledgeLevel::USER) {
        table::enable_user_access(pte);
        directory::enable_user_access(pde);
    } else {
        table::disable_user_access(pte);
    }

    table::mark_present(pte);

    return errors::nil();
}

size_t get_directory_offset(const void* virtual_address) {
    return reinterpret_cast<unsigned int>(virtual_address) /
           (table::ENTRY_NUM * PAGE_SIZE_IN_BYTES);
}

size_t get_table_offset(const void* virtual_address) {
    return reinterpret_cast<unsigned int>(virtual_address) /
           PAGE_SIZE_IN_BYTES % table::ENTRY_NUM;
}

void enable() {
    enable_paging();
}

void disable() {
    disable_paging();
}

void load(const paging instance) {
    load_page_directory(instance.directory);
}

}  // namespace memory::paging
