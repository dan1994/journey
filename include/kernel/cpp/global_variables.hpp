#pragma once

/**
 * Initialize all global variables.
 * Walks the .ctors section that contains pointers to functions that initialize
 * global variables and calls them.
 */
extern "C" void initialize_global_variables();

/**
 * Finalize all global variables.
 * Walks the .dtors section that contains pointers to functions that finalize
 * global variables and calls them.
 */
extern "C" void finalize_global_variables();
