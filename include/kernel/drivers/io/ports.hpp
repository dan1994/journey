#pragma once

#include <stdint.h>

/**
 * Convenient IO port access. Port numbers will be filled into the Port enum as
 * needed.
 */

namespace drivers::io {

enum class Port : uint16_t {
    MASTER_PIC_COMMAND = 0x20,
    MASTER_PIC_DATA,
    SLAVE_PIC_COMMAND = 0xa0,
    SLAVE_PIC_DATA,

    // Read/Write PIO data bytes
    PRIMARY_ATA_DATA = 0x1F0,
    // Used to retrieve any error generated by the last ATA command executed or
    // to control command specific interface features.
    PRIMARY_ATA_FEATURES_OR_ERROR,
    // Number of sectors to read/write (0 is a special value).
    PRIMARY_ATA_SELECTOR_COUNT,
    // This is CHS / LBA28 / LBA48 specific.
    PRIMARY_ATA_SELECTOR_NUMBER_OR_LBA_LOW,
    // Partial Disk Sector address.
    PRIMARY_ATA_CYLINDER_LOW_OR_LBA_MID,
    // Partial Disk Sector address.
    PRIMARY_ATA_CYLINDER_HIGH_OR_LBA_HIGH,
    // Used to select a drive and/or head. Supports extra address/flag bits.
    PRIMARY_ATA_DRIVE_OR_HEAD,
    // Used to read the current status or to send ATA commands to the device.
    PRIMARY_ATA_STATUS_OR_COMMAND,
    // A duplicate of the Status Register which does not affect interrupts, and
    // used to reset the bus or enable/disable interrupts.
    PRIMARY_ATA_ALTERNATE_STATUS_OR_DEVICE_CONTROL = 0x3F6,
    // Provides drive select and head select information.
    PRIMARY_ATA_DRIVE_ADDRESS,
};

/**
 * Read a single byte from the given port.
 * @param port The port to read from.
 * @return The value read from the port.
 */
uint8_t read_byte(Port port);

/**
 * Read a word (2 bytes) from the given port.
 * @param port The port to read from.
 * @return The value read from the port.
 */
uint16_t read_word(Port port);

/**
 * Write a single byte to the given port.
 * @param port The port to write to.
 * @param value The value to write to the port.
 */
void write_byte(Port port, uint8_t value);

/**
 * Write a word (2 bytes) to the given port.
 * @param port The port to write to.
 * @param value The value to write to the port.
 */
void write_word(Port port, uint16_t value);

}  // namespace drivers::io