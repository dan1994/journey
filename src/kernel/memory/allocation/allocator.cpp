#include "memory/allocation/allocator.hpp"

#include <cassert>

namespace memory::allocation {

void *malloc(allocator *self, size_t size) {
    auto [allocation, error] = self->_malloc(self->self, size);
    assertm(!error, "failed to allocate memory");
    return allocation;
}

WithError<void *> try_malloc(allocator *self, size_t size) {
    return self->_malloc(self->self, size);
}

void free(allocator *self, const void *allocation) {
    Error error = self->_free(self->self, allocation);
    assertm(!error, "failed to free memory");
}

Error try_free(allocator *self, const void *allocation) {
    return self->_free(self->self, allocation);
}

}  // namespace memory::allocation
