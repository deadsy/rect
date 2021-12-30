//-----------------------------------------------------------------------------
/*

Rotary Engine Compression Tester

*/
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "common.h"
#include "uart.h"
#include "lcd.h"
#include "timer.h"

//-----------------------------------------------------------------------------

static void rect(void) {

	timer_delay_msec(750);

	float pressure[3] = { 90.12, 100.23, 85.34 };
	lcd_results(321.3, pressure, "psi");

	while (1) {
		printf_P(PSTR("%08x\n"), timer_get_msec());
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
	printf_P(PSTR("RECT\n"));

	// initialisation
	int init_fails = 0;
	INIT(lcd_init);
	INIT(timer_init);
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
