#pragma once

#include <stddef.h>

namespace memory::paging {

constexpr size_t PAGE_SIZE_IN_BYTES = 4096;

// Whether usermode can access or only kernel.
enum class PriviledgeLevel : bool { KERNEL = false, USER = true };

// Whether the page can be written to or only read from.
enum class AccessType : bool { READ_ONLY = false, READ_WRITE = true };

// Whether the page is currently in memory.
enum class Present : bool { FALSE = false, TRUE = true };

}  // namespace memory::paging
