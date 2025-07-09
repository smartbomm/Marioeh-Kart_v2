/**
 * @file ringbuffer.h
 * @brief Beschreibung
 * @author JGesell
 * @date 
 */


#include <stdint.h>
#include "SimpleNET.h"
#include <Arduino.h>
#include <stdlib.h>



/// Define length of ringbuffer
#define RINGBUFFER_SIZE 34u
/// Define Zero-Border for Acceleration values in x and y, lower values will be set to 0
#define ZERO_MOVEMENT 380u
/// define Scaler calculation example is in documentation
#define SPEED_SCALER 14196
#define POSITION_SCALER 14196126u
/// define Zero-Border for contemplation of accel_y
#define ZERO_MOVEMENT_Y 40000u
/// define empiric factor for valueing accel_y in  turns
#define k     20000000u
/// define a amount of samples of motor voltage before a standstill is calculated
#define STANDSTILL  20u


struct common_buffer_data
{
    /* data */
    ///Buffer for acceleration data
    int16_t ringbuffer[RINGBUFFER_SIZE];      
    ///ringbuffer index with actual value
    uint8_t ringbuffer_index;                 
    ///index of element which is kicked out of buffer in next call
    uint8_t index_last_element;               
    ///kicked element is stored here for moving average filter
    int16_t kicked_value; 
    ///sum in moving average filter                   
    int32_t buffer_sum; 
    ///needed for timestamp in integration method                     
    uint32_t last_time;   
    ///needed for timestamp in integration method 
    uint32_t current_time;      
    ///needed for calculation of data points in 32 bit integration             
    int32_t merker_buffer_sum; 
    ///needed for calculation of data points in 64 bit integration              
    int32_t merker_speed;     
    ///needed for calculation of data points when acc_y is relevant                
    int32_t merker_accel_complete;   
    /// placeholder in 32 bit integration when the car drives through a curve         
    int32_t acc_complete;                     
};

///function for buffer creation an initalization
struct common_buffer_data initialize_buffer (void);

///function for storing values in the ringbuffer
///@param data acceleration 
///@param buffer pointer to struct for one direction
void push_data_to_buffer (int32_t data,struct common_buffer_data* buffer);

///function to apply a moving average filter on a ringpuffer
///@param buffer pointer to struct of acceleration
int32_t moving_average (struct common_buffer_data* buffer); 

///function to integrate speed out of acceleration
///@param buffer to struct of acceleration x
///@param speed pointer to speed value
///@param accel_linear acceleration in x
///@param accel_Y acceleration in y 
void integration_32bit(struct common_buffer_data* buffer,int32_t* speed, int32_t accel_linear,int32_t accel_Y); 

///function to integrate position out of speed
///@param buffer struct of accel_X
///@param speed pointer to position 
///@param speed_linear from integration
void integration_64bit(struct common_buffer_data buffer,uint64_t * position, int32_t speed_linear); 








