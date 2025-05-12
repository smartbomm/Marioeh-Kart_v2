#include <MathFunctions.h>

struct integration
{
    uint32_t value;
    uint32_t last_value;
    uint32_t last_time;
};

void* integration_create(void)
{
    struct integration *f = (struct integration *)malloc(sizeof(struct integration));
    if (f != NULL)
    {
        f->value = 0;
        f->last_value = 0;
        f->last_time = 0;
    }
    return f;
}

void integration_update(void *integration_handler, int32_t value, uint32_t time)
{
    struct integration *f = (struct integration *)integration_handler;   
    if (f->last_time == 0)
    {
        f->last_time = time;
        f->last_value = value;
        f->value = 0;
    }
    else
    {
        uint32_t dt = time - f->last_time;
        if (dt > 0)
        {
            f->value += ((value - f->last_value) * dt);
            f->last_value = value;
            f->last_time = time;
        }
    }
}

uint32_t integration_get_value(void *integration_handler)
{
    struct integration *f = (struct integration *)integration_handler;  
    return f->value;
}

void integration_reset(struct integration *f)
{
    f->value = 0;
    f->last_value = 0;
    f->last_time = 0;
}
void integration_set_value(struct integration *f, int32_t value)
{
    f->value = value;
    f->last_value = value;
}