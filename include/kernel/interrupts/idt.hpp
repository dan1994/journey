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
    uint8_t type_attribute;  // {Present, DPL (Descriptor Priviledge Level),
                             // Storage Segment, Gate Type}
    uint16_t offset_msb;     // Offset in memory
} IdtDescriptor;

// Structure of the IDT register
typedef struct __attribute__((packed)) {
    uint16_t limit;  // The length of the IDT - 1 in bytes
    uint32_t base;   // The address of the IDT in memory
} IdtRegister;
