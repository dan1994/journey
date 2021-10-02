#include "std/new"

#include "memory/kernel_heap.hpp"

void* operator new(size_t count) {
    return kernel_heap->allocate(count);
}

void operator delete(void* ptr, size_t count) {
    kernel_heap->free(ptr);
}
