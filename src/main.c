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

#include "common.h"
#include "uart.h"
#include "lcd.h"

//-----------------------------------------------------------------------------

static void rect(void) {

	printf_P(PSTR("RECT\n"));
	printf_P(PSTR("Version 1.0\n"));

	while (1) {
	}
}

//-----------------------------------------------------------------------------
// Use UART for stdio

static FILE uart_stream;

static void uart_stdio(void) {
	uart_init();
	uart_stream.put = uart_putc;
	uart_stream.get = uart_getc;
	uart_stream.flags = _FDEV_SETUP_RW;
	uart_stream.udata = 0;
	stdout = stdin = stderr = &uart_stream;
}

//-----------------------------------------------------------------------------

int main(void) {

	uart_stdio();
	sei();
	putc('\n', stdout);

	printf_P(PSTR("hello world!\n"));

	// initialisation
	int init_fails = 0;
	INIT(lcd_init);
	if (init_fails != 0) {
		// loop forever...
		while (1) ;
	}

	rect();

	// loop forever...
	while (1) ;
	return 0;
}

//-----------------------------------------------------------------------------
