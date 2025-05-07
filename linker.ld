
MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 32K
}

SECTIONS
{
    .text :
    {
        *(.text)
        *(.rodata)
    } > FLASH

    .data :
    {
        *(.data)
    } > RAM AT > FLASH

    .bss :
    {
        *(.bss)
    } > RAM

    .config : { *(.config) } > FLASH AT > 0x00030000 LIMITS 1K :
    {
        *(.config : { *(.config) } > FLASH AT > 0x00030000 LIMITS 1K)
    } > FLASH AT > 0x00030000

    .section_table : { *(.section_table) } > FLASH AT > 0x00040000 LIMITS 1K :
    {
        *(.section_table : { *(.section_table) } > FLASH AT > 0x00040000 LIMITS 1K)
    } > FLASH AT > 0x00040000

    .map_table : { *(.map_table) } > FLASH AT > 0x00050000 LIMITS 190K :
    {
        *(.map_table : { *(.map_table) } > FLASH AT > 0x00050000 LIMITS 190K)
    } > FLASH AT > 0x00050000

    .stack (COPY):
    {
        . = ALIGN(8);
        __stack_start = .;
        . += 0x400;
        __stack_end = .;
    } > RAM
}
