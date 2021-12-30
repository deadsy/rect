
TARGET = rect

CURRENT_DIR = $(shell pwd)

SRC_DIR = $(CURRENT_DIR)/src
U8G2_DIR = $(CURRENT_DIR)/u8g2
U8G2_SRC = $(U8G2_DIR)/csrc
U8G2_AVR = $(U8G2_DIR)/sys/avr/avr-libc/lib

CC = avr-gcc
OBJCPY = avr-objcopy
SIZE = avr-size
AVRDUDE = avrdude

MCU = atmega328p
F_CPU = 16000000

CFLAGS = \
	-mmcu=$(MCU) \
	-DF_CPU=$(F_CPU)UL \
	-Os \
	-std=gnu99 \
	-Wall \
	-Wstrict-prototypes \
	-Werror \
	-ffunction-sections \
	-fdata-sections \
	-I$(U8G2_SRC) \
	-I$(U8G2_AVR) \
	-DAVR_USE_HW_SPI \
	-DSCK_DDR=DDRB \
	-DSCK_BIT=5 \
	-DMOSI_DDR=DDRB \
	-DMOSI_BIT=3

LDFLAGS = \
	-Wl,-u,vfprintf -lprintf_flt \
	-Wl,-lm \
	-Wl,--gc-sections \
	-mmcu=$(MCU)

SRC =	$(shell ls $(U8G2_SRC)/*.c) \
			$(shell ls $(U8G2_AVR)/*.c) \
			$(shell ls $(SRC_DIR)/*.c)

OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all
all: $(TARGET).hex

.PHONY: program
program: $(TARGET).hex
	$(AVRDUDE) -p$(MCU) -cusbtiny -Pusb -Uflash:w:$(TARGET).hex:i

.PHONY: clean
clean:
	-rm -rf $(OBJ)
	-rm -rf $(TARGET).hex
	-rm -rf $(TARGET).elf

$(TARGET).hex: $(TARGET).elf
	$(OBJCPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $(TARGET).elf $(TARGET).hex

$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@
	$(SIZE) --mcu=$(MCU) --format=avr $(TARGET).elf
