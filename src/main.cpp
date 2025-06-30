#include <Arduino.h>
#include <BarcodeReader.h>
#include <MathFunctions.h>
#include <PinConfig.h>
#include <DEBUG.h>

barcodeConfig_t barcode_config = {
    .pin = PIN_PA07,          // Pin where the barcode reader is connected to
    .bitLength = 4,    // Length in mm of 1 bit (sequence of black and white section)
    .readingTimeout = 40000  // Timeout in µs for the reading process
};
uint8_t barcode_value = 0;
uint32_t barcode_velocity = 0;

uint8_t successCounter = 0;
uint8_t phaseErrorCounter = 0;
uint8_t timeoutCounter = 0;
uint8_t errorCounter = 0;

void EIC_Handler(void) {
    if (EIC->INTFLAG.reg & EIC_INTFLAG_EXTINT7) {
        EIC->INTFLAG.reg = EIC_INTFLAG_EXTINT7;  // Flag löschen
        barcodeIsr();  // Barcode ISR aufrufen
    }
}


void setup()
{
    DEBUG_START();
    Serial.begin(115200);
    
    configure_extint();
    barcode_init(barcode_config);
    DEBUG(example_error, (uint8_t) Error1);

    
}

void loop()
{
    barcode_error_t error = barcode_get(barcode_value, barcode_velocity);
    if(barcode_value == 31)
    {
        successCounter++;
        barcode_value = 0;
    }
    else if (error == PHASE_MISMATCH_ERROR)
    {
        phaseErrorCounter++;
        errorCounter++;
    }
    else if (error == TIMEOUT_ERROR)
    {
        errorCounter++;
        timeoutCounter++;;
    }
    if(!(errorCounter %20))
    {
        Serial.print("Success: ");
        Serial.print(successCounter);
        Serial.print(" | Phase Errors: ");
        Serial.print(phaseErrorCounter);
        Serial.print(" | Timeouts: ");
        Serial.println(timeoutCounter);
        errorCounter++;
    }
    delay(500);
}