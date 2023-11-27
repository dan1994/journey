#include "memory/allocation/block_heap.hpp"

namespace memory::allocation::block_heap {

constexpr uint8_t BLOCK_UNUSED = 0;
constexpr uint8_t BLOCK_USED = 1 << 0;
constexpr uint8_t BLOCK_FIRST = 1 << 1;
constexpr uint8_t BLOCK_LAST = 1 << 2;

static with_error<void *> malloc(block_heap *heap, size_t bytes);
static error free(block_heap *heap, const void *allocation);
static size_t divide_round_up(size_t a, size_t b);
static with_error<size_t> find_allocation_offset(block_heap *heap,
                                                 size_t blocks);
static void mark_blocks_as_used(block_heap *heap, size_t offset, size_t blocks);

block_heap make_block_heap(uint8_t *start, block_metadata *block_table,
                           size_t block_size, size_t blocks) {
    return block_heap{
        .malloc = malloc,
        .free = free,
        ._start = start,
        ._block_table = block_table,
        ._block_size = block_size,
        ._blocks = blocks,
    };
}

::memory::allocation::allocator make_allocator(block_heap *heap) {
    return ::memory::allocation::allocator{
        .self = heap,
        ._malloc = reinterpret_cast<MallocType>(malloc),
        ._free = reinterpret_cast<FreeType>(free),
    };
}

with_error<void *> malloc(block_heap *heap, size_t bytes) {
    if (bytes == 0) {
        return {nullptr, errors::make(WITH_LOCATION("can't allocate 0 bytes"))};
    }

    const size_t blocks = divide_round_up(bytes, heap->_block_size);
    auto [offset, error] = find_allocation_offset(heap, blocks);
    if (errors::set(error)) {
        errors::enrich(&error, "find allocation offset");
        return {nullptr, error};
    }

    mark_blocks_as_used(heap, offset, blocks);
    void *address = heap->_start + (heap->_block_size * offset);

    return {address, errors::nil()};
}

error free(block_heap *heap, const void *allocation) {
    const uint8_t *allocation_ = static_cast<const uint8_t *>(allocation);

    if (allocation_ < heap->_start ||
        allocation_ > heap->_start + (heap->_blocks * heap->_block_size)) {
        return errors::make(
            WITH_LOCATION("addrees to free is outside the heap"));
    }

    if ((allocation_ - heap->_start) % heap->_block_size != 0) {
        return errors::make(
            WITH_LOCATION("address to free is not aligned to block size"));
    }

    size_t offset = (allocation_ - heap->_start) / heap->_block_size;

    block_metadata *block = heap->_block_table + offset;
    if (!(*block & BLOCK_FIRST)) {
        return errors::make(
            WITH_LOCATION("first block in allocation isn't marked as first"));
    }

    while (!(*block & BLOCK_LAST)) {
        if (!(*block & BLOCK_USED)) {
            return errors::make(
                WITH_LOCATION("unused block in the middle of an allocation"));
        }

        *block = BLOCK_UNUSED;
        block++;
    }
    *block = BLOCK_UNUSED;

    return errors::nil();
}

size_t divide_round_up(size_t a, size_t b) {
    return (a + b - 1) / b;
}

with_error<size_t> find_allocation_offset(block_heap *heap, size_t blocks) {
    size_t blocks_found = 0;
    for (block_metadata *block = heap->_block_table;
         static_cast<size_t>(block - heap->_block_table) < heap->_blocks;
         block++) {
        if (*block & BLOCK_USED) {
            blocks_found = 0;
            continue;
        }

        blocks_found++;
        if (blocks_found == blocks) {
            const size_t current_block_offset = block - heap->_block_table;
            return {current_block_offset - (blocks - 1), errors::nil()};
        }
    }

    return {0, errors::make(WITH_LOCATION(
                   "no contiguous blocks of requested size found"))};
}

void mark_blocks_as_used(block_heap *heap, size_t offset, size_t blocks) {
    block_metadata *first = heap->_block_table + offset;
    block_metadata *last = heap->_block_table + offset + blocks - 1;

    for (block_metadata *block = first; block <= last; block++) {
        *block = BLOCK_USED;

        if (block == first) {
            *block |= BLOCK_FIRST;
        }

        if (block == last) {
            *block |= BLOCK_LAST;
        }
    }
}

}  // namespace memory::allocation::block_heap
