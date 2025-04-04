//#include <samd21/include/samd21g18a.h>
#include <Arduino.h>
#include <BarcodeReader.h>
#include <OdometerData.h>

barcodeConfig_t config;

uint32_t speed;
uint8_t barcodeValue;
odometerData_t odometerData;

void setup()
{
  config.pin = 2u;
  config.bitLength = 7;
  barcode_init(config);
  Serial.begin(9600);
  Serial.println("START");
}

void loop()
{
  barcode_error_t success = barcode_get(&barcodeValue, &speed);
  if (READING_SUCCESSFUL == success)
  {
    char str[100];
    sprintf(str, "Barcode read:\n----------------\nValue: %d\nSpeed [mm/s]: %d\n\n", barcodeValue, speed);
    Serial.print(str);
  }
  else Serial.println(success);
  delay(1000);
}

/*int main() {

  REG_GCLK_CLKCTRL |= 0b0100000000001001u;
  REG_EIC_CTRL |= EIC_CTRL_ENABLE;  // Enable EIC
  REG_EIC_INTENSET |= EIC_EVCTRL_EXTINTEO0;  // Enable event output on pin 0
  REG_EIC_CONFIG0 |= EIC_CONFIG_SENSE0_BOTH;  // Set event on both edges
  REG_PORT_DIR0 = 1<<17;
  for(;;) {
    REG_PORT_OUT0 = 1<<17;
    for (int i = 0; i < 500000; i++) asm("nop");
    REG_PORT_OUT0 = 0;
    for (int i = 0; i < 500000; i++) asm("nop");

  }


  }*/