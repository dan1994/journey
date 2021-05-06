ENTRY(_start)
OUTPUT_FORMAT(binary) /* Specify that the kernel is a flat binary */

SECTIONS {
    . = 1M; /* Load the kernel here */

    /*
     * Convert sections defined in all files to sections in the output file
     * Syntax <output_section> : { <files>(<input_section>) }
     * Use * to specify all files
     */
    .text : { *(.text) }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss : { *(COMMON) *(.bss) }
}
