#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include <stdint.h>
#include <Arduino_LSM6DS3.h>

#include "SimpleGET.h"
#include "OdometerData.h"
#include "Ringbuffer.h"


constexpr unsigned long READ_INTERVAL_MS = 0;     // Interval between readings


data sensorData;

int16_t accelX, accelY, accelZ;
int16_t gyroX, gyroY, gyroZ;

int32_t filteredAccelX, filteredAccelY, filteredAccelZ;
int32_t fixedGyroX, fixedGyroY, fixedGyroZ;
int32_t filtered_data_accel_x = 0, filtered_data_velocity_x = 0, filtered_data_pos_x = 0;
uint16_t banana = 0;

 //Ringbuffer defined in "ringbuffer.h"
struct common_buffer_data Struct_Accel_X  = initialize_buffer();
struct common_buffer_data Struct_Accel_Y  = initialize_buffer();
struct common_buffer_data Struct_Accel_Z  = initialize_buffer();

unsigned long previousMillis = 0;


void setup() {
  Serial.begin(9600);
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
      
      if (accelX > 0 & accelX < 2200u) {
        accelX =0u;
      }


    
      // Aktualisierung des Ringpuffers 
      push_data_to_buffer(accelX, &Struct_Accel_X);
      push_data_to_buffer(accelY, &Struct_Accel_Y);
      push_data_to_buffer(accelZ, &Struct_Accel_Z);
            
      // Auslesen der Filterwerte
      filteredAccelX = moving_average(&Struct_Accel_X);
      filteredAccelY = moving_average(&Struct_Accel_Y);
      filteredAccelZ = moving_average(&Struct_Accel_Z);
      if (filteredAccelX < 2200u) {
        filtered_data_velocity_x = 0u;
      }
      else {
        integration(&Struct_Accel_X, &filtered_data_velocity_x,filteredAccelX);
      }
    }

    if (gyroAvailable) {
      IMU.readGyroscope(gyroX, gyroY, gyroZ);


      // Evtl. auch hier Aktualierung des Ringpuffers für die Gyro-Werte
    }

  


    sensorData.accel_vec[0] = filteredAccelX;
    sensorData.accel_vec[1] = filteredAccelY;
    sensorData.accel_vec[2] = filteredAccelZ;

    sensorData.accel_lin = filteredAccelX;
    sensorData.speed_lin = filtered_data_velocity_x;
    sensorData.pos_lin = filtered_data_pos_x;
    sensorData.track_section = 1;
    if (banana > 1000) {
      banana = 0;
    }
    banana = banana+1;
    sensorData.pos_lin = banana;

    SUDP_send(sensorData);
  }
  
}
