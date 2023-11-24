#include "utilities/error.hpp"

#include "drivers/display/vga3.hpp"

namespace errors {

error nil() {
    return make(nullptr);
}

error make(const char* message) {
    error error;

    error.messages[0] = message;
    for (size_t i = 1; i < MAX_DEPTH; i++) {
        error.messages[i] = nullptr;
    }

    return error;
}

bool set(error error) {
    return error.messages[0] != nullptr;
}

void enrich(error* error, const char* message) {
    for (size_t i = 0; i < MAX_DEPTH; i++) {
        if (error->messages[i] == nullptr) {
            error->messages[i] = message;
            return;
        }
    }
}

void log(error error) {
    if (!set(error)) {
        return;
    }

    drivers::display::vga3::print("ERROR: ",
                                  drivers::display::vga3::Color::RED);

    for (int i = MAX_DEPTH - 1; i >= 0; i--) {
        if (error.messages[i] != nullptr) {
            drivers::display::vga3::print(error.messages[i]);
            if (i != 0) {
                drivers::display::vga3::print(": ");
            }
        }
    }

    drivers::display::vga3::print("\n");
}

}  // namespace errors
