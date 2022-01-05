//-----------------------------------------------------------------------------
/*

Analog To Digital Driver

*/
//-----------------------------------------------------------------------------

#ifndef ADC_H
#define ADC_H

//-----------------------------------------------------------------------------

typedef struct {
	uint32_t n;		// sample number
	uint16_t val;		// sample value
} ADC_SAMPLE;

//-----------------------------------------------------------------------------
// api

// free-running interupt driven
void adc_isr(void);
void adc_start(uint8_t pin);
void adc_stop(void);
int adc_read(ADC_SAMPLE * sample, int timeout);

// polled
uint16_t adc_poll(uint8_t pin);

int adc_init(void);

//-----------------------------------------------------------------------------

#endif				// ADC_H

//-----------------------------------------------------------------------------
