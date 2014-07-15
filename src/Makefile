
TARGET         = pardyBuzz
OBJ            = main.o osc_cal.o usbdrv/usbdrv.o usbdrv/usbdrvasm.o usbdrv/oddebug.o
MCU_TARGET     = attiny85
OPTIMIZE       = -Os

DEFS           = -DF_CPU=16000000UL -DDEBUG_LEVEL=0
LIBS           = 
INCS           = -Iusbdrv -I.

CC             = avr-gcc

CFLAGS         = -g -Wall $(OPTIMIZE) $(INCS) -mmcu=$(MCU_TARGET) $(DEFS)
LDFLAGS        = 

OBJCOPY        = avr-objcopy
OBJCOPYFLAGS_I = -O ihex $(TARGET).elf $(TARGET).ihex
OBJCOPYFLAGS_B = -O binary $(TARGET).elf $(TARGET).bin

AVRDUDE	       = avrdude
DUDEFLAGS      = -v -p t85 -c stk500 -P /dev/ttyUSB0
DUDETARGET     = -U $(TARGET).ihex
DUDEFUSES      = -U lfuse:w:0xe1:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

COMPILE        = $(CC) $(CFLAGS)

all: $(TARGET).elf copy

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

prog: all
	$(AVRDUDE) $(DUDEFLAGS) $(DUDETARGET)

fuses:
	$(AVRDUDE) $(DUDEFLAGS) $(DUDEFUSES)

readcal:
	$(AVRDUDE) $(DUDEFLAGS) -U calibration:r:/dev/stdout:i | head -1

copy: $(TARGET).elf
	$(OBJCOPY) $(OBJCOPYFLAGS_I)
	$(OBJCOPY) $(OBJCOPYFLAGS_B)

clean:
	rm -rf *.o usbdrv/*.o $(TARGET).bin $(TARGET).elf $(TARGET).ihex *.lst *.map

.PHONY: clean all copy prog






