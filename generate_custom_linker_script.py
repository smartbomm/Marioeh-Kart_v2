def generate_custom_linker_script(firmware_size, config_size, section_table_size, map_table_size, output_file="ldscripts/linker.ld"):
    FLASH_START = 0x00000000
    CONFIG_START = FLASH_START + firmware_size * 1024  
    SECTION_TABLE_START = CONFIG_START + config_size * 1024
    MAP_TABLE_START = SECTION_TABLE_START + section_table_size * 1024
    FLASH_END = 0x00040000  # 256 KiB Flash insgesamt

    FLASH_SIZE = (FLASH_END - FLASH_START) // 1024
    RAM_SIZE = 32  # KiB

    memory_definition = f"""
MEMORY
{{
    FLASH (rx) : ORIGIN = 0x{FLASH_START:08X}, LENGTH = {FLASH_SIZE}K
    RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = {RAM_SIZE}K
}}
"""

    section_definition = f"""
SECTIONS
{{
    .text :
    {{
        *(.text*)
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)
        KEEP(*(.init))
        KEEP(*(.fini))
    }} > FLASH

    .data : 
    {{
        *(.data*)
    }} > RAM AT > FLASH

    .bss (NOLOAD) :
    {{
        *(.bss*)
        *(COMMON)
    }} > RAM

    .config :
    {{
        KEEP(*(.config))
    }} > FLASH AT > 0x{CONFIG_START:08X}

    .section_table :
    {{
        KEEP(*(.section_table))
    }} > FLASH AT > 0x{SECTION_TABLE_START:08X}

    .map_table :
    {{
        KEEP(*(.map_table))
    }} > FLASH AT > 0x{MAP_TABLE_START:08X}

    .stack (COPY):
    {{
        . = ALIGN(8);
        __stack_start = .;
        . += 0x400;
        __stack_end = .;
    }} > RAM
}}
"""

    with open(output_file, "w") as file:
        file.write(memory_definition + section_definition)

    print(f"Linker-Skript '{output_file}' erfolgreich erstellt.")


# Beispiel-Aufruf mit Eingabe der Firmware-Größe

generate_custom_linker_script(firmware_size=64, config_size=1, section_table_size=1, map_table_size=190)
