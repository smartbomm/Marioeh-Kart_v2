#include <BarcodeReader.h>

#define BARCODE_bits 8
#define BARCODE_edges (BARCODE_bits * 2 + 2)

#define DISABLE_EXTINT_7() EIC->INTENCLR.reg = EIC_INTENCLR_EXTINT3;
#define ENABLE_EXTINT_7() EIC->INTENSET.reg = EIC_INTENSET_EXTINT3;

typedef enum
{
    PHASE_BLACK,
    PHASE_WHITE
} barcodePhase_t;

typedef struct bcb
{
    uint32_t whiteTime;
    uint32_t blackTime;
} barCodeBit_t;

// Private Variables
barcode_error_t barcode_error = NO_CODE_DETECTED; //Variable to store the error code from the barcode reader

// Private Functions Prototypes
void _reset_counters();

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

struct barcodeReader_t
{
    barcodeConfig_t config;
    uint8_t pin;
    volatile uint8_t bitCounter;
    volatile uint32_t lastTime;
    volatile barCodeBit_t barcodeByte[8];
    volatile uint8_t edgeCounter; // every barcode has 18 edges, 9 rising, 9 falling ungerade Zahl: rising, gerade Zahl FALLING
    uint16_t readingTimeout;
} barcodeReader;

void barcodeIsr()
{
    DEBUG_PRINTLN("Barcode ISR triggered");
    
    barcodeReader.edgeCounter++;
    uint32_t actualTime = micros();

    // Validate Phase
    bool pinPhase = PORT->Group[PORTA].IN.reg & (1u << 7);
    bool counterPhase = barcodeReader.edgeCounter % 2; // 1 = White Phase, 0 = Black Phase
    if (pinPhase != counterPhase)
    {
        // Phase mismatch, reset edge counter
        _reset_counters();
        barcode_error = PHASE_MISMATCH_ERROR;
        DISABLE_EXTINT_7();
        DEBUG_PRINTLN("Phase MISMATCH ERROR");
        return;
    }
    

    // First edge
    if (1 == barcodeReader.edgeCounter)
    {
        barcodeReader.lastTime = micros();
        return;
    }
    //All other edges
    else if (barcodeReader.edgeCounter < 18u)
    {
        // Check for timeouts
        if ((actualTime - barcodeReader.lastTime) > barcodeReader.readingTimeout)
        {
            // Timeout error, reset edge counter
            _reset_counters();
            barcode_error = TIMEOUT_ERROR;
            DISABLE_EXTINT_7();
            DEBUG_PRINTLN("Timeout ERROR");
            return;
        }
        if (pinPhase) // Black Phase ending
        {
            barcodeReader.barcodeByte[barcodeReader.bitCounter].blackTime = actualTime - barcodeReader.lastTime;
            barcodeReader.bitCounter++;
        }
        else // White Phase ending
        {
            barcodeReader.barcodeByte[barcodeReader.bitCounter].whiteTime = actualTime - barcodeReader.lastTime;
        }
        barcodeReader.lastTime = actualTime;
        if (barcodeReader.edgeCounter == 18)
        {
            DISABLE_EXTINT_7();
            DEBUG_PRINTLN("Barcode reading finished");
        }
    }
}

void barcode_init(barcodeConfig_t config)
{
    barcodeReader.config = config;
    barcodeReader.config.bitLength *= 1000000u;
    barcodeReader.pin = config.pin;
    barcodeReader.readingTimeout = config.readingTimeout;
    barcodeReader.bitCounter = 0u;
    barcodeReader.edgeCounter = 0u;

    ENABLE_EXTINT_7();
}

barcode_error_t barcode_get(uint8_t &value, uint32_t &velocity)
{
    barcode_error_t reading_status;
    if (barcode_error != NO_CODE_DETECTED) // All errors
    {
        // Error has been detected, reset counters
        _reset_counters();
        reading_status = barcode_error;
        barcode_error = NO_CODE_DETECTED; // Reset error code
        ENABLE_EXTINT_7();
    }
    else if (barcodeReader.edgeCounter >= 18)
    {
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
        velocity = (uint32_t)barcodeReader.config.bitLength / (barcodeReader.barcodeByte[7].blackTime + barcodeReader.barcodeByte[7].whiteTime);
        value = barcodeValue;
        barcodeReader.bitCounter = 0;
        barcodeReader.edgeCounter = 0u;
        reading_status = READING_SUCCESSFUL; // Reading successful, reset counters and return value
    }
    else if (barcodeReader.edgeCounter == 0u) // No code detected
    {
        reading_status = NO_CODE_DETECTED;
    }
    else // Not finished yet
    {
        reading_status = READING_IN_PROGRESS;
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