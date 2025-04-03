#include "SimpleUDP.h"
#include "OdometerData.h"
#include <math.h>
#include <Arduino_LSM6DS3.h>
#include <Wire.h>
#include <stdint.h>

data acell1;
const String carId = "Car1"; 
float y, z, x;

void setup() {
  SUDP_beginn();
  IMU.begin();
//TODO: lade strecke
//loadfileTCP(dateiname.csv)

}

void loop() {
  if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x,y,z);    
  }
x=x*512;
y=y*512;
z=z*512;
  acell1.accel_vec[0] = (int32_t) x;
  acell1.accel_vec[1] = (int32_t) y;
  acell1.accel_vec[2] = (int32_t) z;

  //acell1.time = millis(); // TODO: implement time in the struct
  //acell1.carId = carId; // TODO: implement carId in the struct


    SUDP_send(acell1);
   delay(50);
}