#include <stdint.h>


// Define length of buffer
#define RINGBUFFER_SIZE 30u
// define scaler acc
#define Scaler_Acc 256u
//define g
#define G 9.81 

struct common_buffer_data
{
    /* data */
    int16_t ringbuffer[RINGBUFFER_SIZE];      //Buffer for sensor Values
    uint8_t ringbuffer_index;                 //ringbuffer index with actual value
    uint8_t index_last_element;               //index with element which is kicked in next call
    int16_t kicked_value;                     //kicked element is stored here for moving average
    int32_t buffer_sum;                       //sum in moving average filter
    int32_t buffer_average;                   //average 
    uint32_t last_time;                       //needed for integration
    uint32_t current_time;                    //needed for Integration
    uint8_t index_for_integration;           //for calculation of dx


};


 struct common_buffer_data initialize_buffer (void) {
    struct common_buffer_data b1;
    b1.index_last_element = 0u;   // Starting with zero; values on this position will be replaced
    b1.ringbuffer_index = RINGBUFFER_SIZE-1;     // Initialized to last Element; needed for Moving average
    b1.index_for_integration = RINGBUFFER_SIZE-2; //Initialized to secondlast Element  for integration
    memset(b1.ringbuffer,0u,sizeof(b1.ringbuffer));
    b1.kicked_value=0u;
    b1.buffer_sum=0u;
    b1.buffer_average=0u;
    b1.last_time=0u;
    b1.current_time=0u;
    return b1;
}

void push_data_to_buffer (int32_t data, common_buffer_data* buffer){
    buffer->kicked_value=buffer->ringbuffer[buffer->index_last_element];
    buffer->ringbuffer[buffer->index_last_element] = data;
    buffer->index_last_element++;
    buffer->ringbuffer_index++;
    buffer->index_for_integration++;

    if (buffer -> last_time == 0u) {        // Fixing first Step from 0 to 460000000ms
        buffer -> last_time = accurateMillis() - 1u;
    }
    else {
        buffer->last_time = buffer->current_time;
    }

    buffer->current_time = accurateMillis();
    

    if (buffer->ringbuffer_index >= RINGBUFFER_SIZE) {
        buffer->ringbuffer_index = 0u;
    }
    if (buffer->index_last_element >= RINGBUFFER_SIZE) {
        buffer->index_last_element = 0u;
    }
    if (buffer->index_for_integration >= RINGBUFFER_SIZE) {
        buffer->index_for_integration = 0u;
    }
}

int32_t moving_average (common_buffer_data* buffer) {
    buffer->buffer_sum=buffer->buffer_sum-buffer->kicked_value+buffer->ringbuffer[buffer->ringbuffer_index];
    return buffer->buffer_sum;
}

uint32_t integration(common_buffer_data* buffer,uint32_t * speed, int32_t accel_linear) {
    uint32_t dt = buffer->current_time-buffer->last_time;
    int32_t a = accel_linear;
    *speed = *speed+a*dt;
    return dt;
}
double scaling (int32_t* buffer_sum)
{
    double scaled_value;
 
return scaled_value;
}



