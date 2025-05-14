#include <stdint.h>


// Define length of buffer
#define RINGBUFFER_SIZE 10u

struct common_buffer_data
{
    /* data */
    int32_t ringbuffer[RINGBUFFER_SIZE];
    uint8_t ringbuffer_index;
    uint8_t index_last_element;
};


 struct common_buffer_data initialize_buffer (void) {
    struct common_buffer_data b1;
    b1.index_last_element = 0u;
    b1.ringbuffer_index = 0u;
    b1.ringbuffer;
    memset(b1.ringbuffer,0u,sizeof(b1.ringbuffer));    
    return b1;
}

void push_data_to_buffer (int32_t data, common_buffer_data buffer_data){
    buffer_data.ringbuffer[buffer_data.ringbuffer_index] = data;
    buffer_data.ringbuffer_index++;
    if (buffer_data.ringbuffer_index >= sizeof(buffer_data.ringbuffer)) {
        buffer_data.ringbuffer_index = 0u;

    }
}

int32_t moving_average (int32_t ringbuffer[]) {
    int32_t buffer_sum = 0;
    int32_t buffer_average = 0;
    for (int i=0;i=sizeof(ringbuffer);i++) {
        buffer_sum = buffer_sum + ringbuffer[i]; 
    }
    buffer_average = buffer_sum/sizeof(ringbuffer);
    return buffer_average;
}


