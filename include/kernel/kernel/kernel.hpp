#pragma once

#include <memory>

#include "drivers/storage/ata.hpp"
#include "memory/allocation/allocator.hpp"
#include "memory/paging/paging.hpp"

class Kernel final {
   public:
    /**
     * Get the kernel singleton.
     */
    [[nodiscard]] static Kernel& get_kernel(allocator* allocator);

    /**
     * Dtor.
     */
    ~Kernel() = default;

    // Deleted functions
    Kernel(const Kernel&) = delete;
    Kernel(Kernel&&) = delete;
    Kernel& operator=(const Kernel&) = delete;
    Kernel& operator=(Kernel&&) = delete;

   private:
    /**
     * Ctor. Performs all kernel startup activities.
     */
    explicit Kernel(allocator* allocator);

    // The kernel singleton.
    static std::unique_ptr<Kernel> kernel_;

    [[nodiscard]] static memory::paging::Paging initialize_kernel_paging(
        allocator* allocator);

    constexpr static size_t MAX_NUMBER_OF_DISKS = 4;

    drivers::storage::ata::disk boot_disk_;
    memory::paging::Paging kernel_paging_;
};
