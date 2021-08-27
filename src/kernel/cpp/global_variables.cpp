#include "cpp/global_variables.hpp"

#include <stddef.h>

extern void (*__ctors_array_start[])(int, char **, char **)
    __attribute__((visibility("hidden")));
extern void (*__ctors_array_end[])(int, char **, char **)
    __attribute__((visibility("hidden")));

extern "C" void initialize_global_variables() {
    constexpr int ARGC = 1;
    constexpr char **ARGV = nullptr;
    constexpr char **ENVP = nullptr;

    const size_t size = __ctors_array_end - __ctors_array_start;
    for (size_t i = 0; i < size; i++) {
        (*__ctors_array_start[i])(ARGC, ARGV, ENVP);
    }
}

extern void (*__dtors_array_start[])() __attribute__((visibility("hidden")));
extern void (*__dtors_array_end[])() __attribute__((visibility("hidden")));

extern "C" void finalize_global_variables() {
    size_t i = __dtors_array_end - __dtors_array_start;
    while (i-- > 0) {
        (*__dtors_array_start[i])();
    }
}
