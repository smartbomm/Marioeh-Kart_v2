#include <BarcodeReader.h>

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

struct barcodeReader_t
{
    barcodeConfig_t config;
    uint8_t pin;
    barcodePhase_t readingPhase;
    uint8_t bitCounter;
    bool readingInProgress;
    bool readingFinished;
    uint32_t lastTime;
    barCodeBit_t barcodeByte[8];
} barcodeReader;



void barcodeIsr()
{
    uint32_t actualTime = micros();
    if (barcodeReader.readingInProgress)
    {
        if (barcodeReader.readingPhase == PHASE_WHITE)
        {
            barcodeReader.barcodeByte[barcodeReader.bitCounter].whiteTime = actualTime - barcodeReader.lastTime;
            barcodeReader.readingPhase = PHASE_BLACK;
        }
        else
        {
            barcodeReader.barcodeByte[barcodeReader.bitCounter].blackTime = actualTime - barcodeReader.lastTime;
            barcodeReader.readingPhase = PHASE_WHITE;
            if (barcodeReader.bitCounter < 7)
            {
                barcodeReader.bitCounter++;
            }
            else
            {
                barcodeReader.bitCounter = 0;
                barcodeReader.readingFinished = true;
                barcodeReader.readingInProgress = false;
            }
        }
    }
    else if (barcodeReader.readingFinished)
    {
        return;
    }
    else
    {
        barcodeReader.readingInProgress = true;
    }
    barcodeReader.lastTime = actualTime;
}

void barcode_init(barcodeConfig_t config)
{
    barcodeReader.config = config;
    barcodeReader.config.bitLength *= 10000000u;
    barcodeReader.bitCounter = 0;
    barcodeReader.readingPhase = PHASE_WHITE;
    pinMode(config.pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(config.pin), barcodeIsr, CHANGE);
}

/**
 * @brief Read the value and the calculated velocity from a barcode
 * 
 * @param value Pointer variable to store the barcode value to
 * @param velocity Pointer variable to store the velocity value in nm/µs = µm/ms = mm/s
 * @return barcode_error_t returns READING_SUCCESSFULL if barcode has been read and READING_IN_PROGRESS if reading is already in process and NO_CODE_DETCTED if there hasn't been detected a code since the last call of the function
 */
barcode_error_t barcode_get(uint8_t* value, uint32_t* velocity)
{
    if (barcodeReader.readingFinished)
    {
        uint8_t barcodeValue = 0u;
        for (uint8_t i = 0; i < 8; i++)
        {
            barcodeValue = barcodeValue << 1;
            if (barcodeReader.barcodeByte[i].whiteTime > barcodeReader.barcodeByte[i].blackTime)
            {
                barcodeValue |= (1 << i);
            }
            else
            {
                barcodeValue &= ~(1 << i);
            }
        }
        barcodeReader.readingFinished = false;
        *velocity = (uint32_t) barcodeReader.config.bitLength / (barcodeReader.barcodeByte[7].blackTime + barcodeReader.barcodeByte [7].whiteTime);
        *value = barcodeValue;
        return READING_SUCCESSFUL;
    }
    else if (barcodeReader.readingInProgress)   // Not finished yet
    {
        return READING_IN_PROGRESS;
    }
    else 
    {
        return NO_CODE_DETECTED;
    }
}