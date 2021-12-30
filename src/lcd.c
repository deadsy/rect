//-----------------------------------------------------------------------------
/*


*/
//-----------------------------------------------------------------------------

#include <u8g2.h>
#include <util/delay.h>
#include <u8x8_avr.h>
#include <avr/power.h>

#include "lcd.h"

//-----------------------------------------------------------------------------

// arduino uno
// clock = 13 (PB5)
// data = 11 (PB3)
// cs = 10 (PB2)
// dc = 9 (PB1)
// reset = 8 (PB0)

#define CS_DDR DDRB
#define CS_PORT PORTB
#define CS_BIT 2

#define DC_DDR DDRB
#define DC_PORT PORTB
#define DC_BIT 1

#define RESET_DDR DDRB
#define RESET_PORT PORTB
#define RESET_BIT 0

//-----------------------------------------------------------------------------
// stdio compatible putc

int lcd_putc(char c, FILE * stream) {
	return 0;
}

//-----------------------------------------------------------------------------

static u8g2_t u8g2;

static uint8_t u8x8_gpio_and_delay(u8x8_t * u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
	// Re-use library for delays
	if (u8x8_avr_delay(u8x8, msg, arg_int, arg_ptr))
		return 1;

	switch (msg) {
		// called once during init phase of u8g2/u8x8
		// can be used to setup pins
	case U8X8_MSG_GPIO_AND_DELAY_INIT:
		CS_DDR |= _BV(CS_BIT);
		DC_DDR |= _BV(DC_BIT);
		RESET_DDR |= _BV(RESET_BIT);
		break;
		// CS (chip select) pin: Output level in arg_int
	case U8X8_MSG_GPIO_CS:
		if (arg_int)
			CS_PORT |= _BV(CS_BIT);
		else
			CS_PORT &= ~_BV(CS_BIT);
		break;
		// DC (data/cmd, A0, register select) pin: Output level in arg_int
	case U8X8_MSG_GPIO_DC:
		if (arg_int)
			DC_PORT |= _BV(DC_BIT);
		else
			DC_PORT &= ~_BV(DC_BIT);
		break;
		// Reset pin: Output level in arg_int
	case U8X8_MSG_GPIO_RESET:
		if (arg_int)
			RESET_PORT |= _BV(RESET_BIT);
		else
			RESET_PORT &= ~_BV(RESET_BIT);
		break;
	default:
		u8x8_SetGPIOResult(u8x8, 1);
		break;
	}
	return 1;
}

int lcd_init(void) {

	u8g2_Setup_st7565_nhd_c12864_f(&u8g2, U8G2_R2, u8x8_byte_avr_hw_spi, u8x8_gpio_and_delay);
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
	u8g2_SetContrast(&u8g2, 40 << 2);

	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);

	float x0 = 90.1234;
	float x1 = 91.2345;
	float x2 = 92.3456;

	char s[64];
	sprintf(s, "r%d: %.2f psi", 0, x0);
	u8g2_DrawStr(&u8g2, 0, 16, s);

	sprintf(s, "r%d: %.2f psi", 1, x1);
	u8g2_DrawStr(&u8g2, 0, 32, s);

	sprintf(s, "r%d: %.2f psi", 2, x2);
	u8g2_DrawStr(&u8g2, 0, 48, s);

	u8g2_SendBuffer(&u8g2);

	return 0;
}

//-----------------------------------------------------------------------------
