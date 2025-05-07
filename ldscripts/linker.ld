MEMORY
{
    FLASH           (rx) : ORIGIN = 0x00000000, LENGTH = 64K
    CONFIG_FLASH    (rx) : ORIGIN = 0x00010000, LENGTH = 1K
    TABLE_FLASH     (rx) : ORIGIN = 0x00010400, LENGTH = 1K
    MAP_FLASH       (rx) : ORIGIN = 0x00010800, LENGTH = 190K
    RAM             (rwx): ORIGIN = 0x20000000, LENGTH = 32K
}

SECTIONS
{
    .text :
    {
        *(.text*)
        *(.rodata*)
        KEEP(*(.init))
        KEEP(*(.fini))
    } > FLASH

    .data : 
    {
        *(.data*)
    } > RAM AT > FLASH

    .bss (NOLOAD) :
    {
        *(.bss*)
        *(COMMON)
    } > RAM

    .config :
    {
        KEEP(*(.config))
    } > CONFIG_FLASH

    .section_table :
    {
        KEEP(*(.section_table))
    } > TABLE_FLASH

    .map_table :
    {
        KEEP(*(.map_table))
    } > MAP_FLASH

    .stack (COPY):
    {
        . = ALIGN(8);
        __stack_start = .;
        . += 0x400;
        __stack_end = .;
    } > RAM
}
