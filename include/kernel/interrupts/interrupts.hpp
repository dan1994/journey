#pragma once

#define DISABLE_INTERRUPTS() __asm__("cli;")
#define ENABLE_INTERRUPTS() __asm__("sti;")
