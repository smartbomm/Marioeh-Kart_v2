#include <FlashStorage.h>

#define __fits_in_page_buffer(x) ((x) <= (FLASH_PAGE_SIZE - page_buffer_index))
#define __fits_in_flash(x) ((x) <= (FLASH_END - (uint32_t)next_free_address))
extern char __etext;
extern char __data_start;

#define FLASH_PAGE_SIZE 64
#define FLASH_ROW_SIZE (FLASH_PAGE_SIZE * 4)
#define FLASH_USER_START_ADDR ((uint32_t)&__etext + FLASH_ROW_SIZE - ((uint32_t)&__etext % FLASH_ROW_SIZE))
#define FLASH_END 0x0003FFFFu // last Flash Address

uint8_t page_buffer[FLASH_PAGE_SIZE];
uint8_t page_buffer_index = 0u;
void *next_free_address = 0u;
void *next_free_address_extern = 0u;


uint32_t flash_free_memory(void)
{
  uint32_t free = FLASH_END - FLASH_USER_START_ADDR;
  return free;
}

struct flashStorage_t
{
  uint32_t size;
  uint8_t data[FLASH_PAGE_SIZE];
};

void flash_init(void)
{
  next_free_address = FLASH_USER_START_ADDR;
  next_free_address_extern = FLASH_USER_START_ADDR;
  _erase_flash_row(next_free_address);
}

void *flash_allocateMemory(uint32_t size)
{
}

void *flash_write(const void *data, uint32_t size)
{
  void *address = next_free_address_extern;
  if(__fits_in_flash(size))
  {
    if(__fits_in_page_buffer(size))
    {
      address = _pageBuffer_write(data, size);
    }
    else
    {
      uint32_t remaining_page_buffer = FLASH_PAGE_SIZE - page_buffer_index;
      uint32_t number_of_whole_pages_to_write = remaining_page_buffer / FLASH_PAGE_SIZE;
      _pageBuffer_write(data, remaining_page_buffer);

    }
    return address;
  }
  uint32_t number_of_whole_pages_to_write = size / FLASH_PAGE_SIZE;
  uint8_t *dataBytes = (uint8_t *)data;
  void *address = next_free_address;
  uint8_t remaining_page_size = FLASH_PAGE_SIZE - page_buffer_index;
  memcpy(&page_buffer [page_buffer_index], dataBytes, min(remaining_page_size, size));

  
  if (next_free_address > FLASH_END)
  {
    return NULL;
  }

  memcpy(page_buffer, data, size);
  _flash_write_page((uint32_t)address, page_buffer);
  next_free_address += size;
  
}

void _erase_flash_row(uint32_t address)
{
  // Warten bis NVM bereit ist
  while (!NVMCTRL->INTFLAG.bit.READY)
    ;

  // Adresse in 16-Bit-Worten (da NVMCTRL->ADDR in Halbworten rechnet!)
  NVMCTRL->ADDR.reg = address / 2;

  // Erase-Kommando (Command: ER = 0x02)
  NVMCTRL->CTRLA.reg = NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_ER;

  // Warten bis fertig
  // while (!NVMCTRL->INTFLAG.bit.READY);
}

void _flash_write_page(uint32_t addr, const uint8_t *data)
{
  // Warten bis NVM bereit ist
  while (!NVMCTRL->INTFLAG.bit.READY);

  // In Page page_buffer schreiben (32-Bit-weise)
  volatile uint32_t *dst = (volatile uint32_t *)addr;
  const uint32_t *src = (const uint32_t *)data;

  for (int i = 0; i < FLASH_PAGE_SIZE / 4; i++)
  {
    dst[i] = src[i];
  }

  // Adresse in 16-bit-Worten
  NVMCTRL->ADDR.reg = addr / 2;

  // Page schreiben
  NVMCTRL->CTRLA.reg =
      NVMCTRL_CTRLA_CMDEX_KEY |
      NVMCTRL_CTRLA_CMD_WP;
      NVMCTRL->ADDR.reg = address + i;

  while (!NVMCTRL->INTFLAG.bit.READY);
}

uint8_t _flash_read_byte(uint32_t addr)
{
  return *(volatile uint8_t *)addr;
}
void* _pageBuffer_write(void* data, uint32_t size)
{
  memcpy(&page_buffer[page_buffer_index], data, size);
  page_buffer_index += size;
  uint32_t* address = next_free_address_extern;
  next_free_address_extern += size;
  return address;
}