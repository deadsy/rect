//-----------------------------------------------------------------------------
/*

Analog To Digital Driver

*/
//-----------------------------------------------------------------------------

#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "common.h"
#include "adc.h"

//-----------------------------------------------------------------------------

// prescalar = 128
#define ADC_PRESCALAR 7

//-----------------------------------------------------------------------------

// ADC Buffer Size (must be a power of 2 and < 256)
#define ADC_BUFSIZE 8

static struct {
	uint32_t n;		// sample counter
	int state;		// peak detector state
	ADC_SAMPLE max;		// peak detector maximum
	ADC_SAMPLE min;		// peak detector minimum
	uint8_t rd;		// read index into buffer
	volatile uint8_t wr;	// write index into buffer
	ADC_SAMPLE buf[ADC_BUFSIZE];	// samples buffer
} adc;

// peak detector states
enum {
	FIND_MAX,		// find a maximum
	FIND_MIN,		// find a minimum
};

#define FALLING_THOLD 15	// switch to minimum detection
#define RISING_THOLD 15		// switch to maximum detection

//-----------------------------------------------------------------------------

static inline void adc_write(ADC_SAMPLE * s) {
	uint8_t wr_next = (adc.wr + 1) & (ADC_BUFSIZE - 1);
	if (wr_next != adc.rd) {
		adc.buf[adc.wr].val = s->val;
		adc.buf[adc.wr].n = s->n;
		adc.wr = wr_next;
	}
}

static inline void adc_set_sample(ADC_SAMPLE * s, int16_t val, uint32_t n) {
	s->val = val;
	s->n = n;
}

// adc conversion complete
void adc_isr(void) {
	int16_t val = ADC;
	adc.n++;

	if ((adc.n & ((1 << 12) - 1)) != 0) {
		return;
	}

	ADC_SAMPLE s;
	adc_set_sample(&s, val, adc.n);
	adc_write(&s);
}

// peak detector
void adc_isr2(void) {
	uint16_t val = ADC;
	adc.n++;

	if (adc.state == FIND_MAX) {
		// find maximum
		if (adc.max.val - val > FALLING_THOLD) {
			// falling - find a minimum
			adc.state = FIND_MIN;
			adc_set_sample(&adc.min, val, adc.n);
			// write the peak sample to the buffer
			adc_write(&adc.max);
		} else {
			if (val > adc.max.val) {
				// update the maximum
				adc_set_sample(&adc.max, val, adc.n);
			}
		}
	} else {
		// find minimum
		if (val - adc.min.val > RISING_THOLD) {
			// rising - find a maximum
			adc.state = FIND_MAX;
			adc_set_sample(&adc.max, val, adc.n);
		} else {
			if (val < adc.min.val) {
				// update the minimum
				adc_set_sample(&adc.min, val, adc.n);
			}
		}
	}
}

//-----------------------------------------------------------------------------

// start free-running adc
void adc_start(uint8_t pin) {
	// make sure we have stopped
	ADCSRA = 0;
	// reset the adc state
	memset(&adc, 0, sizeof(adc));
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
int adc_read(ADC_SAMPLE * s, int timeout) {
	// Wait for a sample in the buffer.
	while (adc.rd == adc.wr) ;
	cli();
	s->val = adc.buf[adc.rd].val;
	s->n = adc.buf[adc.rd].n;
	adc.rd = (adc.rd + 1) & (ADC_BUFSIZE - 1);
	sei();
	return 0;
}

//-----------------------------------------------------------------------------

// polled read of the adc value
int16_t adc_poll(uint8_t pin) {
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
