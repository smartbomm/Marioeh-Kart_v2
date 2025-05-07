MEMORY
{
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256K      /* Firmwarebereich */
  RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 32K      /* SAMD21 RAM */
}

/* Manuelle Konstanten für deine reservierten Datenbereiche */
__config_start      = 0x00010000;   /* 64 KiB */
__config_end        = 0x00010100;   /* +256 B */

__sectiontable_start = 0x00010100;  /* +768 B */
__sectiontable_end   = 0x00010400;

__map_start         = 0x00010400;   /* +191 KiB */
__map_end           = 0x00040000;   /* = 256 KiB total */
