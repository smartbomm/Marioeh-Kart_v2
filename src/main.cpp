#include <Arduino.h>
#include <BarCodeHw.h>

uint8_t myCounter = 0;

void myIsr()
{
    // This is a user-defined ISR that will be called when the timer expires
    myCounter++;
}

void tc3_test()
{

    pinMode(LED_BUILTIN, OUTPUT); // Set pin 13 as output for debugging (optional)
    // Initialize TC3 with a timeout of 1000 ms
    _tc3_init(1000000);

    // Set a user-defined ISR (optional)
    _tc3_set_isr(myIsr);

    // Start the timer
    _tc3_start();

    Serial.begin(9600);
    Serial.println("TC3 Test Started");

    while (1)
    {
        if (myCounter % 2)
        {
            digitalWrite(LED_BUILTIN, HIGH); // Turn on the LED
            Serial.print("ON ");
            Serial.print(myCounter);
            Serial.println(" times");
        }
        else
        {
            Serial.print("OFF ");
            Serial.print(myCounter);
            Serial.println(" times");
            digitalWrite(LED_BUILTIN, LOW); // Turn off the LED
            if (~(myCounter % 6))
            {
                Serial.println("Counter reset and timer restarted");
                _tc3_restart(); // Restart the timer every 6 counts
                if (~(myCounter % 15))
                {
                    Serial.println("Counter reset and timer stopped");
                    _tc3_stop();
                    Serial.println("TC3 Test Completed");
                    return;
                }
            }
        }
    }
}

int main() {
    tc3_test();
}