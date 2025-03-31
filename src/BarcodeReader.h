#ifndef BarcodeReader_h
#define BarcodeReader_h
#include <Arduino.h>

/**
 * @brief Firmware component to read a barcode linear. 
 * The barcode has to be encoded timebased.
 * 
 * ##Encoding barcode 
 */

typedef enum
{
    NO_CODE_DETECTED,
    READING_IN_PROGRESS,
    READING_SUCCESSFUL
} barcode_error_t;

typedef struct  {
    uint8_t pin;
    uint32_t bitLength;
} barcodeConfig_t;

barcode_error_t barcode_get(uint8_t* value, uint32_t* velocity);
void barcode_init(barcodeConfig_t config);

#endif
