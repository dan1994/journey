#include "interrupts/interrupts.hpp"

#include "drivers/vga3.hpp"
#include "entrypoint/config.hpp"
#include "interrupts/isr.hpp"
#include "interrupts/pic.hpp"
#include "std/type_traits.hpp"

extern "C" void load_idt(const IdtRegister *idtr);

IdtDescriptor Interrupts::interrupts[INTERRUPT_NUMBER] = {{0}};

void Interrupts::init() {
    const IdtRegister idtr = {.limit = sizeof(interrupts) - 1,
                              .base = reinterpret_cast<uint32_t>(interrupts)};

    load_idt(&idtr);

    ProgrammableInterruptController::remap(
        ProgrammableInterruptController::Id::MASTER, PIC_OFFSET);

    register_all();
}

void Interrupts::enable(Id id) {
    constexpr uint8_t ENABLE_PRESENT_BIT_MASK = 1 << 7;
    interrupts[static_cast<std::underlying_type_t<Id>>(id)].type_attribute |=
        ENABLE_PRESENT_BIT_MASK;
}

void Interrupts::disable(Id id) {
    constexpr uint8_t DISABLE_PRESENT_BIT_MASK =
        static_cast<uint8_t>(~(1 << 7));
    interrupts[static_cast<std::underlying_type_t<Id>>(id)].type_attribute &=
        DISABLE_PRESENT_BIT_MASK;
}

void Interrupts::register_all() {
    register_interrupt(Id::DIVIDE_BY_ZERO, isr_divide_by_zero,
                       PriviledgeLevel::KERNEL, GateSize::BITS32);

    register_interrupt(Id::PIC_KEYBOARD, isr_keyboard_press,
                       PriviledgeLevel::KERNEL, GateSize::BITS32);
}

void Interrupts::register_task(Id id, PriviledgeLevel dpl) {
    drivers::Vga3::print(
        "TSS selector not yet implemented, task will not be registered.\n");

    // constexpr Isr no_isr = nullptr;
    // register_internal(id, no_isr, dpl, GateType::TASK, GateSize::BITS16);
}

void Interrupts::register_interrupt(Id id, Isr isr, PriviledgeLevel dpl,
                                    GateSize size) {
    register_internal(id, isr, dpl, GateType::INTERRUPT, size);
}

void Interrupts::register_trap(Id id, Isr isr, PriviledgeLevel dpl,
                               GateSize size) {
    register_internal(id, isr, dpl, GateType::TRAP, size);
}

void Interrupts::register_internal(Id id, Isr isr, PriviledgeLevel dpl,
                                   GateType type, GateSize size) {
    // Enable by default
    constexpr uint8_t ACTIVE = 1;
    // According to the intel manual this bit is always 0.
    // See Part 6.11 in
    // https://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-vol-3a-part-1-manual.pdf
    constexpr uint8_t STORAGE_SEGMENT = 0;

    IdtDescriptor *const interrupt =
        &interrupts[static_cast<std::underlying_type_t<Id>>(id)];

    const uint32_t handler_address_uint32 = reinterpret_cast<uint32_t>(isr);
    interrupt->offset_lsb = handler_address_uint32 & 0xffff;
    interrupt->offset_msb = handler_address_uint32 >> 16;

    // All interrupts are executed in the kernel
    interrupt->selector = KERNEL_CODE_SELECTOR;

    interrupt->zero = 0;

    interrupt->type_attribute =
        ACTIVE << 7 |
        static_cast<std::underlying_type_t<PriviledgeLevel>>(dpl) << 5 |
        STORAGE_SEGMENT << 4 |
        static_cast<std::underlying_type_t<GateSize>>(size) << 3 |
        static_cast<std::underlying_type_t<GateType>>(type);
}
