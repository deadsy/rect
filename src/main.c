//-----------------------------------------------------------------------------
/*

Rotary Engine Compression Tester

*/
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "uart.h"
#include "display.h"
#include "timer.h"
#include "adc.h"

//-----------------------------------------------------------------------------

#define NUM_ROTOR_SAMPLES 16
#define NUM_ROTOR 3

struct rotor_stats {
	int32_t prev_t;		// previous time sample
	int32_t sum_p;		// sum of pressure
	int32_t sum_p2;		// sum of pressure * pressure
	int32_t sum_t;		// sum of time
	int32_t sum_t2;		// sum of time * time
};

static struct rotor_stats rotor[NUM_ROTOR];

// reset all rotor samples
static void rotor_reset(void) {
	memset(rotor, 0, sizeof(rotor));
}

// add a sample for a rotor
static void rotor_add_sample(int n, ADC_SAMPLE * s) {
	struct rotor_stats *r = &rotor[n];
	int32_t p = s->val;
	int32_t t = (r->prev_t == 0) ? 0 : (s->n - r->prev_t);
	r->prev_t = s->n;
	r->sum_p += p;
	r->sum_p2 += p * p;
	r->sum_t += t;
	r->sum_t2 += t * t;
}

// average pressure for a rotor
static float rotor_p_ave(int n) {
	struct rotor_stats *r = &rotor[n];
	return (float)r->sum_p / (float)NUM_ROTOR_SAMPLES;
}

// standard deviation of rotor pressure samples
static float rotor_p_sd(int n) {
	struct rotor_stats *r = &rotor[n];
	float ave = rotor_p_ave(n);
	float sd2 = ((float)r->sum_p2 / (float)NUM_ROTOR_SAMPLES) - (ave * ave);
	return sqrtf(sd2);
}

// average time delta for rotor samples
static float rotor_t_ave(int n) {
	struct rotor_stats *r = &rotor[n];
	return (float)r->sum_t / (float)NUM_ROTOR_SAMPLES;
}

// standard deviation of time delta for rotor samples
static float rotor_t_sd(int n) {
	struct rotor_stats *r = &rotor[n];
	float ave = rotor_t_ave(n);
	float sd2 = ((float)r->sum_t2 / (float)NUM_ROTOR_SAMPLES) - (ave * ave);
	return sqrtf(sd2);
}

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
	rotor_reset();
	adc_start(0);

	for (int i = 0; i < NUM_ROTOR_SAMPLES; i++) {
		for (int j = 0; j < NUM_ROTOR; j++) {
			ADC_SAMPLE s;
			adc_read(&s, 0);
			rotor_add_sample(j, &s);
		}
	}

	for (int j = 0; j < NUM_ROTOR; j++) {
		float p_ave = rotor_p_ave(j);
		float p_sd = rotor_p_sd(j);
		float t_ave = rotor_t_ave(j);
		float t_sd = rotor_t_sd(j);
		printf("r%d: p %f(%f) t %f(%f)\n", j, p_ave, p_sd, t_ave, t_sd);
	}

#if 0

	while (1) {
		ADC_SAMPLE s;
		adc_read(&s, 0);
		printf("%08lx %04x %c\n", s.n, s.val, get_joystick(s.val));
	}

#endif

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

	printf_P(PSTR("sample rate %e\n"), ADC_SAMPLE_RATE);
	printf_P(PSTR("sample time %e\n"), ADC_SAMPLE_TIME);

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
