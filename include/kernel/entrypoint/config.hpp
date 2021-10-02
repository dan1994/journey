#pragma once

#include <stddef.h>

// These constants must match those used to set up the GDT.
constexpr size_t KERNEL_CODE_SELECTOR = 0x8;
constexpr size_t KERNEL_DATA_SELECTOR = 0x10;
