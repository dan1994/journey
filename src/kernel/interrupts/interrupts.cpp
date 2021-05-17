#include "kernel/interrupts/interrupts.hpp"

#include "kernel/entrypoint/config.hpp"

extern "C" void load_idt(const IdtRegister *idtr);

IdtDescriptor Interrupts::interrupts[INTERRUPT_NUMBER] = {{0}};

void Interrupts::init() {
    const IdtRegister idtr = {.limit = sizeof(interrupts) - 1,
                              .base = reinterpret_cast<uint32_t>(interrupts)};

    load_idt(&idtr);
}

void Interrupts::register_interrupt(uint8_t interrupt_number,
                                    Isr handler_address, PriviledgeLevel dpl,
                                    GateType gate_type) {
    // Enable by default
    constexpr uint8_t ACTIVE = 1;
    // According to the intel manual this bit is always 0.
    // See Part 6.11 in
    // https://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-vol-3a-part-1-manual.pdf
    constexpr uint8_t STORAGE_SEGMENT = 0;

    IdtDescriptor *const interrupt = &interrupts[interrupt_number];

    const uint32_t handler_address_uint32 =
        reinterpret_cast<uint32_t>(handler_address);
    interrupt->offset_lsb = handler_address_uint32 & 0xffff;
    interrupt->offset_msb = handler_address_uint32 >> 16;

    // All interrupts are executed in the kernel
    interrupt->selector = KERNEL_CODE_SELECTOR;

    interrupt->zero = 0;

    interrupt->type_attribute = ACTIVE << 7 | static_cast<uint8_t>(dpl) << 5 |
                                STORAGE_SEGMENT << 4 |
                                static_cast<uint8_t>(gate_type);
}

void Interrupts::enable(uint8_t interrupt_number) {
    constexpr uint8_t ENABLE_PRESENT_BIT_MASK = 1 << 7;
    interrupts[interrupt_number].type_attribute |= ENABLE_PRESENT_BIT_MASK;
}

void Interrupts::disable(uint8_t interrupt_number) {
    constexpr uint8_t DISABLE_PRESENT_BIT_MASK =
        static_cast<uint8_t>(~(1 << 7));
    interrupts[interrupt_number].type_attribute &= DISABLE_PRESENT_BIT_MASK;
}
