#pragma once

#include "drivers/storage/ata.hpp"
#include "memory/paging/paging.hpp"

struct kernel {
    allocator* heap;
    drivers::storage::ata::disk boot_disk;
    memory::paging::paging kernel_paging;
};

with_error<kernel> make(allocator* heap);
error destroy(kernel* kernel);
