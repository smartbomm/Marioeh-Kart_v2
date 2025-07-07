#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include <stdint.h>
#include <Arduino_LSM6DS3.h>

#include "SimpleGET.h"
#include "OdometerData.h"
#include "ringbuffer.h"
#include "Hardware_Config.h"
#include <BarcodeReader.h>

// Time intervals
constexpr unsigned long READ_INTERVAL_MS = 2;     // Interval between readings
constexpr unsigned long INTERVAL_STOP_COND = 100;

// Data struct to be sent
data sensorData;

// Incoming data from sensor in 16-bit format
int16_t accelX, accelY, accelZ;
int16_t gyroX, gyroY, gyroZ;

// Other variable definition
int32_t filteredAccelX, filteredAccelY, filteredAccelZ;
int32_t filtered_data_velocity_x = 0;
uint64_t filtered_data_pos_x = 0u;
uint8_t counter_sending = 0u;
int32_t acc_complete_for_debugging = 0;
uint32_t debugCount = 0u;
uint8_t counter_standstill = 0u;
uint8_t barcode_value = 0u;
uint32_t barcode_debug_velocity = 0u;

 //Ringbuffer defined in "ringbuffer.h"
struct common_buffer_data Struct_Accel_X  = initialize_buffer();
struct common_buffer_data Struct_Accel_Y  = initialize_buffer();
struct common_buffer_data Struct_Accel_Z  = initialize_buffer();
struct common_buffer_data Struct_Motor_Voltage = initialize_buffer();

// Timestamps
unsigned long previousMillis = 0;
uint32_t previousMillis_stop_cond = 0;

// Barcode 
void EIC_Handler(void) {
    if (EIC->INTFLAG.reg & EIC_INTFLAG_EXTINT7) {
        EIC->INTFLAG.reg = EIC_INTFLAG_EXTINT7;  // Flag löschen
        barcodeIsr();  // Barcode ISR aufrufen
    }
}

barcodeConfig_t barcode_config = {
    .pin = PIN_PA07,          // Pin where the barcode reader is connected to
    .bitLength = 9489,    // Length in µm of 1 bit (sequence of black and white section)
    .readingTimeout = 40000  // Timeout in µs for the reading process
};


void setup()
 {
  Serial.begin(9600);
  uint64_t systemTime = 0;
  while (systemTime == 0) {
    systemTime = bytesToUint64_StringDigits(simpleGET("/t"));
  }
  SUDP_beginn(systemTime);
  if (!IMU.begin()) {
    while (true);
  }
  configure_extint();
  barcode_init(barcode_config);
}

void loop() 
{
       unsigned long currentMillis = accurateMillis();
debugCount = micros();

  if (currentMillis - previousMillis >= READ_INTERVAL_MS) 
  {
    previousMillis = currentMillis;

    if (IMU.readAcceleration(accelX, accelY, accelZ)) 
    {
      if (abs(accelX)<ZERO_MOVEMENT)
      {
        accelX =0u;
      }     
      // zero movemnet auch für y
      if (abs(accelY)<ZERO_MOVEMENT)
      {
        accelY =0u;
      }     

      // Aktualisierung des Ringpuffers 
      push_data_to_buffer(accelX, &Struct_Accel_X);
      push_data_to_buffer(accelY, &Struct_Accel_Y);
      push_data_to_buffer(accelZ, &Struct_Accel_Z);

      push_data_to_buffer(analogRead(Motor), &Struct_Motor_Voltage);
      // Auslesen der Filterwerte
      filteredAccelX = moving_average(&Struct_Accel_X) ;
      filteredAccelY = moving_average(&Struct_Accel_Y) ;
      filteredAccelZ = moving_average(&Struct_Accel_Z) ;
      motor_voltage = moving_average(&Struct_Motor_Voltage);

    
      // Speed processing with check against hardwired logic over a short time intervall
      if ((motor_voltage > DEADZONE_MOTOR) && (filteredAccelX == 0)) 
      {
          counter_standstill++;
      }
      else 
      {
        counter_standstill = 0;
      }
      
      if (counter_standstill >= STANDSTILL) 
      {
        filtered_data_velocity_x = 0;
        counter_standstill = 0u;
      }
      else
      {
        //integration of speed out of filtered x and y acceleration
        acc_complete_for_debugging = integration_32bit(&Struct_Accel_X, &filtered_data_velocity_x, filteredAccelX,filteredAccelY);
      }
      
    
      //integration of position from intgrated speed; incrementation of send counter
       integration_64bit(Struct_Accel_X, &filtered_data_pos_x, filtered_data_velocity_x);
       counter_sending++;
    }


//reading of Gyroscope values and store them in gyroX,gyroY,gyroZ
    if (IMU.readGyroscope(gyroX, gyroY, gyroZ)) 
    {
    


      // Evtl. auch hier Aktualierung des Ringpuffers für die Gyro-Werte
    }
      
   debugCount=micros()-debugCount;
    Serial.println(debugCount);

  // Barcode recognition
    barcode_error_t error = barcode_get(barcode_value, barcode_debug_velocity);
    if (error == READING_SUCCESSFUL){
      // set back position to zero when entering a new track section
      filtered_data_pos_x = 0;
      //take the velocity from the barcode reader to prevent integration problems
      filtered_data_velocity_x=(int32_t)(barcode_debug_velocity*SPEED_SCALER);

    }
    // after 20 programcycles actual values are send via udp to the mqtt bridge
if (counter_sending>=20) 
{

    sensorData.accel_vec[0] = accelX;  //unfiltered acceleration x
    sensorData.accel_vec[1] = accelY;  //unfiltered acceleration y
    sensorData.accel_vec[2] = accelZ;  //unfiltered acceleration z
    sensorData.gyro_vec[0] = gyroX;    //unfiltered gyro x
    sensorData.gyro_vec[1] = gyroY;    //unfiltered gyro y
    sensorData.gyro_vec[2] = gyroZ;    //unfiltered gyro z
    sensorData.error_code = error; // 0 as standard --> machts was ihr wollt damit


    sensorData.accel_lin = filteredAccelX;   //acceleration linear 
    sensorData.speed_lin = filtered_data_velocity_x/SPEED_SCALER;    //speed linear in mm/s
    sensorData.pos_lin = (uint32_t)(filtered_data_pos_x/POSITION_SCALER); // account for Integration error

    sensorData.track_section = barcode_value;
   

    // Debug Motor Voltage
    Serial.print(motor_voltage);
    

    SUDP_send(sensorData);
    counter_sending=0U;

}
  }
  
}
