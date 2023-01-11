//
//
//    Low priority ISR
//    This replaces the SDCC signals system
//    for generating interrupt processing.
//


#include <xc.h>

#include "PreampController.h"


void __interrupt(low_priority) ISR_LOWt(void)
{
    if (ADIE && ADIF)
    { // any AD interrupts?        
        ADIF = 0;
        ad_handler();
    }
    
    if(TMR1IE && TMR1IF)
    { // any timer 1 interrupts?
        TMR1IF = 0;
        tmr1_handler();
    }
    // process other interrupt sources here, if required
    return;
}

