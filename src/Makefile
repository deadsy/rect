TOP = ..

MCU = atmega32u4

TARGET = rect

RECT_SRC = ./rect
U8G2_SRC = ./u8g2/csrc
U8G2_AVR = ./u8g2/sys/avr/avr-libc/lib

EXTRAINCDIRS += $(U8G2_AVR)
EXTRAINCDIRS += $(U8G2_SRC)

CDEFS += -DAVR_USE_HW_SPI
CDEFS += -DSCK_DDR=DDRB
CDEFS += -DSCK_BIT=5
CDEFS += -DMOSI_DDR=DDRB
CDEFS += -DMOSI_BIT=3

SRC =	$(shell ls $(RECT_SRC)/*.c) \
			$(shell ls $(U8G2_AVR)/*.c) \
			$(shell ls $(U8G2_SRC)/*.c) \

include $(TOP)/mk/common.mk
