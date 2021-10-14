#pragma once

#include "memory/heap.hpp"

extern memory::Heap* kernel_heap;

extern "C" memory::Heap& get_kernel_heap();
