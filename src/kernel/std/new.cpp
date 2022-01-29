#include "std/new"

#include "memory/heap/heap_status.hpp"
#include "memory/heap/kernel_heap.hpp"

// Temporary fix for vscode. For some reason vscode expects operator new to
// receive unsigned int instead of unsigned long, although the compiler expects
// unsigned long.
// Requires the following vscode configuration to work:
// "C_Cpp.default.mergeConfigurations" : true,
// "C_Cpp.default.defines": ["INTELLISENSE"]
#ifdef INTELLISENSE
#define size_t unsigned int
#endif

void* fix_array_offset(const void* ptr);

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
    operator delete(fix_array_offset(ptr), static_cast<size_t>(0));
}

void operator delete[](void* ptr, size_t count) {
    operator delete(fix_array_offset(ptr), count);
}

void operator delete(void* ptr, size_t count) {
    memory::heap::HeapStatus status;
    memory::heap::kernel_heap->free(ptr, status);
}

void* fix_array_offset(const void* ptr) {
    constexpr size_t ARRAY_DELETE_OFFSET = 4;
    return reinterpret_cast<void*>(reinterpret_cast<unsigned int>(ptr) +
                                   ARRAY_DELETE_OFFSET);
}

#ifdef INTELLISENSE
#undef size_t
#endif
