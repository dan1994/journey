#include "std/new"

#include "memory/heap_status.hpp"
#include "memory/kernel_heap.hpp"

void* operator new[](size_t count) {
    return operator new(count);
}

void* operator new(size_t count) {
    memory::HeapStatus status;
    return kernel_heap->allocate(count, status);
}

void* operator new(size_t count, void* ptr) {
    return ptr;
}

void operator delete[](void* ptr) {
    operator delete(ptr, static_cast<size_t>(0));
}

void operator delete[](void* ptr, size_t count) {
    operator delete(ptr, count);
}

void operator delete(void* ptr, size_t count) {
    memory::HeapStatus status;
    kernel_heap->free(ptr, status);
}
