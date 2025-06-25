#include <stdint.h>
#include <Arduino.h>
#include <cpio.h>


// Define deadzone of motor voltage
#define DEADZONE_MOTOR 55000u

//introduce variables 
uint16_t motor_voltage = 0u;

// Pin config for mmotor voltage
uint Motor = A6;


