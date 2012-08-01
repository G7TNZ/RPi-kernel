LIBNAME	= ../usbstack
APPNAME = main

# Tool definitions
CC      = arm-elf-gcc
LD      = arm-elf-ld -v
AR      = arm-elf-ar
AS      = arm-elf-as
CP      = arm-elf-objcopy
OD		= arm-elf-objdump
RM		= rm

# Tool flags
CFLAGS  = -I./ -I../ -c -W -Wall -Os -g -DDEBUG -mcpu=arm7tdmi
ASFLAGS = -ahls -mapcs-32 -o crt.o
LFLAGS  =  -nostartfiles --warn-common
CPFLAGS = -O ihex
ODFLAGS	= -x --syms

LINKFILE	= lpc2148-rom.ld

CSRCS	= startup.c printf.c console.c
OBJS 	= crt.o $(CSRCS:.c=.o)

EXAMPLES = hid serial msc custom

all: depend $(EXAMPLES)

hid: 	$(OBJS) main_hid.o $(LIBNAME).a
serial:	$(OBJS) main_serial.o serial_fifo.o armVIC.o $(LIBNAME).a
msc:	$(OBJS) main_msc.o msc_bot.o msc_scsi.o blockdev_sd.o lpc2000_spi.o $(LIBNAME).a
custom:	$(OBJS) main_custom.o $(LIBNAME).a

$(EXAMPLES):
	@ echo "Building $@ example..."
	$(CC) -T $(LINKFILE) $(LFLAGS) $^ -o $@.elf -Wl,-Map,$@.map
	$(CP) $(CPFLAGS) $@.elf $@.hex
	$(OD) $(ODFLAGS) $@.elf > $@.dmp

crt.o: crt.s
	@ echo ".assembling"
	$(CC) -c $(AFLAGS) -Wa,-ahlms=crt.lst crt.s -o crt.o

clean:
	rm -f *.hex *.elf *.o *.lst *.dmp *.map .depend

# recompile if the Makefile changes
$(OBJS): Makefile

# dependency checking
depend: $(CSRCS)
	$(CC) $(CFLAGS) -MM $^ > .depend || rm -f .depend

# phony targets
.PHONY: clean

-include .depend

