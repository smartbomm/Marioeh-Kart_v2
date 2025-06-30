#include "DEBUGLib.h"

/*

static struct _debug_Memory
{
    uint8_t write_index;
    uint8_t read_index;
    debugMessage_t messages[SIZE_OF_DEBUG_MEMORY]; // Array to store debug messages
} debug_Memory;

debugMessage_t *_readDebugMessage()
{
    if (debug_Memory.write_index == debug_Memory.read_index) // If the memory is empty, return NULL
    {
        return NULL;
    }
    debugMessage_t *message = &debug_Memory.messages[debug_Memory.read_index];      // Get the message from the memory
    debug_Memory.read_index = (debug_Memory.read_index + 1) % SIZE_OF_DEBUG_MEMORY; // Increment the read index
    return message;                                                                 // Return the message
}

void _writeDebugMessage(const debugType_t *type, uint8_t subtype)
{
    if(type->debug_level > DEBUG_LEVEL) // If the debug level of the message is higher than the configured debug level, do not write the message
    {
        return; // Exit the function without writing the message
    }
    debug_Memory.messages[debug_Memory.write_index] = (debugMessage_t){micros(), type, subtype}; // Create a new debug message
    debug_Memory.write_index = (debug_Memory.write_index + 1) % SIZE_OF_DEBUG_MEMORY; // Increment the write index
    if (debug_Memory.write_index == debug_Memory.read_index)                          // If the memory is full, increment the read index
    {
        debug_Memory.read_index = (debug_Memory.read_index + 1) % SIZE_OF_DEBUG_MEMORY; // Increment the read index
    }
}

*/