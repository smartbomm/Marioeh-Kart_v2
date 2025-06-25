#ifndef DEBUG_h
#define DEBUG_h

#include <Arduino.h>

//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT_HEX(x) Serial.print(x, HEX)
#define DEBUG_PRINT_HEXLN(x) Serial.println(x, HEX)
#define DEBUG_PRINT_BIN(x) Serial.print(x, BIN)
#define DEBUG_PRINT_LN_BIN(x) Serial.println(x, BIN)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT_HEX(x)
#define DEBUG_PRINT_HEXLN(x)
#define DEBUG_PRINT_BIN(x)
#define DEBUG_PRINT_LN_BIN(x)
#endif




#endif