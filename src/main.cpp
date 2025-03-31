#include "SimpleUDP.h"
#include <math.h>
#include <Arduino_LSM6DS3.h>
#include <Wire.h>

const String carId = "Car1";  // hier kannst du die ID des Autos anpassen
float y, z;
float x=1;

void setup() {
  SUDP_beginn();
  IMU.begin();
//TODO: lade strecke
//loadfile(dateiname.csv)

}

void loop() {
  if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x, y, z);    
  }

    String message = carId + "," 
                   + "a/x=" + String(x) + "," 
                   + "a/y=" + String(y) + ","
                   + "a/z=" + String(z) + ","
                   + "t=" + String(millis());

    SUDP_send(message);
   delay(100);
}