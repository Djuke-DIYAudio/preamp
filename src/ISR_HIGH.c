
//
//
//    High priority ISR
//    This replaces the SDCC signals system
//    for generating interrupt processing.
//

#include <xc.h>

#include "PreampController.h"

void __interrupt(high_priority) ISR_HIGH(void)
{
    if (TMR0IE && TMR0IF)
    { // any timer 0 interrupts?        
        TMR0IF = 0;
        tmr0_handler();
    }
    if(LVDIE && LVDIF)
    { // any LVD interrupts?
        LVDIF = 0;
        lvd_handler();
    }
    
    if(RCIE && RCIF)
    { // any UART RX interrupts?
        RCIF = 0;
        rx_handler();
    }
}
