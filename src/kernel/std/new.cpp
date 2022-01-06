#include "std/new"

#include "memory/heap/heap_status.hpp"
#include "memory/heap/kernel_heap.hpp"

void* operator new[](size_t count) {
    return operator new(count);
}

void* operator new(size_t count) {
    memory::heap::HeapStatus status;
    return memory::heap::kernel_heap->allocate(count, status);
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
    memory::heap::HeapStatus status;
    memory::heap::kernel_heap->free(ptr, status);
}
