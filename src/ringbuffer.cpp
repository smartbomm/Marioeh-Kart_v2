#include "ringbuffer.h"
#include <stdint.h>

 struct common_buffer_data initialize_buffer (void)
  {
    struct common_buffer_data b1;
    b1.index_last_element = 0u;   // Starting with zero; values on this position will be replaced
    b1.ringbuffer_index = RINGBUFFER_SIZE-1;     // Initialized to last Element; needed for Moving average
    memset(b1.ringbuffer,0u,sizeof(b1.ringbuffer));
    b1.kicked_value=0;
    b1.buffer_sum=0;
    b1.last_time=0u;
    b1.current_time=0u;
    b1.merker_buffer_sum=0;
    b1.merker_speed=0;
    b1.acc_complete=0;
    b1.merker_accel_complete=0;

    return b1;
}

void push_data_to_buffer (int32_t data, struct common_buffer_data* buffer)
{
    if (buffer != NULL) 
    {
        buffer->kicked_value=buffer->ringbuffer[buffer->index_last_element];
        buffer->ringbuffer[buffer->index_last_element] = -data; //-data because of Orientation of IMU brake should be negative
        buffer->index_last_element++;
        buffer->ringbuffer_index++;

        if (buffer -> last_time == 0u) 
        {        // Fixing first Step from 0 to 460000000ms
        buffer -> last_time = accurateMillis() - 1u;
        }
        else 
        {
        buffer->last_time = buffer->current_time;
        }

        buffer->current_time = accurateMillis();
    

        if (buffer->ringbuffer_index >= RINGBUFFER_SIZE) 
        {
        buffer->ringbuffer_index = 0u;
        }
        if (buffer->index_last_element >= RINGBUFFER_SIZE)  
        {
        buffer->index_last_element = 0u;
        }
  
    }
}

int32_t moving_average (struct common_buffer_data* buffer) 
{   
    if(buffer != NULL)  
    { 
        buffer->merker_buffer_sum=buffer->buffer_sum;
        buffer->buffer_sum=(buffer->buffer_sum)-(buffer->kicked_value)+(buffer->ringbuffer[buffer->ringbuffer_index]);
        return buffer->buffer_sum;
    }
    return 0;
}

void integration_32bit(struct common_buffer_data* buffer,int32_t* speed, int32_t accel_linear,int32_t accel_Y) 
{
    if ((buffer != NULL)&&(speed != NULL))
    {
        buffer->merker_accel_complete=buffer->acc_complete;  
        buffer->merker_speed= *speed;                        //merker werden überschrieben
        if ((abs(accel_Y)>=ZERO_MOVEMENT_Y)&&(*speed!=0))   //kurvenbetrachtung
        {
            buffer->acc_complete=accel_linear+(abs(accel_Y)*(k/(*speed)));
        }
        else 
        {
            buffer->acc_complete=accel_linear;  // fall für gerade
        }
        int32_t a1=buffer->merker_accel_complete;
        int32_t a4=buffer->acc_complete;
        int32_t a2=((2*a1)+a4)/3;
        int32_t a3=(a1+(2*a4))/3;
        int32_t dt = buffer->current_time-buffer->last_time;
        *speed = *speed+((a1+(2*a2)+(2*a3)+a4)/6)*dt;

    }
}

void integration_64bit(struct common_buffer_data buffer,uint64_t * position, int32_t speed_linear) 
{
    if (position != NULL)
    {
        int32_t dt = (buffer.current_time)-(buffer.last_time);
        int32_t v1 = buffer.merker_speed;
        int32_t v4 = speed_linear;
        int32_t v2 = ((2*v1)+v4)/3;
        int32_t v3 = (v1+(2*v4))/3;
        *position = *position+((v1+(2*v2)+(2*v3)+v4)/6)*dt;
    }
}








