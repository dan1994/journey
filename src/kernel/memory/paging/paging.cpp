#include "memory/paging/paging.hpp"

#include <stddef.h>

#include <cstddef>
#include <utility>

#include "utilities/bitranges.hpp"

extern "C" void enable_paging();

extern "C" void load_page_directory(const void* page_directory);

namespace memory::paging {

with_error<Paging> Paging::make(allocator* allocator,
                                const directory::Flags& directory_flags,
                                const table::Flags& table_flags,
                                InitializationMode initialization_mode) {
    auto [directory_allocation, directory_error] =
        try_malloc(allocator, directory::ENTRY_NUM * sizeof(directory::Entry));
    if (errors::set(directory_error)) {
        errors::enrich(&directory_error, "allocate page directory");
        return {Paging{allocator, nullptr, nullptr}, directory_error};
    }

    directory::Entry* const directory =
        reinterpret_cast<directory::Entry*>(directory_allocation);

    auto [tables_allocation, tables_error] =
        try_malloc(allocator, directory::ENTRY_NUM * sizeof(table::Entry*));
    if (errors::set(tables_error)) {
        errors::enrich(&tables_error, "allocate page tables");
        return {Paging{allocator, directory, nullptr}, tables_error};
    }

    table::Entry** const tables =
        reinterpret_cast<table::Entry**>(tables_allocation);

    const std::byte* address = 0;

    for (size_t directory_index = 0; directory_index < directory::ENTRY_NUM;
         directory_index++) {
        auto [table, table_error] =
            try_malloc(allocator, table::ENTRY_NUM * sizeof(table::Entry));
        if (errors::set(table_error)) {
            errors::enrich(&table_error, "allocate page table");
            return {Paging{allocator, directory, tables}, table_error};
        }

        tables[directory_index] = reinterpret_cast<table::Entry*>(table);

        for (size_t table_index = 0; table_index < table::ENTRY_NUM;
             table_index++) {
            tables[directory_index][table_index] =
                table::make_entry(address, table_flags);

            if (initialization_mode == InitializationMode::LINEAR) {
                address += PAGE_SIZE_IN_BYTES;
            }
        }

        directory[directory_index] =
            directory::make_entry(tables[directory_index], directory_flags);
    }

    return {Paging(allocator, directory, tables), errors::nil()};
}

Paging::Paging(allocator* allocator, directory::Entry* directory,
               table::Entry** tables)
    : allocator_(allocator), directory_(directory), tables_(tables) {}

Paging::Paging(Paging&& other)
    : directory_(std::move(other.directory_)),
      tables_(std::move(other.tables_)) {
    other.directory_ = nullptr;
    other.tables_ = nullptr;
}

Paging& Paging::operator=(Paging&& other) {
    if (this == &other) {
        return *this;
    }

    this->directory_ = std::move(other.directory_);
    this->tables_ = std::move(other.tables_);

    other.directory_ = nullptr;
    other.tables_ = nullptr;

    return *this;
}

Paging::~Paging() {
    if (tables_ != nullptr) {
        for (size_t i = 0; i < directory::ENTRY_NUM; i++) {
            if (tables_[i] != nullptr) {
                free(allocator_, tables_[i]);
            }
        }
        free(allocator_, tables_);
    }
    if (directory_ != nullptr) {
        free(allocator_, directory_);
    }
}

const void* Paging::get_directory() const {
    return directory_;
}

error Paging::map(const void* virtual_address, const void* physical_address,
                  const Flags& flags) {
    const size_t directory_offset = get_directory_offset(virtual_address);
    directory::Entry* const pde = &directory_[directory_offset];
    if (!directory::is_present(*pde)) {
        return errors::make(WITH_LOCATION(
            "can't map address because of non-present page table"));
    }

    const size_t table_offset = get_table_offset(virtual_address);
    table::Entry* const pte = &tables_[directory_offset][table_offset];

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

size_t Paging::get_directory_offset(const void* virtual_address) {
    return reinterpret_cast<unsigned int>(virtual_address) /
           (table::ENTRY_NUM * PAGE_SIZE_IN_BYTES);
}

size_t Paging::get_table_offset(const void* virtual_address) {
    return reinterpret_cast<unsigned int>(virtual_address) /
           PAGE_SIZE_IN_BYTES % table::ENTRY_NUM;
}

void enable_paging() {
    ::enable_paging();
}

void load(const Paging& instance) {
    load_page_directory(instance.get_directory());
}

}  // namespace memory::paging
