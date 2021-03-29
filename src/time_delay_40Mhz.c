#include "time_delay.h"

// Following functions only valid for CLOCK_FREQ == 40Mhz
void delay_us(unsigned char us)
{
	delay10tcy(us);			// 1us
}

void delay_10us(unsigned char us10)
{
	delay100tcy(us10);		// 10 us
}

void delay_100us(unsigned char us100)
{
	delay1ktcy(us100);		// 100 us
}

void delay_ms(unsigned char ms)
{
	delay10ktcy(ms);		// 1 ms
}

void delay_10ms(unsigned char ms10)
{
	delay100ktcy(ms10);		// 10 ms
}

void delay_100ms(unsigned char ms100)
{
	delay1mtcy(ms100);		// 100 ms
}

void delay_s(unsigned char s)
{
	delay1mtcy(10*s);			// 1 s
}

