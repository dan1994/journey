#include "kernel/kernel.hpp"

#include <utility>

#include "interrupts/idt.hpp"
#include "logging/logger.hpp"
#include "memory/allocation/allocator.hpp"
#include "memory/allocation/block_heap.hpp"
#include "memory/layout.hpp"

with_error<kernel> make(allocator* heap) {
    kernel kernel{.heap = heap,
                  .boot_disk = {.bus = drivers::storage::ata::Bus::PRIMARY,
                                .port = drivers::storage::ata::Port::MASTER,
                                .mode = drivers::storage::ata::Mode::PIO}};

    interrupts::init();
    logging::debug("Initialized interrupts...");

    auto [paging, error] =
        memory::paging::make(kernel.heap,
                             {
                                 memory::paging::PriviledgeLevel::KERNEL,
                                 memory::paging::AccessType::READ_WRITE,
                                 memory::paging::Present::TRUE,
                             },
                             {
                                 memory::paging::PriviledgeLevel::KERNEL,
                                 memory::paging::AccessType::READ_WRITE,
                                 memory::paging::Present::TRUE,
                             });
    kernel.kernel_paging = paging;
    if (errors::set(error)) {
        errors::enrich(&error, "initialize paging");
        return {kernel, error};
    }

    memory::paging::load(paging);
    memory::paging::enable();
    logging::debug("Initialized paging...");

    return {kernel, errors::nil()};
}

error destroy(kernel* kernel) {
    memory::paging::disable();
    error err = memory::paging::destroy(&kernel->kernel_paging);
    if (errors::set(err)) {
        errors::enrich(&err, "destory paging");
        return err;
    }

    return errors::nil();
}
