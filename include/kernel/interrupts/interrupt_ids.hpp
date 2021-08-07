#pragma once

#include "interrupts/pic.hpp"

enum class Interrupt : uint8_t {
    DIVIDE_BY_ZERO,
    PIC_TIMER = ProgrammableInterruptController::MASTER_OFFSET,
    PIC_KEYBOARD,
    PIC_CASCADE,
    PIC_COM2,
    PIC_COM1,
    PIC_LPT2,
    PIC_FLOPPY,
    PIC_LPT1,
    PIC_CMOS_RTC = ProgrammableInterruptController::SLAVE_OFFSET,
    PIC_CGA,
    PIC_UNUSED_0,
    PIC_UNUSED_1,
    PIC_PS2,
    PIC_FPU,
    PIC_HDD,
    PIC_UNUSED_2,
};
