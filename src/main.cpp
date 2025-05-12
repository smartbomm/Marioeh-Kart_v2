#include <Arduino_LSM6DS3.h>
#include <Arduino.h>
#include <MathFunctions.h>

integration_handler_t integration_handler;
int main() 
{
    integration_handler = integration_create();
    integration_handler->value = 0;
    IMU.begin();
    int16_t x, y, z;
if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
}
    while(1)
    {

    }
}