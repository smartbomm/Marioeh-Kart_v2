#include <stdint.h>


// Define length of buffer
#define RINGBUFFER_SIZE 34u
// Define Zero-Border for Accelleration values
#define ZERO_MOVEMENT 2200
// define Scaler
#define SPEED_SCALER 14196
#define POSITION_SCALER 14196126u
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
    int32_t last_time;                       //needed for integration
    int32_t current_time;                    //needed for Integration
    int32_t merker_buffer_sum;                //needed for calculation of dx in 32 bit integration
    int32_t merker_speed;                    //needed for calculation of dx in 64 bit integration


};


 struct common_buffer_data initialize_buffer (void) {
    struct common_buffer_data b1;
    b1.index_last_element = 0u;   // Starting with zero; values on this position will be replaced
    b1.ringbuffer_index = RINGBUFFER_SIZE-1;     // Initialized to last Element; needed for Moving average
    memset(b1.ringbuffer,0u,sizeof(b1.ringbuffer));
    b1.kicked_value=0;
    b1.buffer_sum=0;
    b1.buffer_average=0;
    b1.last_time=0;
    b1.current_time=0;
    b1.merker_buffer_sum=0;
    b1.merker_speed=0;

    return b1;
}

void push_data_to_buffer (int32_t data, common_buffer_data* buffer){
    buffer->kicked_value=buffer->ringbuffer[buffer->index_last_element];
    buffer->ringbuffer[buffer->index_last_element] = -data;
    buffer->index_last_element++;
    buffer->ringbuffer_index++;

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
  
}

int32_t moving_average (common_buffer_data* buffer) 
{   buffer->merker_buffer_sum=buffer->buffer_sum;
    buffer->buffer_sum=buffer->buffer_sum-buffer->kicked_value+buffer->ringbuffer[buffer->ringbuffer_index];
    return buffer->buffer_sum;
}

int32_t integration_32bit(common_buffer_data* buffer,int32_t * speed, int32_t accel_linear) {
    buffer->merker_speed=*speed;
    int32_t dt = buffer->current_time-buffer->last_time;

    int32_t dx = buffer->buffer_sum-buffer->merker_buffer_sum;
    *speed = *speed+((dx*dt)/2)+(buffer->merker_buffer_sum*dt);
    //*speed = *speed+accel_linear*dt;
    return dx;
}

int32_t integration_64bit(common_buffer_data* buffer,uint64_t * position, int32_t speed_linear) {
    int32_t dt = buffer->current_time-buffer->last_time;
    int32_t dx = speed_linear-buffer->merker_speed;
    *position = *position + ((dx*dt)/2)+(buffer->merker_speed*dt);
    //*position = *position + speed_linear*dt;
    return dx;
}







