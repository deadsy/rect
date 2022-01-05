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

// ADC Buffer Size (must be a power of 2 and < 256)
#define ADC_BUFSIZE 8

static struct {
	uint32_t n;		// sample counter
	uint8_t rd;		// read index into buffer
	volatile uint8_t wr;	// write index into buffer
	ADC_SAMPLE buf[ADC_BUFSIZE];	// samples buffer
} adc;

//-----------------------------------------------------------------------------

// adc conversion complete
void adc_isr(void) {
	uint16_t val = ADC;
	adc.n++;

	if ((adc.n & ((1 << 12) - 1)) != 0) {
		return;
	}

	uint8_t wr_next = (adc.wr + 1) & (ADC_BUFSIZE - 1);
	if (wr_next != adc.rd) {
		adc.buf[adc.wr].val = val;
		adc.buf[adc.wr].n = adc.n;
		adc.wr = wr_next;
	}
}

// start free-running adc
void adc_start(uint8_t pin) {
	// make sure we have stopped
	ADCSRA = 0;
	// reset the adc buffering
	adc.n = adc.rd = adc.wr = 0;
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
	// Wait for a sample in the buffer.
	while (adc.rd == adc.wr) ;
	cli();
	sample->val = adc.buf[adc.rd].val;
	sample->n = adc.buf[adc.rd].n;
	adc.rd = (adc.rd + 1) & (ADC_BUFSIZE - 1);
	sei();
	return 0;
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
