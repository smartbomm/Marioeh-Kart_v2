#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include <stdint.h>
#include <Arduino_LSM6DS3.h>

#include "SimpleGET.h"
#include "OdometerData.h"
#include "Ringbuffer.h"


constexpr float ACCEL_SCALE = 512.0;              // Custom scaling factor for acceleration
constexpr float GYRO_SCALE = 32768.0 / 2000.0;    // Scale gyroscope data from dps to fixed-point
constexpr unsigned long READ_INTERVAL_MS = 50;     // Interval between readings


data sensorData;

float accelX, accelY, accelZ;
float gyroX, gyroY, gyroZ;

int32_t fixedAccelX, fixedAccelY, fixedAccelZ;
int32_t fixedGyroX, fixedGyroY, fixedGyroZ;
uint16_t banana = 0;

 //Ringbuffer defined in "ringbuffer.h"

unsigned long previousMillis = 0;


void setup() {
  Serial.begin(115200);
  uint64_t systemTime = 0;
  while (systemTime == 0) {
    systemTime = bytesToUint64_StringDigits(simpleGET("/t"));
}
  SUDP_beginn(systemTime);

  if (!IMU.begin()) {
    while (true);
  }
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= READ_INTERVAL_MS) {
    previousMillis = currentMillis;

    bool accelAvailable = IMU.accelerationAvailable();
    bool gyroAvailable = IMU.gyroscopeAvailable();

    if (accelAvailable) {
      IMU.readAcceleration(accelX, accelY, accelZ);

      fixedAccelX = static_cast<int32_t>(accelX * ACCEL_SCALE);
      fixedAccelY = static_cast<int32_t>(accelY * ACCEL_SCALE);
      fixedAccelZ = static_cast<int32_t>(accelZ * ACCEL_SCALE);
    
      // Aktualisierung des Ringpuffers 

    }

    if (gyroAvailable) {
      IMU.readGyroscope(gyroX, gyroY, gyroZ);

      fixedGyroX = static_cast<int32_t>(gyroX * GYRO_SCALE);
      fixedGyroY = static_cast<int32_t>(gyroY * GYRO_SCALE);
      fixedGyroZ = static_cast<int32_t>(gyroZ * GYRO_SCALE);

      // Evtl. auch hier Aktualierung des Ringpuffers für die Gyro-Werte
    }

  


    sensorData.accel_vec[0] = fixedAccelX;
    sensorData.accel_vec[1] = fixedAccelY;
    sensorData.accel_vec[2] = fixedAccelZ;

    sensorData.speed_vec[0] = fixedGyroX;
    sensorData.speed_vec[1] = fixedGyroY;
    sensorData.speed_vec[2] = fixedGyroZ;
    sensorData.track_section = 1;
    if (banana > 1000) {
      banana = 0;
    }
    banana = banana+10;
    sensorData.pos_lin = banana;

    SUDP_send(sensorData);
  }
}
