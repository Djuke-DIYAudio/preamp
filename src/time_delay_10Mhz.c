#include "time_delay.h"

// Following functions only valid for CLOCK_FREQ == 10Mhz
void delay_us(unsigned char us)
{
	if (us < 4) delay10tcy(1);	// 4 us
	else delay10tcy(us/4);
}

void delay_10us(unsigned char us10)
{
	delay10tcy(us10);		// 4 us
	delay10tcy(us10);		// 4 us
}

void delay_100us(unsigned char us100)
{
	delay100tcy(us100);		// 40 us
	delay100tcy(us100);		// 40 us
	delay10tcy(us100);		// 4 us
	delay10tcy(us100);		// 4 us
	delay10tcy(us100);		// 4 us
	delay10tcy(us100);		// 4 us
	delay10tcy(us100);		// 4 us
}

void delay_ms(unsigned char ms)
{
	delay1ktcy(ms);		// 400 us
	delay1ktcy(ms);		// 400 us
	delay100tcy(ms);		// 40 us
	delay100tcy(ms);		// 40 us
	delay100tcy(ms);		// 40 us
	delay100tcy(ms);		// 40 us
	delay100tcy(ms);		// 40 us
}

void delay_10ms(unsigned char ms10)
{
	delay10ktcy(ms10);		// 4 ms
	delay10ktcy(ms10);		// 4 ms
	delay1ktcy(ms10);		// 400 us
	delay1ktcy(ms10);		// 400 us
	delay1ktcy(ms10);		// 400 us
	delay1ktcy(ms10);		// 400 us
	delay1ktcy(ms10);		// 400 us
}

void delay_100ms(unsigned char ms100)
{
	delay100ktcy(ms100);		// 40 ms
	delay100ktcy(ms100);		// 40 ms
	delay10ktcy(ms100);		// 4 ms
	delay10ktcy(ms100);		// 4 ms
	delay10ktcy(ms100);		// 4 ms
	delay10ktcy(ms100);		// 4 ms
	delay10ktcy(ms100);		// 4 ms
}

void delay_s(unsigned char s)
{
	delay1mtcy(s);			// 400 ms
	delay1mtcy(s);			// 400 ms
	delay100ktcy(s);		// 40 ms
	delay100ktcy(s);		// 40 ms
	delay100ktcy(s);		// 40 ms
	delay100ktcy(s);		// 40 ms
	delay100ktcy(s);		// 40 ms
}

