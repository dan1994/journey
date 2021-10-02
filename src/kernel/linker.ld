ENTRY(_start)
OUTPUT_FORMAT(binary) /* Specify that the kernel is a flat binary */

SECTIONS {
    . = 1M; /* Load the kernel here */

    /*
     * Convert sections defined in all files to sections in the output file
     * Syntax <output_section> : { <files>(<input_section>) }
     * Use * to specify all files
     */
    .start : ALIGN(4096) { *(.start) } /* Make sure the first code that is run is our start section */
    .text : ALIGN(4096) { *(.text) }
    .rodata : ALIGN(4096) { *(.rodata) }
    .data : ALIGN(4096) { *(.data) }
    .bss : ALIGN(4096) { *(COMMON) *(.bss) }
    .asm : ALIGN(4096) { *(.asm) } /* Avoid C code alignment issues by putting most assembly code down here */
}
