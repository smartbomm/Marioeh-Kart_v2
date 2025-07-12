#ifndef Hardware_Config_h
#define Hardware_Config_h
#include <stdint.h>
#include <Arduino.h>
#include <cpio.h>
#include <sam.h>

// Define deadzone of motor voltage
#define DEADZONE_MOTOR 55000u

//introduce variables 
uint16_t motor_voltage = 0u;

// Pin config for mmotor voltage
uint Motor = A6;



void configure_extint() {
    // Enable clock for EIC
    PM->APBAMASK.reg |= PM_APBAMASK_EIC;

    // Enable GCLK for EIC
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_EIC |
                        GCLK_CLKCTRL_GEN_GCLK0 |
                        GCLK_CLKCTRL_CLKEN;
    while (GCLK->STATUS.bit.SYNCBUSY);

    // Configure PA07 (Arduino D9) for EIC
    PORT->Group[PORTA].PINCFG[7].bit.PMUXEN = 1;
    PORT->Group[PORTA].PMUX[7 >> 1].bit.PMUXO = PORT_PMUX_PMUXO_A;

    // Enable input and set internal pull-up
    PORT->Group[PORTA].PINCFG[7].bit.INEN = 1;
    PORT->Group[PORTA].PINCFG[7].bit.PULLEN = 1;
    PORT->Group[PORTA].OUTSET.reg = (1 << 7);  // Enable pull-up

    // Disable EIC before configuration
    EIC->CTRL.bit.ENABLE = 0;
    while (EIC->STATUS.bit.SYNCBUSY);

    // Sense configuration: BOTH EDGES
    EIC->CONFIG[PORTA].reg &= ~EIC_CONFIG_SENSE7_Msk;
    EIC->CONFIG[PORTA].reg |= EIC_CONFIG_SENSE7_BOTH;

    // Enable EXTINT7
    EIC->INTENSET.reg = EIC_INTENSET_EXTINT7;

    // Enable EIC
    EIC->CTRL.bit.ENABLE = 1;
    while (EIC->STATUS.bit.SYNCBUSY);

    // Enable NVIC
    NVIC_EnableIRQ(EIC_IRQn);
}


#endif
