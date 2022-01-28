#include "memory/paging/paging.hpp"

extern "C" void enable_paging();

extern "C" void load_page_directory(const void* page_directory);

namespace memory::paging {

void enable_paging() {
    ::enable_paging();
}

void load(const PagingInstance& instance) {
    load_page_directory(instance.get_directory().entries());
}

}  // namespace memory::paging
