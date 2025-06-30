#include <Arduino.h>
#include <BarcodeReader.h>
#include <PinConfig.h>


//This is an example sketch to test the barcode reader library
//It reads a barcode and prints the value and velocity to the serial monitor


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
    Serial.begin(115200);
    while(!Serial); // Warten, bis die serielle Verbindung hergestellt ist

    Serial.println("Barcode Reader Test");
    
    configure_extint();
    barcode_init(barcode_config);

    
}

void loop()
{
    barcode_error_t error = barcode_get(barcode_value, barcode_velocity);
    switch (error)
    {
    case NO_CODE_DETECTED:
        Serial.println("NO_CODE_DETECTED");
        break;
    case READING_IN_PROGRESS:
        Serial.println("READING_IN_PROGRESS");
        break;
    case READING_SUCCESSFUL:
        Serial.print("READING_SUCCESSFUL: ");
        Serial.print("Barcode value: ");
        Serial.print(barcode_value);
        Serial.print(", Velocity: ");
        Serial.println(barcode_velocity);
        break;
    case PHASE_MISMATCH_ERROR:
        Serial.print("PHASE_MISMATCH_ERROR");
        Serial.println();
        break;
    case TIMEOUT_ERROR:
        Serial.println("TIMEOUT_ERROR");
        break;
    }
    delay(500);
}