#include "interrupts/interrupts.hpp"

#include <type_traits>

#include "drivers/vga3.hpp"
#include "entrypoint/config.hpp"
#include "interrupts/isr.hpp"

IdtDescriptor Interrupts::interrupts[INTERRUPT_NUMBER] = {{0}};

void Interrupts::init() {
    disable();

    const IdtRegister idtr = {.limit = sizeof(interrupts) - 1,
                              .base = reinterpret_cast<uint32_t>(interrupts)};

    load_idt(&idtr);

    drivers::Pic8259::init();

    register_all();

    enable();
}

void Interrupts::enable() {
    __asm__("sti;");
}

void Interrupts::disable() {
    __asm__("cli;");
}

void Interrupts::enable(Interrupt interrupt) {
    constexpr uint8_t ENABLE_PRESENT_BIT_MASK = 1 << 7;
    interrupts[static_cast<std::underlying_type_t<Interrupt>>(interrupt)]
        .type_attribute |= ENABLE_PRESENT_BIT_MASK;
}

void Interrupts::disable(Interrupt interrupt) {
    constexpr uint8_t DISABLE_PRESENT_BIT_MASK =
        static_cast<uint8_t>(~(1 << 7));
    interrupts[static_cast<std::underlying_type_t<Interrupt>>(interrupt)]
        .type_attribute &= DISABLE_PRESENT_BIT_MASK;
}

void Interrupts::register_all() {
    register_interrupt(static_cast<Interrupt>(0x20), isr_acknowledge_interrupt,
                       PriviledgeLevel::KERNEL, GateSize::BITS32);
    register_interrupt(static_cast<Interrupt>(0x76), isr_acknowledge_interrupt,
                       PriviledgeLevel::KERNEL, GateSize::BITS32);

    register_interrupt(Interrupt::DIVIDE_BY_ZERO, isr_divide_by_zero,
                       PriviledgeLevel::KERNEL, GateSize::BITS32);

    register_interrupt(Interrupt::PIC_KEYBOARD, isr_keyboard_press,
                       PriviledgeLevel::KERNEL, GateSize::BITS32);
}

void Interrupts::register_task(Interrupt interrupt, PriviledgeLevel dpl) {
    drivers::Vga3::print(
        "TSS selector not yet implemented, task will not be registered.\n");

    // constexpr Isr no_isr = nullptr;
    // register_internal(interrupt, no_isr, dpl, GateType::TASK,
    // GateSize::BITS16);
}

void Interrupts::register_interrupt(Interrupt interrupt, Isr isr,
                                    PriviledgeLevel dpl, GateSize size) {
    register_internal(interrupt, isr, dpl, GateType::INTERRUPT, size);
}

void Interrupts::register_trap(Interrupt interrupt, Isr isr,
                               PriviledgeLevel dpl, GateSize size) {
    register_internal(interrupt, isr, dpl, GateType::TRAP, size);
}

void Interrupts::register_internal(Interrupt interrupt, Isr isr,
                                   PriviledgeLevel dpl, GateType type,
                                   GateSize size) {
    // Enable by default
    constexpr uint8_t ACTIVE = 1;
    // According to the intel manual this bit is always 0.
    // See Part 6.11 in
    // https://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-vol-3a-part-1-manual.pdf
    constexpr uint8_t STORAGE_SEGMENT = 0;

    IdtDescriptor *const entry =
        &interrupts[static_cast<std::underlying_type_t<Interrupt>>(interrupt)];

    const uint32_t handler_address_uint32 = reinterpret_cast<uint32_t>(isr);
    entry->offset_lsb = handler_address_uint32 & 0xffff;
    entry->offset_msb = handler_address_uint32 >> 16;

    // All interrupts are executed in the kernel
    entry->selector = KERNEL_CODE_SELECTOR;

    entry->zero = 0;

    entry->type_attribute =
        ACTIVE << 7 |
        static_cast<std::underlying_type_t<PriviledgeLevel>>(dpl) << 5 |
        STORAGE_SEGMENT << 4 |
        static_cast<std::underlying_type_t<GateSize>>(size) << 3 |
        static_cast<std::underlying_type_t<GateType>>(type);
}
