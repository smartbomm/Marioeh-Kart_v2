#include <Arduino_LSM6DS3.h>
#include <Arduino.h>

int main() 
{
    IMU.begin();
    float x, y, z;
if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
}
    while(1)
    {

    }
}