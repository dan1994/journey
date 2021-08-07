#pragma once

/**
 * Structures relevant to the IDT (Interrupt Descriptor Table).
 * See https://wiki.osdev.org/IDT for more info.
 */

#include <stddef.h>
#include <stdint.h>

typedef struct __attribute__((packed)) {
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
} IdtDescriptor;

// Structure of the IDT register
typedef struct __attribute__((packed)) {
    uint16_t limit;  // The length of the IDT - 1 in bytes
    uint32_t base;   // The address of the IDT in memory
} IdtRegister;

/**
 * Loads the IDT register.
 *
 * @param idtr Pointer to memory that contains the content to be loaded to the
 * IDT register.
 */
extern "C" void load_idt(const IdtRegister *idtr);
