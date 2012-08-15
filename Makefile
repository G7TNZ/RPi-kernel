# Modified to compile on the Raspberry Pi
#  also compiles on the scratchbox ARM compilers.

COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding -std=c99

gcc : bin/kernel.hex SDcard/kernel.img

clean :
	rm -f bin/*.o
	rm -f bin/*.bin
	rm -f bin/*.hex
	rm -f bin/*.elf
	rm -f *.list
	rm -f SDcard/*.img

bin/asmStart.o : sources/asmStart.s
	as sources/asmStart.s -o bin/asmStart.o

bin/armDivision.o : sources/armDivision.s
	as sources/armDivision.s -o bin/armDivision.o

bin/morseAscii.o : sources/morseAscii.s
	as sources/morseAscii.s -o bin/morseAscii.o

bin/memset.o : sources/memset.s
	as sources/memset.s -o bin/memset.o

bin/main.o : sources/main.c
	gcc $(COPS) -c sources/main.c -o bin/main.o

bin/framebuffer.o : sources/framebuffer.c
	gcc $(COPS) -c sources/framebuffer.c -o bin/framebuffer.o

bin/mailbox.o : sources/mailbox.c
	gcc $(COPS) -c sources/mailbox.c -o bin/mailbox.o

bin/gpio.o : sources/gpio.c
	gcc $(COPS) -c sources/gpio.c -o bin/gpio.o

bin/console.o : sources/console.c
	gcc $(COPS) -c sources/console.c -o bin/console.o

bin/atags.o : sources/atags.c
	gcc $(COPS) -c sources/atags.c -o bin/atags.o

bin/allocateMemory.o : sources/allocateMemory.c
	gcc $(COPS) -c sources/allocateMemory.c -o bin/allocateMemory.o

bin/morse.o : sources/morse.c
	gcc $(COPS) -c sources/morse.c -o bin/morse.o

bin/string.o : sources/string.c
	gcc $(COPS) -c sources/string.c -o bin/string.o

bin/general.o : sources/general.c
	gcc $(COPS) -c sources/general.c -o bin/general.o

bin/generalIO.o : sources/generalIO.c
	gcc $(COPS) -c sources/generalIO.c -o bin/generalIO.o

bin/standardFont.o : sources/standardFont.c
	gcc $(COPS) -c sources/standardFont.c -o bin/standardFont.o

bin/kernel.elf : sources/memmap bin/asmStart.o bin/armDivision.o bin/morseAscii.o bin/memset.o bin/main.o bin/framebuffer.o bin/mailbox.o bin/gpio.o bin/console.o bin/atags.o bin/allocateMemory.o bin/morse.o bin/generalIO.o bin/string.o bin/general.o bin/standardFont.o
	ld bin/asmStart.o bin/armDivision.o bin/morseAscii.o bin/memset.o bin/main.o bin/framebuffer.o bin/mailbox.o bin/gpio.o bin/console.o bin/morse.o bin/atags.o bin/allocateMemory.o bin/generalIO.o bin/string.o bin/general.o bin/standardFont.o -T sources/memmap -o bin/kernel.elf
	objdump -D bin/kernel.elf > kernel.list

SDcard/kernel.img : bin/kernel.elf
	objcopy bin/kernel.elf -O binary SDcard/kernel.img

bin/kernel.hex : bin/kernel.elf
	objcopy bin/kernel.elf -O ihex bin/kernel.hex

