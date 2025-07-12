/**
 * @file BarcodeReader.h
 * @author Joel Bommeli (joel.bommeli@hof-university.de)
 * @brief The BarcodeReader is a software module written in C++ for the Arduino Nano 33 IOT development board. It enables reading the barcodes at the Carrera track with an IR line finder sensor. It realizes the following functions:
 * - Detecting the edges and levels of the binary sensor signal via interrupt
 * - Evaluate the edges and levels of the binary signal and calculate the barcode value
 * - Calculating the overrun car velocity out from the sensor reading times in the unit mm/s
 * - Detecting errors that can occur during reading the barcode
 * @version 1.0.5
 * @date 2025-07-09
 * 
 * 
 */

#ifndef BarcodeReader_h
#define BarcodeReader_h
#include <Arduino.h>


 /**
  * @brief Enum to define actual status from the barcode reader unit and errors that can occur during reading the barcode
  * 
  * 
  */
typedef enum
{
    NO_CODE_DETECTED,           /**< No code detected since last request */
    READING_SUCCESSFUL,         /**< Reading was successful, barcode value and velocity are available */
    READING_IN_PROGRESS,        /**< Reading is still in progress, no value available yet. No error was detected until now */
    PHASE_MISMATCH_ERROR,       /**< Phase mismatch error, the detected surface was black when it should be white or otherwise. */
    TIMEOUT_ERROR               /**< Timeout error, the interval between two detected edges was longer than the definde reading tieout. */
} barcode_error_t;

/**
 * @brief Configuration struct for the barcode reader
 * @param pin Digital input pin where the barcode reader is connected to
 * @param bitLength Length in µm of 1 bit (seuqence of black and white section)
 * @param readingTimeout Timeout in µs for the reading process
 */
typedef struct  {
    uint8_t pin;
    uint32_t bitLength;
    uint16_t readingTimeout;
} barcodeConfig_t;


/**
 * @brief Polling function read the value and the calculated velocity from a barcode. Has to be called repeatedly.
 * 
 * @param value Pointer variable to store the barcode value to
 * @param velocity Pointer variable to store the velocity value in nm/µs = µm/ms = mm/s
 * @return barcode_error_t actual status of the barcode reader or error that occurred during reading the barcode
 */
barcode_error_t barcode_get(uint8_t &value, uint32_t &velocity);

/**
 * @brief Initialize the pin configuration, the interrupt and the variables for the barcode reader
 * 
 * @param config config struct for configuration of the barcode reader
 */
void barcode_init(barcodeConfig_t config);

/**
 * @brief ISR function for the barcode reader
 * 
 * 
 */
void barcodeIsr(void);

#endif
