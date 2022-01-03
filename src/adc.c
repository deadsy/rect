//-----------------------------------------------------------------------------
/*

Analog To Digital Driver

*/
//-----------------------------------------------------------------------------

#include <avr/io.h>

#include "common.h"
#include "adc.h"

//-----------------------------------------------------------------------------

uint16_t adc_poll(uint8_t pin) {
	// set the source
	ADMUX = _BV(REFS0) | ((pin & 15) << MUX0);
	// start the conversion
	ADCSRA |= _BV(ADSC);
	// wait for the conversion to complete
	while ((ADCSRA & _BV(ADSC)) != 0) ;
	// return the result
	return ADC;
}

int adc_init(void) {
	// turn on the adc power
	PRR &= ~_BV(PRADC);
	// AVcc (5V) external reference, right justified output
	ADMUX = _BV(REFS0);
	// ADC enable, free running, prescalar=128 (adc clock = 16MHz/128 = 125kHz)
	ADCSRA = _BV(ADEN) | (7 << ADPS0);
	// no analog comparison, no triggering
	ADCSRB = 0;
	// no digital pin disables
	DIDR0 = 0;
	return 0;
}

//-----------------------------------------------------------------------------
