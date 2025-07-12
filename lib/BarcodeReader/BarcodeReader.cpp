#include <BarcodeReader.h>

//Macros

#define BARCODE_bits 8
#define BARCODE_edges (BARCODE_bits * 2 + 2)

#define DISABLE_EXTINT_7() EIC->INTENCLR.reg = EIC_INTENCLR_EXTINT3;
#define ENABLE_EXTINT_7() EIC->INTENSET.reg = EIC_INTENSET_EXTINT3;

//-----Private Enums-----//

// Enum to indicate the phases of a barcode bit
typedef enum
{
    PHASE_BLACK,
    PHASE_WHITE
} barcodePhase_t;

//-----Private Structs-----//

//struct to save the durations of the black and white phases of one barcode bit
typedef struct bcb
{
    uint32_t whiteTime;
    uint32_t blackTime;
} barCodeBit_t;

// struct to save all parameters and data of the barcode reader
struct barcodeReader_t
{
    barcodeConfig_t config;                     // Configuration struct for the barcode reader
    uint8_t pin;                                // Pin where the barcode reader is connected to
    volatile uint8_t bitCounter;                // Counter for the current barcode bit, 0-7
    volatile uint32_t lastTime;                 // Timestamp of the last edge in microseconds
    volatile barCodeBit_t barcodeByte[8];       // Array to store the durations of the black and white phases of each barcode bit
    volatile uint8_t edgeCounter;               // Counter for the number of edges detected, 0-18
    uint16_t readingTimeout;                    // Timeout in microseconds for the reading process
} barcodeReader;


//-----Private Variables-----//

// Variable to store the actual status from the barcode reader
barcode_error_t barcode_error = NO_CODE_DETECTED; // Variable to store the error code from the barcode reader

// Private Functions Prototypes

// Function to reset the internal counters of the barcode reader
void _reset_counters();

//Debugging function to print the barcode code and the phase durations and timestamps in the serial monitor
void printCode(volatile barCodeBit_t code[8])
{
    Serial.println("Bit | White | Black | Value");
    Serial.println("----|-------|-------|-------");
    for (uint8_t i = 0; i < 8; i++)
    {
        Serial.print(i);
        Serial.print("   | ");
        Serial.print(code[i].whiteTime);
        Serial.print(" | ");
        Serial.print(code[i].blackTime);
        Serial.print(" | ");
        if (code[i].whiteTime > code[i].blackTime)
        {
            Serial.println("1");
        }
        else
        {
            Serial.println("0");
        }
    }
}

void barcodeIsr()
{
    //Should be called on rising and falling edge of the pin

    barcodeReader.edgeCounter++;
    uint32_t actualTime = micros();     // actual system time

    // Validate Phase
    bool pinPhase = PORT->Group[PORTA].IN.reg & (1u << 7);  // Phase, the sensor is reading; Pin is static configured here to PA07!
    bool counterPhase = barcodeReader.edgeCounter % 2;      // Phase, the sensor should be reading; 0 = Black Phase, 1 = White Phase
    
    // Check if the pin phase matches the expected phase
    if (pinPhase != counterPhase)
    {
        // Phase mismatch, reset edge counter
        _reset_counters();
        barcode_error = PHASE_MISMATCH_ERROR;
        return;
    }

    // First edge of a barcode
    if (1 == barcodeReader.edgeCounter)
    {
        barcodeReader.lastTime = micros();
        return;
    }
    // All other edges of a barcode
    else if (barcodeReader.edgeCounter < 18u)
    {
        if (pinPhase) // Rising edge, Black Phase ending
        {
            barcodeReader.barcodeByte[barcodeReader.bitCounter].blackTime = actualTime - barcodeReader.lastTime;
            barcodeReader.bitCounter++;
        }
        else // Falling edge, White Phase ending
        {
            barcodeReader.barcodeByte[barcodeReader.bitCounter].whiteTime = actualTime - barcodeReader.lastTime;
        }
        barcodeReader.lastTime = actualTime;
        if (barcodeReader.edgeCounter == 18u)       // !!! This code has no effects. The enclosing else-if must be editetd. This wasn't done because this errors was first found during the documentations process and ther was no time to correct it.
        {
            DISABLE_EXTINT_7();
        }
    }
}

void barcode_init(barcodeConfig_t config)
{
    barcodeReader.config = config;
    barcodeReader.config.bitLength *= 1000u;
    barcodeReader.pin = config.pin;
    barcodeReader.readingTimeout = config.readingTimeout;
    barcodeReader.bitCounter = 0u;
    barcodeReader.edgeCounter = 0u;

    ENABLE_EXTINT_7();
}

barcode_error_t barcode_get(uint8_t &value, uint32_t &velocity)
{
    barcode_error_t reading_status;
    if (barcode_error != NO_CODE_DETECTED) // An error has been detected since last call (PHASE_MISMATCH_ERROR)
    {
        // Error has been detected, reset counters
        _reset_counters();
        reading_status = barcode_error;
        barcode_error = NO_CODE_DETECTED; // Reset error code
        ENABLE_EXTINT_7();
    }
    else if (barcodeReader.edgeCounter >= 18)       // No error occurred, but the barcode has been read completely
    {
        //Calculate the barcode value with comparing the durations of the black and white phases
        uint8_t barcodeValue = 0u;
        for (uint8_t i = 0u; i < 8u; i++)
        {
            if (barcodeReader.barcodeByte[i].whiteTime > (barcodeReader.barcodeByte[i].blackTime))
            {
                barcodeValue |= (0x80u >> i); // MSB first
            }
            else
            {
                barcodeValue &= ~(0x80u >> i); // MSB first
            }
        }
        //Calculate the velocity in µm/ms = mm/s
        velocity = (uint32_t)barcodeReader.config.bitLength / (barcodeReader.barcodeByte[7].blackTime + barcodeReader.barcodeByte[7].whiteTime);
        value = barcodeValue;
        _reset_counters();
        //Update BarcodeReader status
        reading_status = READING_SUCCESSFUL; 
    }
    else if (barcodeReader.edgeCounter == 0u) // No error occurred, but no code detected yet
    {
        reading_status = NO_CODE_DETECTED;
    }
    else // / No error occurred, but Not finished yet
    {
        //Check for timeouts
        uint32_t actualTime = micros();
        if (actualTime - barcodeReader.lastTime > barcodeReader.readingTimeout)
        {
            _reset_counters();
            reading_status = TIMEOUT_ERROR;
        }
        else
        {
            reading_status = READING_IN_PROGRESS;
        }
    }
    return reading_status;
}

/*********************************************************************************** */
// Private Functions
/*********************************************************************************** */
void _reset_counters()
{
    barcodeReader.bitCounter = 0u;
    barcodeReader.edgeCounter = 0u;
}