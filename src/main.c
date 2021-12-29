//-----------------------------------------------------------------------------
/*

Rotary Engine Compression Tester

*/
//-----------------------------------------------------------------------------

#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <math.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "lcd.h"

//-----------------------------------------------------------------------------

static void rect(void) {

	printf_P(PSTR("RECT\n"));
	printf_P(PSTR("Version 1.0\n"));

	while (1) {
	}
}

//-----------------------------------------------------------------------------
// Use LCD for stdio

static FILE lcd_stream;

static void lcd_stdio(void) {
	lcd_init();
	lcd_stream.put = lcd_putc;
	lcd_stream.get = 0;
	lcd_stream.flags = _FDEV_SETUP_RW;
	lcd_stream.udata = 0;
	stdout = stdin = stderr = &lcd_stream;
}

//-----------------------------------------------------------------------------

int main(void) {
	sei();

	lcd_stdio();
	putc('\n', stdout);

#if 0
	// initialisation
	int init_fails = 0;
	INIT(uart_init);
	INIT(timer_init);
	INIT(led_init);
	INIT(midi_init);
	INIT(key_init);
	if (init_fails != 0) {
		// loop forever...
		while (1) ;
	}
#endif

	rect();

	// loop forever...
	while (1) ;
	return 0;
}

//-----------------------------------------------------------------------------
