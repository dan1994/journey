#pragma once

namespace memory::heap {

enum class HeapStatus {
    SUCCESS,
    NOT_ENOUGH_CONTIGUOUS_MEMORY,
    CANT_FREE_ADDRESS_OUT_OF_HEAP_RANGE,
    CANT_FREE_FROM_THE_MIDDLE_OF_AN_ALLOCATION,
    FREE_ENTRY_IN_THE_MIDDLE_OF_AN_ALLOCATION,
    CANT_ALLOCATE_ZERO_ENTRIES,
};

}  // namespace memory::heap