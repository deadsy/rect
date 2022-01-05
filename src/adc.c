//-----------------------------------------------------------------------------
/*

Analog To Digital Driver

*/
//-----------------------------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>

#include "common.h"
#include "adc.h"

//-----------------------------------------------------------------------------

// prescalar = 128 (adc clock = 16MHz/128 = 125kHz)
#define ADC_PRESCALAR 7

//-----------------------------------------------------------------------------

static uint32_t adc_samples;

// adc conversion complete
void adc_isr(void) {
	uint16_t val = ADC;
	(void)val;
	adc_samples++;
}

// start free-running adc
void adc_start(uint8_t pin) {
	adc_samples = 0;
	// set the source
	ADMUX = _BV(REFS0) | ((pin & 15) << MUX0);
	// ADC enable, prescalar, auto-triggering, interrupt enabled, start the conversion
	ADCSRA = _BV(ADEN) | (ADC_PRESCALAR << ADPS0) | _BV(ADATE) | _BV(ADIE) | _BV(ADSC);
}

// stop free-running adc
void adc_stop(void) {
	// ADC disabled
	ADCSRA = 0;
}

// read a sample from the sample buffer
int adc_read(ADC_SAMPLE * sample, int timeout) {
	return 0;
}

uint32_t adc_get_count(void) {
	cli();
	uint32_t n = adc_samples;
	sei();
	return n;
}

//-----------------------------------------------------------------------------

// polled read of the adc value
uint16_t adc_poll(uint8_t pin) {
	// set the source
	ADMUX = _BV(REFS0) | ((pin & 15) << MUX0);
	// start the conversion
	ADCSRA = _BV(ADEN) | (ADC_PRESCALAR << ADPS0) | _BV(ADSC);
	// wait for the conversion to complete
	while ((ADCSRA & _BV(ADSC)) != 0) ;
	// return the result
	return ADC;
}

// adc initialise
int adc_init(void) {
	// turn on the adc power
	PRR &= ~_BV(PRADC);
	// AVcc (5V) external reference, right justified output
	ADMUX = _BV(REFS0);
	// ADC enable and prescalar
	ADCSRA = _BV(ADEN) | (ADC_PRESCALAR << ADPS0);
	// no analog comparison, no triggering, free-running
	ADCSRB = 0;
	// no digital pin disables
	DIDR0 = 0;
	return 0;
}

//-----------------------------------------------------------------------------
