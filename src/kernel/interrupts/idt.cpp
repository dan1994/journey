#include "interrupts/idt.hpp"

#include <type_traits>

#include "entrypoint/config.hpp"
#include "interrupts/interrupts.hpp"
#include "interrupts/isr.hpp"

namespace interrupts {

struct __attribute__((packed)) IdtRegister {
    uint16_t limit;  // The length of the IDT - 1 in bytes
    uint32_t base;   // The address of the IDT in memory
};

}  // namespace interrupts

extern "C" void load_idt(const interrupts::IdtRegister *idtr);

namespace interrupts {

struct __attribute__((packed)) IdtDescriptor {
    uint16_t offset_lsb;     // Offset in memory
    uint16_t selector;       // Selector from the GDT
    uint8_t zero;            // Reserved
    uint8_t type_attribute;  // Equivalent to (assuming LSB->MSB):
    // uint8_t gate_type : 3;        // Task/Interrupt/Trap
    // uint8_t gate_size : 1;        // 16/32 bits
    // uint8_t storage_segment : 1;  // Always 0 according to manual
    // uint8_t dpl : 2;              // Ring from which can be invoked
    // uint8_t present : 1;          // Is it active?
    uint16_t offset_msb;  // Offset in memory
};

enum class GateType : uint8_t {
    TASK = 0x5,
    INTERRUPT,
    TRAP,
};

enum class GateSize : uint8_t { BITS16, BITS32 };

enum class PriviledgeLevel : uint8_t { KERNEL, RING_1, RING_2, USER };

using ServiceRoutine = void (*)(void *);

static void register_all();
static void register_task(Id interrupt, PriviledgeLevel dpl);
static void register_interrupt(Id interrupt, ServiceRoutine isr,
                               PriviledgeLevel dpl, GateSize size);
static void register_trap(Id interrupt, ServiceRoutine isr, PriviledgeLevel dpl,
                          GateSize size);
static void register_internal(Id interrupt, ServiceRoutine isr,
                              PriviledgeLevel dpl, GateType type,
                              GateSize size);

// Maximum is 512, but since the int command can only get an 8 bit operand,
// we can never invoke more than 256 interrupts.
constexpr static size_t INTERRUPT_NUMBER = 256;

static IdtDescriptor interrupt_table[INTERRUPT_NUMBER] = {{0}};

void init() {
    DISABLE_INTERRUPTS();

    const IdtRegister idtr = {
        .limit = sizeof(interrupt_table) - 1,
        .base = reinterpret_cast<uint32_t>(interrupt_table)};

    load_idt(&idtr);

    drivers::interrupts::Pic8259::init();

    register_all();

    ENABLE_INTERRUPTS();
}

void register_all() {
    register_interrupt(Id::PIC_TIMER, isr_acknowledge_interrupt,
                       PriviledgeLevel::KERNEL, GateSize::BITS32);
    register_interrupt(Id::PIC_HDD, isr_acknowledge_interrupt,
                       PriviledgeLevel::KERNEL, GateSize::BITS32);

    register_interrupt(Id::DIVIDE_BY_ZERO, isr_divide_by_zero,
                       PriviledgeLevel::KERNEL, GateSize::BITS32);

    register_interrupt(Id::PIC_KEYBOARD, isr_keyboard_press,
                       PriviledgeLevel::KERNEL, GateSize::BITS32);
}

void register_task(Id interrupt, PriviledgeLevel dpl) {
    constexpr ServiceRoutine no_isr = nullptr;
    register_internal(interrupt, no_isr, dpl, GateType::TASK, GateSize::BITS16);
}

void register_interrupt(Id interrupt, ServiceRoutine isr, PriviledgeLevel dpl,
                        GateSize size) {
    register_internal(interrupt, isr, dpl, GateType::INTERRUPT, size);
}

void register_trap(Id interrupt, ServiceRoutine isr, PriviledgeLevel dpl,
                   GateSize size) {
    register_internal(interrupt, isr, dpl, GateType::TRAP, size);
}

void register_internal(Id interrupt, ServiceRoutine isr, PriviledgeLevel dpl,
                       GateType type, GateSize size) {
    // Enable by default
    constexpr uint8_t ACTIVE = 1;
    // According to the intel manual this bit is always 0.
    // See Part 6.11 in
    // https://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-vol-3a-part-1-manual.pdf
    constexpr uint8_t STORAGE_SEGMENT = 0;

    IdtDescriptor *const entry =
        &interrupt_table[static_cast<std::underlying_type_t<Id>>(interrupt)];

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

}  // namespace interrupts
