#ifndef Data_h
#define Data_h
#include <stdint.h>

#define VERSION 0.1

typedef struct data {
    uint8_t track_section;
    uint32_t pos_lin;
    uint32_t pos_vec[3];
    int32_t speed_lin;
    int32_t speed_vec[3];
    int32_t accel_lin;
    int32_t accel_vec[3];
} odometerData_t;


#endif