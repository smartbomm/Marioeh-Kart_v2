#include "SimpleNET.h"
#include "SimpleGET.h"
#include "OdometerData.h"
#include <Arduino.h>
#include <math.h>
#include <Arduino_LSM6DS3.h>
#include <Wire.h>
#include <stdint.h>

data acell1; 
float y, z, x;
float xg, yg, zg;
uint32_t u32_x, u32_y, u32_z;
uint32_t u32_xg, u32_yg, u32_zg;
String datei;

void setup() {
  delay(500);
  String systemTime = simpleGET("/t"); //initialize the sudp with system time
  Serial.begin(115200);
  Serial.println(systemTime);
  SUDP_beginn((stringToUint64(simpleGET("/t")))); //convert the string to uint64_t and start the SUDP with the system time //
  IMU.begin();
}

bool banana = false;
bool apple = false;

unsigned long previousMillis = 0;
const unsigned long interval = 10; // in Millisekunden

void loop() {
  static bool banana = false;
  static bool apple = false;


  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    acell1.accel_vec[0] = (int32_t)u32_x;
    acell1.accel_vec[1] = (int32_t)u32_y;
    acell1.accel_vec[2] = (int32_t)u32_z;
    acell1.speed_vec[0] = (int32_t)u32_xg;
    acell1.speed_vec[1] = (int32_t)u32_yg;
    acell1.speed_vec[2] = (int32_t)u32_zg;
    if (IMU.accelerationAvailable()) {
      banana = true;
      IMU.readAcceleration(x, y, z);
    }
  
    if (IMU.gyroscopeAvailable()) {
      apple = true;
      IMU.readGyroscope(xg, yg, zg);
    }
  
    if (banana) {
      u32_x = (uint32_t)(x * 512);
      u32_y = (uint32_t)(y * 512);
      u32_z = (uint32_t)(z * 512);
      banana = false;
    }
  
    if (apple) {
      u32_xg = (uint32_t)(xg * 32768.0 / 2000.0);
      u32_yg = (uint32_t)(yg * 32768.0 / 2000.0);
      u32_zg = (uint32_t)(zg * 32768.0 / 2000.0);

      apple = false;
    }
  

    SUDP_send(acell1);
  }
}
