//-----------------------------------------------------------------------------
/*


*/
//-----------------------------------------------------------------------------

#include "lcd.h"
#include "U8glib.h"

//-----------------------------------------------------------------------------

U8GLIB_NHD_C12864 u8g(13 /*SCK*/, 11 /*MOSI*/, 10 /*CS*/, 9 /*CD*/, 8 /*RST*/);

static void draw(void) {
	// graphic commands to redraw the complete screen should be placed here
	u8g.setFont(u8g_font_unifont);
	//u8g.setFont(u8g_font_osb21);
	u8g.drawStr(0, 20, "www.DFRobot.com");
}

static void setup(void) {
	u8g.setContrast(0);	// Config the contrast to the best effect
	u8g.setRot180();	// rotate screen, if required
	// set SPI backup if required
	//u8g.setHardwareBackup(u8g_backup_avr_spi);

	// assign default color value
	if (u8g.getMode() == U8G_MODE_R3G3B2) {
		u8g.setColorIndex(255);	// white
	} else if (u8g.getMode() == U8G_MODE_GRAY2BIT) {
		u8g.setColorIndex(3);	// max intensity
	} else if (u8g.getMode() == U8G_MODE_BW) {
		u8g.setColorIndex(1);	// pixel on
	} else if (u8g.getMode() == U8G_MODE_HICOLOR) {
		u8g.setHiColorByRGB(255, 255, 255);
	}
}

//-----------------------------------------------------------------------------
// stdio compatible putc

int lcd_putc(char c, FILE * stream) {
	return 0;
}

//-----------------------------------------------------------------------------

int lcd_init(void) {
	setup();
	draw();
	return 0;
}

//-----------------------------------------------------------------------------
