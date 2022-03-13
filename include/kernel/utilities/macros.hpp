#pragma once

/**
 * Allow stringifying macros such as __LINE__.
 * Due to the way macros work, two layers are required.
 */
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
