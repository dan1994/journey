#include "memory/paging/paging.hpp"

#include <stddef.h>

#include <cstddef>
#include <utility>

#include "utilities/bitranges.hpp"

extern "C" void enable_paging();

extern "C" void load_page_directory(const void* page_directory);

namespace memory::paging {

WithError<Paging> Paging::make(const directory::Flags& directory_flags,
                               const table::Flags& table_flags,
                               InitializationMode initialization_mode) {
    directory::Entry* const directory =
        new directory::Entry[directory::ENTRY_NUM];
    table::Entry** const tables = new table::Entry*[directory::ENTRY_NUM];

    if (directory == nullptr || tables == nullptr) {
        return {Paging(directory, tables),
                WITH_LOCATION("Failed to allocate paging directory/tables")};
    }

    const std::byte* address = 0;

    for (size_t directory_index = 0; directory_index < directory::ENTRY_NUM;
         directory_index++) {
        directory[directory_index] =
            directory::make_entry(tables[directory_index], directory_flags);

        tables[directory_index] = new table::Entry[table::ENTRY_NUM];
        if (tables[directory_index] == nullptr) {
            return {
                Paging(directory, tables),
                WITH_LOCATION("Failed to allocate paging directory/tables")};
        }
        for (size_t table_index = 0; table_index < table::ENTRY_NUM;
             table_index++) {
            tables[directory_index][table_index] =
                table::make_entry(address, table_flags);

            if (initialization_mode == InitializationMode::LINEAR) {
                address += PAGE_SIZE_IN_BYTES;
            }
        }
    }

    return {Paging(directory, tables), nullptr};
}

Paging::Paging(directory::Entry* directory, table::Entry** tables)
    : directory_(directory), tables_(tables) {}

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
                delete[] tables_[i];
            }
        }
        delete[] tables_;
    }
    if (directory_ != nullptr) {
        delete[] directory_;
    }
}

const void* Paging::get_directory() const {
    return directory_;
}

Error Paging::map(const void* virtual_address, const void* physical_address,
                  const Flags& flags) {
    const size_t directory_offset = get_directory_offset(virtual_address);
    directory::Entry* const pde = &directory_[directory_offset];
    if (!directory::is_present(*pde)) {
        return WITH_LOCATION(
            "Can't map address because of non-present page table");
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

    return nullptr;
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
