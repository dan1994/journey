#pragma once

#include <stddef.h>
#include <stdint.h>

#include <type_traits>

#include "memory/allocation/allocator.hpp"
#include "utilities/error.hpp"

namespace memory::allocation::block_heap {

typedef uint8_t block_metadata;

struct block_heap {
    with_error<void *> (*malloc)(block_heap *self, size_t size);
    error (*free)(block_heap *self, const void *allocation);

    uint8_t *_start;
    block_metadata *_block_table;
    size_t _block_size;
    size_t _blocks;
};

block_heap make_block_heap(uint8_t *start, block_metadata *block_table,
                           size_t block_size, size_t blocks);

::memory::allocation::allocator make_allocator(block_heap *heap);

}  // namespace memory::allocation::block_heap
