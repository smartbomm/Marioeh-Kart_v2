#include <FlashStorage.h>

#define __fits_in_page_buffer(x) ((x) <= (FLASH_PAGE_SIZE - _page_index))
#define __fits_in_flash(x) ((x) <= (FLASH_END - (uint32_t)next_free_address))
extern char __etext;
extern char __data_start;

#define FLASH_PAGE_SIZE 64
#define FLASH_ROW_SIZE (FLASH_PAGE_SIZE * 4)
#define FLASH_USER_START_ADDR ((uint32_t)&__etext + FLASH_ROW_SIZE - ((uint32_t)&__etext % FLASH_ROW_SIZE))
#define FLASH_END 0x0003FFFFu // last Flash Address

uint8_t _page_index = 0u;
uint8_t _row_index = 0u;
void *next_free_address = 0u;

void _erase_flash_row(void *address);
void _flash_write_page(void *addr);


void flash_init(void)
{
  next_free_address = (void*) FLASH_USER_START_ADDR;
}

void *flash_start()
{
  return FLASH_USER_START_ADDR;
}

void *flash_write(void *data, uint32_t size)
{
  if (__fits_in_flash(size))
  {
    // Erase the row if needed
    void *address = next_free_address;
    if (__fits_in_page_buffer(size))
    {
      if (_row_index == 0)
      {
        _erase_flash_row(next_free_address);
      }
      memcpy(next_free_address, data, size);
      next_free_address += size;
      _page_index += size;
      _row_index += size;
    }
    else
    {
      uint32_t remaining_page_buffer = FLASH_PAGE_SIZE - _page_index;
      memcpy(next_free_address, data, remaining_page_buffer);
      data = (uint8_t *)data + remaining_page_buffer;
      _flash_write_page(next_free_address);
      next_free_address += remaining_page_buffer;
      uint32_t number_of_whole_pages_to_write = size / FLASH_PAGE_SIZE;
      for (uint32_t i = 0; i < number_of_whole_pages_to_write; i++)
      {
        if (_row_index == 0)
        {
          _erase_flash_row(next_free_address);
        }
        memcpy(next_free_address, data, FLASH_PAGE_SIZE);
        data = (uint8_t *)data + FLASH_PAGE_SIZE;
        _flash_write_page(next_free_address);
        next_free_address += FLASH_PAGE_SIZE;
        _page_index = 0;
        _row_index += FLASH_PAGE_SIZE;
      }
      if (size % FLASH_PAGE_SIZE != 0)
      {
        if (_row_index == 0)
        {
          _erase_flash_row(next_free_address);
        }
        uint32_t remaining_data = size % FLASH_PAGE_SIZE;
        memcpy(next_free_address, data, remaining_data);
        next_free_address += remaining_data;
        _page_index = remaining_data;
        _row_index += remaining_data;
      }
    }
    return address;
  }
  return NULL;
}

uint32_t flash_free_memory(void)
{
  uint32_t free = FLASH_END - FLASH_USER_START_ADDR;
  return free;
}



void _erase_flash_row(void *address)
{
  // Warten bis NVM bereit ist
  while (!NVMCTRL->INTFLAG.bit.READY)
    ;

  // Adresse in 16-Bit-Worten (da NVMCTRL->ADDR in Halbworten rechnet!)
  NVMCTRL->ADDR.reg = (uint32_t)address / 2;

  // Erase-Kommando (Command: ER = 0x02)
  NVMCTRL->CTRLA.reg = NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_ER;

  // Warten bis fertig
  // while (!NVMCTRL->INTFLAG.bit.READY);
}

void _flash_write_page(void *addr)
{
  // Warten bis NVM bereit ist
  while (!NVMCTRL->INTFLAG.bit.READY)
    ;

  // Adresse in 16-bit-Worten
  NVMCTRL->ADDR.reg = (uint32_t)addr / 2;

  // Page schreiben
  NVMCTRL->CTRLA.reg =
      NVMCTRL_CTRLA_CMDEX_KEY |
      NVMCTRL_CTRLA_CMD_WP;

  while (!NVMCTRL->INTFLAG.bit.READY)
    ;
}

uint8_t _flash_read_byte(uint32_t addr)
{
  return *(volatile uint8_t *)addr;
}

void flash_flush()
{
  _flash_write_page(next_free_address);
  uint8_t size = FLASH_PAGE_SIZE - _page_index;
  next_free_address += size;
  _page_index = 0;
  _row_index += size;
}