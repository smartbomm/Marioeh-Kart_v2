#include "SimpleNET.h"
#include "SimpleGET.h"
#include "OdometerData.h"
#include <Arduino.h>
#include <math.h>
#include <Arduino_LSM6DS3.h>
#include <Wire.h>
#include <stdint.h>

data acell1;
const String carId = "Car1"; 
float y, z, x;
String datei;

void setup() {

  String systemTime = simpleGET("/t"); //initialize the sudp with system time
  Serial.begin(115200);
  Serial.println(systemTime);
  SUDP_beginn((stringToUint64(systemTime))); //convert the string to uint64_t and start the SUDP with the system time
  IMU.begin();

datei = simpleGET("/mario.csv");


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


   SUDP_send(acell1);
   delay(50);

}