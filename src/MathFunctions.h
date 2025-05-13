#ifndef MathFunctions_h
#define MathFunctions_h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct integration;
typedef struct integration* integration_handler_t;

integration_handler_t integration_create(void);

void integration_update(integration_handler_t f, int32_t value, uint32_t time);
uint32_t integration_get_value(integration_handler_t f);
void integration_reset(integration_handler_t f);
void integration_set_value(integration_handler_t f, int32_t value);

#ifdef __cplusplus
}
#endif
#endif