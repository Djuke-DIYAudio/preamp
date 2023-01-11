//#ifndef TIME_DELAY_H
//#define TIME_DELAY_H

#include <xc.h>

// Main oscillator frequency is 40MHz
#define _XTAL_FREQ      40000000

#define delay_us(X)     __delay_us(X)
#define delay_10us(X)   __delay_us(10*X)
#define delay_100us(X)  __delay_us(100*X);

#define delay_ms(X)     __delay_ms(X)
#define delay_10ms(X)   __delay_ms(10*X)
#define delay_100ms(X)  __delay_ms(100*X);

#define delay_s(X)      __delay_ms(1000*X)


//#endif
