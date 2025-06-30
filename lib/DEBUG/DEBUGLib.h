#ifndef DEBUGLib_h
#define DEBUGLib_h

#include <Arduino.h>





//##########################################################################################################//
// ENUMS
//##########################################################################################################//

/**
 * @brief Levels of debug messages
 * 
 */
typedef enum _debugLevel_t
{
    DEBUG_LEVEL_ERROR = 1,
    DEBUG_LEVEL_WARNING = 2,
    DEBUG_LEVEL_INFO = 3,
    DEBUG_LEVEL_CUSTOM = 4
} debugLevel_t;

//##########################################################################################################//
// STRUCTURES
//##########################################################################################################//

typedef struct _debugType_t
{
    debugLevel_t debug_level; // Debug level of the message
    const char *name;         // Description of the debug type
    const char *subType[];    // Array of subtypes for the debug type
} debugType_t;

typedef struct _debugMessage_t
{
    uint32_t timestamp; // Timestamp of the debug message
    const debugType_t *type;  // Type of the debug message
    uint8_t subtype;
} debugMessage_t;



//##########################################################################################################//
// FUNCTION PROTOTYPES
//##########################################################################################################//
// Function to add a debug message to the memory
void _writeDebugMessage(const debugType_t *type, uint8_t subtype = 0);
debugMessage_t *_readDebugMessage();

#endif