#include <MathFunctions.h>

struct integration
{
    uint32_t value;
    uint32_t last_value;
    uint32_t last_time;
};

integration_handler_t integration_create(void)
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

void integration_update(integration_handler_t f, int32_t value, uint32_t time)
{
    struct integration *g = (struct integration *)f;   
    if (g->last_time == 0)
    {
        g->last_time = time;
        g->last_value = value;
        g->value = 0;
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

uint32_t integration_get_value(integration_handler_t f)
{
    struct integration *g = (struct integration *)f;  
    return g->value;
}

void integration_reset(integration_handler_t f) 
{
    f->value = 0;
    f->last_value = 0;
    f->last_time = 0;
}
void integration_set_value(integration_handler_t f, int32_t value)
{
    f->value = value;
    f->last_value = value;
}