#include "kernel/kernel.hpp"

#include <utility>

#include "interrupts/idt.hpp"
#include "logging/logger.hpp"

Kernel& Kernel::get_kernel() {
    if (!kernel_) {
        kernel_ = std::unique_ptr<Kernel>(new Kernel);
    }

    return *kernel_;
}

Kernel::Kernel()
    : boot_disk_(drivers::storage::ata::disk{
          .bus = drivers::storage::ata::Bus::PRIMARY,
          .port = drivers::storage::ata::Port::MASTER,
          .mode = drivers::storage::ata::Mode::PIO,
      }),
      kernel_paging_(initialize_kernel_paging()) {
    interrupts::init();
    logging::debug("Initialized interrupts...");

    memory::paging::load(kernel_paging_);
    memory::paging::enable_paging();
    logging::debug("Initialized paging...");
}

memory::paging::Paging Kernel::initialize_kernel_paging() {
    WithError<memory::paging::Paging> paging_err = memory::paging::Paging::make(
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

std::unique_ptr<Kernel> Kernel::kernel_{nullptr};
