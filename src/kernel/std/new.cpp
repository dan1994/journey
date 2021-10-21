#include "std/new"

#include "memory/heap_status.hpp"
#include "memory/kernel_heap.hpp"

void* operator new(size_t count) {
    memory::HeapStatus status;
    return kernel_heap->allocate(count, status);
}

void operator delete(void* ptr, size_t count) {
    memory::HeapStatus status;
    kernel_heap->free(ptr, status);
}