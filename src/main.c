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
#include "display.h"
#include "timer.h"
#include "adc.h"

//-----------------------------------------------------------------------------

#if 0

static void rect(void) {

	timer_delay_msec(750);

	float pressure[3] = { 90.12, 100.23, 85.34 };
	lcd_results(321.3, pressure, "psi");

	int state = -1;
	while (1) {
		int new_state = get_button();
		if (state != new_state) {
			state = new_state;
			printf_P(PSTR("%d\n"), state);
		}
	}
}

#endif

static void rect(void) {
	adc_start(0);
	while (1) {
		ADC_SAMPLE s;
		adc_read(&s, 0);
		printf("%08lx %04x %c\n", s.n, s.val, get_joystick(s.val));
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
	INIT(adc_init);
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
