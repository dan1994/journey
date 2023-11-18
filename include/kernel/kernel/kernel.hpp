#pragma once

#include "drivers/storage/ata.hpp"
#include "memory/allocation/block_heap.hpp"
#include "memory/paging/paging.hpp"

struct kernel {
    memory::allocation::block_heap::block_heap heap_implementation;
    allocator heap;
    drivers::storage::ata::disk boot_disk;
    memory::paging::Paging kernel_paging;
};

kernel make();
