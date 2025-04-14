#ifndef FlashStorage_h
#define FlashStorage_h

#ifdef ARDUINO_SAMD_NANO_33_IOT
#include <FlashStorage_SAMD.h>

uint32_t f = NVMCTRL_RWW_EEPROM_ADDR;
NVMCTRL

typedef struct
{
    uint16_t pos_x;
    uint16_t pos_y;
} position_t;
typedef struct
{
    uint8_t barcode;
    position_t *position;
} section_t;

typedef struct
{
    uint8_t version;
    uint8_t number_of_sections;
    uint8_t number_of_coordinates;
} map_config_t;

typedef struct
{
    bool valid;
    uint8_t version;
} flash_config_t;

enum config_valid_t
{
    NOT_VALID,
    VERSION_INVALID,
    VALID
};

FlashStorage(flash_config, flash_config_t);

FlashStorageClass<position_t> *track_map;
FlashStorageClass<map_config_t> map_config_storage;

config_valid_t map_config(map_config_t &config)
{
    config_valid_t returnValue;
    flash_config_t existingConfig;
    flash_config.read(existingConfig);
    if (!existingConfig.valid)
    {
        returnValue = NOT_VALID;
    }
    else if (existingConfig.version != config.version)
    {
        returnValue = VERSION_INVALID;
    }
    else if (existingConfig.valid)
    {
        return VALID;
    }
    existingConfig.valid = true;
    existingConfig.version = config.version;
    flash_config.write(existingConfig);
    return returnValue;
}

void pullConfig

#endif
#endif