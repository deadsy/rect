//-----------------------------------------------------------------------------
/*

Analog To Digital Converter Driver

*/
//-----------------------------------------------------------------------------

#ifndef ADC_H
#define ADC_H

//-----------------------------------------------------------------------------

typedef struct {
	uint32_t n;		// sample number
	int16_t val;		// sample value
} ADC_SAMPLE;

// adc_sample_rate = cpu_clock / (adc_clocks_per_sample * prescalar)
#define ADC_SAMPLE_RATE  (16000000.f / (13.f * 128.f))
#define ADC_SAMPLE_TIME  (1.f / ADC_SAMPLE_RATE)

//-----------------------------------------------------------------------------
// api

// free-running interupt driven
void adc_isr(void);
void adc_start(uint8_t pin);
void adc_stop(void);
int adc_read(ADC_SAMPLE * sample, int timeout);

// polled
int16_t adc_poll(uint8_t pin);

int adc_init(void);

//-----------------------------------------------------------------------------

#endif				// ADC_H

//-----------------------------------------------------------------------------
