#include "drivers/vga3.hpp"
#include "interrupts/interrupts.hpp"
#include "logging/logger.hpp"

using namespace drivers;

extern "C" void main() {
    Vga3::clear();
    Logger::set_log_level(Logger::Level::INFO);

    Logger::info("Initializing Journey...");

    Interrupts::init();
    Logger::debug("Initialized interrupts...");
}
