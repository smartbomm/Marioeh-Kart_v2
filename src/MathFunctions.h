#ifndef MathFunctions_h
#define MathFunctions_h
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct integration;

typedef struct integration integration_handler_t;

integration_handler_t integration_create(void);

void integration_update(void *f, uint32_t value, uint32_t time);
uint32_t integration_get_value(void *f);
void integration_reset(void *f);
void integration_set_value(void *f, uint32_t value);

#ifdef __cplusplus
}
#endif
#endif