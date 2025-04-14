#ifndef TrackMap_h
#define TrackMap_h

#include <stdint.h>

struct coordinates_t {
    uint16_t pos_x;
    uint16_t pos_y;
};

struct section_t {
    uint8_t barcode;
    struct coordinates_t*  position;
};

struct map_config_t {
    uint8_t number_of_sections;
    uint32_t number_of_coordinates_t;
};

#endif