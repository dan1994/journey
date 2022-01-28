#pragma once

#include "memory/paging/paging_instance.hpp"

namespace memory::paging {

/**
 * Enable paging in the processor.
 * WARNING: A page directory must be loaded before enabling paging. Otherwise
 * the system will panic.
 */
void enable_paging();

/**
 * Make the paging mechanism use the given paging instance.
 * @param instance The paging instance to use.
 */
void load(const PagingInstance& instance);

}  // namespace memory::paging
