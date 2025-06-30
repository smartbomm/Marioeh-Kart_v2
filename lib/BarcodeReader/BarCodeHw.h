#ifndef BarCodeHw_h
#define BarCodeHw_h

#ifdef __cplusplus
extern "C" {
#endif

#include <sam.h>
#include <stddef.h>
void _tc3_init(uint16_t timeout);

void _tc3_start(void);

void _tc3_stop(void);

void _tc3_restart(void);

void _tc3_set_isr(void (*isr)(void));

#ifdef __cplusplus
}   
#endif

#endif