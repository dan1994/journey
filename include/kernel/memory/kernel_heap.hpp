#pragma once

#include "memory/heap.hpp"

// This is the global heap used by the kernel.
extern memory::Heap* kernel_heap;

/**
 * Initializes the kernel heap before any other global.
 *
 * @return A reference to the kernel heap.
 *
 * Why do we have this:
 * The heap is a global object and as such is initialized along with all the
 * other globals. But since we use gcc magic to generate the initialization code
 * for global objects we don't control the order of initialization.
 * This can be an issue if any other global is expecting the heap to be set up
 * to make allocations during initialization.
 * To overcome this we call this function before initializing globals and use a
 * local static variable to create the heap. Then we assign the global variable
 * to point to the local static.
 * We also return a reference to the variable so that we can provide a value to
 * initialize the global variable during the standard globals initialization
 * phase.
 */
extern "C" memory::Heap& get_kernel_heap();
