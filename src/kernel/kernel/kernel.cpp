#include "kernel/kernel.hpp"

#include <utility>

#include "interrupts/idt.hpp"
#include "logging/logger.hpp"

Kernel& Kernel::get_kernel(allocator* allocator) {
    if (!kernel_) {
        Kernel* const kernel =
            reinterpret_cast<Kernel*>(malloc(allocator, sizeof(Kernel)));
        kernel_ = std::unique_ptr<Kernel>(allocator, kernel);
    }

    return *kernel_;
}

Kernel::Kernel(allocator* allocator)
    : boot_disk_(drivers::storage::ata::disk{
          .bus = drivers::storage::ata::Bus::PRIMARY,
          .port = drivers::storage::ata::Port::MASTER,
          .mode = drivers::storage::ata::Mode::PIO,
      }),
      kernel_paging_(initialize_kernel_paging(allocator)) {
    interrupts::init();
    logging::debug("Initialized interrupts...");

    memory::paging::load(kernel_paging_);
    memory::paging::enable_paging();
    logging::debug("Initialized paging...");
}

memory::paging::Paging Kernel::initialize_kernel_paging(allocator* allocator) {
    WithError<memory::paging::Paging> paging_err = memory::paging::Paging::make(
        allocator,
        {
            memory::paging::PriviledgeLevel::KERNEL,
            memory::paging::AccessType::READ_WRITE,
            memory::paging::Present::TRUE,
        },
        {
            memory::paging::PriviledgeLevel::KERNEL,
            memory::paging::AccessType::READ_WRITE,
            memory::paging::Present::TRUE,
        },
        memory::paging::Paging::InitializationMode::LINEAR);

    return std::move(paging_err.first);
}

std::unique_ptr<Kernel> Kernel::kernel_{nullptr, nullptr};
