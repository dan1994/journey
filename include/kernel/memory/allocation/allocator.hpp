#pragma once

#include <stddef.h>
#include <stdint.h>

#include <type_traits>

#include "utilities/error.hpp"

namespace memory::allocation {

using MallocType = with_error<void *> (*)(void *self, size_t size);
using FreeType = error (*)(void *self, const void *allocation);

struct allocator {
    void *self;
    MallocType _malloc;
    FreeType _free;
};

void *malloc(allocator *self, size_t size);
with_error<void *> try_malloc(allocator *self, size_t size);
void free(allocator *self, const void *allocation);
error try_free(allocator *self, const void *allocation);

}  // namespace memory::allocation

using memory::allocation::allocator;
using memory::allocation::free;
using memory::allocation::malloc;
