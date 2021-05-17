#include "kernel/interrupts/isr.hpp"

#include "kernel/drivers/vga3.hpp"

using namespace drivers;

extern "C" void isr_divide_by_zero(void *reserved) {
    Vga3::print("Divide by zero error!\n");
}
