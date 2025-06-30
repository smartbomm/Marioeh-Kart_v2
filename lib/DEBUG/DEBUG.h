#ifndef DEBUG_h
#define DEBUG_h



//##########################################################################################################//
// SETTINGS FOR DEBUGGING LIBRARY
//##########################################################################################################//

//ENAEBLE OR DISABLE DEBUGGING
    //#define DEBUG_FILE
    #define DEBUG_IMMEDIATE
    //#define NO_DEBUG

#define SIZE_OF_DEBUG_MEMORY 100 // Size of the debug memory in messages    

// Set the debug level for the messages. Higher levels will include lower levels.
// NO_DEBUGGING = 0, DEBUG_LEVEL_ERROR = 1, DEBUG_LEVEL_WARNING = 2, DEBUG_LEVEL_INFO = 3, DEBUG_LEVEL_CUSTOM = 4
#define DEBUG_LEVEL 4

#ifdef DEBUG_FILE
#include "Macros.h"
#include "DEBUGLib.h"
#endif

//##########################################################################################################//
// INSERT ALL DEBUG-MESSAGE-TYPES HERE!
//##########################################################################################################//

#ifdef DEBUG_FILE

#define INIT_LIST_OF_DEBUG_TYPES() \
    ADD(example_error, DEBUG_LEVEL_ERROR, "ExampleError", Error1, Error2, Error3) \
    ADD(example_info, DEBUG_LEVEL_INFO, "ExampleInfo", Info1, Info2) \
    ADD(example_warning, DEBUG_LEVEL_WARNING, "ExampleWarning", Warning1, Warning2) \
    ADD(example_custom, DEBUG_LEVEL_CUSTOM, "ExampleCustom", Custom1, Custom2, Custom3)

#endif

//##########################################################################################################//
// MACROS
//##########################################################################################################//




#ifdef DEBUG_IMMEDIATE
// If DEBUG_IMMEDIATE is defined, we will use the Serial port for debugging
// This is useful for immediate feedback during development
// It will print debug messages to the Serial Monitor
// Make sure to call DEBUG_START() in your setup function
#define DEBUG_START()     \
    Serial.begin(115200); \
    while (!Serial)       \
        ; // Wait for serial port to connect. Needed for native USB
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT_HEX(x) Serial.print(x, HEX)
#define DEBUG_PRINT_HEXLN(x) Serial.println(x, HEX)
#define DEBUG_PRINT_BIN(x) Serial.print(x, BIN)
#define DEBUG_PRINT_LN_BIN(x) Serial.println(x, BIN)
#else
#define DEBUG_START()
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT_HEX(x)
#define DEBUG_PRINT_HEXLN(x)
#define DEBUG_PRINT_BIN(x)
#define DEBUG_PRINT_LN_BIN(x)
#endif

#ifdef DEBUG_FILE
INIT_LIST_OF_DEBUG_TYPES();
#define DEBUG(type, subtype) _writeDebugMessage(&type, subtype); 
#define DEBUG_PRINT_DATA() \
    debugMessage_t *message = _readDebugMessage(); \
    while (message != NULL) \
    { \
        Serial.print(message->timestamp); \
        Serial.print(" | "); \
        Serial.print(message->type->name); \
        Serial.print(", "); \
        Serial.println(message->type->subType[message->subtype]); \
        message = _readDebugMessage(); \
    }

#else
#define DEBUG(type, subtype)
#endif





#endif