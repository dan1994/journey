#pragma once

#include <cstddef>

namespace memory {

/**
 * Describes the general layout of memory as viewed by the kernel.
 */
enum class Layout {
    KERNEL_HEAP_ENTRY_TABLE = 0x7e00,
    VIDEO = 0xb8000,
    KERNEL = 0x100000,
    KERNEL_HEAP = 0x1000000
};

}  // namespace memory
