#pragma once

#include "memory/paging/page_directory.hpp"

extern "C" void enable_paging();

extern "C" void load_page_directory(const void* page_directory);

namespace memory::paging {}  // namespace memory::paging
