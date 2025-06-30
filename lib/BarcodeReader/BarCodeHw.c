#include "BarCodeHw.h"

#include "sam.h"

static void (*tc3_user_isr)(void) = NULL;

void _tc3_init(uint16_t timeout)
{

    // Setup GCLK4 = 48 MHz / 48 = 1 MHz
    GCLK->GENDIV.reg = GCLK_GENDIV_ID(4) | GCLK_GENDIV_DIV(48);
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(4) |
                        GCLK_GENCTRL_GENEN |
                        GCLK_GENCTRL_SRC_DFLL48M |
                        GCLK_GENCTRL_IDC;
    while (GCLK->STATUS.bit.SYNCBUSY)
        ;

    // Route GCLK4 to TC3
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_TCC2_TC3 |
                        GCLK_CLKCTRL_GEN_GCLK4 |
                        GCLK_CLKCTRL_CLKEN;
    while (GCLK->STATUS.bit.SYNCBUSY)
        ;

    // Enable TC3 APB clock
    PM->APBCMASK.reg |= PM_APBCMASK_TC3;

    // Reset TC3
    TC3->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY)
        ;
    while (TC3->COUNT16.CTRLA.bit.SWRST)
        ;

    // Configure TC3 to count at 1 MHz
    TC3->COUNT16.CTRLA.reg =
        TC_CTRLA_MODE_COUNT16 |
        TC_CTRLA_PRESCALER_DIV1 | // Kein weiterer Prescaler
        TC_CTRLA_WAVEGEN_MFRQ;    // Match frequency
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY)
        ;

    // Set compare value for interrupt (timer period)
    TC3->COUNT16.CC[0].reg = timeout;
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY)
        ;

    // Enable interrupt on compare match
    TC3->COUNT16.INTENSET.reg = TC_INTENSET_MC0;

    // Enable TC3 interrupt in NVIC
    NVIC_EnableIRQ(TC3_IRQn);

}

void _tc3_start(void)
{
    // Start TC3
    TC3->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY)
        ;
}

void _tc3_stop(void)
{
    // Stop TC3
    TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY)
        ;
    // Reset the counter
    TC3->COUNT16.COUNT.reg = 0;
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY)
        ;
    // Clear the interrupt flag
    TC3->COUNT16.INTFLAG.reg = TC_INTFLAG_MC0;
    while (TC3->COUNT16.INTFLAG.reg & TC_INTFLAG_MC0)
        ;
}

void _tc3_restart(void)
{
        // Reset the counter
    TC3->COUNT16.COUNT.reg = 0;
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY)
        ;
}

void _tc3_set_isr(void (*isr)(void))
{
    tc3_user_isr = isr;
}



void TC3_Handler() {
    if (TC3->COUNT16.INTFLAG.bit.MC0) {
        // Dein Code hier (z. B. Toggle einer LED, Zähler, usw.)
        TC3->COUNT16.INTFLAG.reg = TC_INTFLAG_MC0; // Interrupt-Flag löschen
        if (tc3_user_isr != NULL) {
            tc3_user_isr(); // Benutzerdefinierte ISR aufrufen
        }
    }
}


