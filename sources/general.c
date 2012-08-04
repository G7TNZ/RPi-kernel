/*
 * general.c
 *
 */
 
 #include <stdint.h>
 #include "headers/general.h"
 #include "headers/gpio.h"
 #include "headers/asmStart.h"
 
int string_length(const char* str) {
	const char* s = str;
	while(*s) ++s;
	return s-str;
}

/*
 * provides the error notification for the divide functions
 */
void __div0(void) {
	Gpio_SetMorse(1, false);
	Gpio_FlashStatusLed(PAT_D, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_I, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_V, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_I, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_D, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_E, END_OF_WORD);
	Gpio_FlashStatusLed(PAT_0, END_OF_WORD);
	Gpio_FlashStatusLed(PAT_E, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_R, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_R, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_O, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_R, END_OF_WORD);
}

void* NextFreeMemory = 0;

void* AllocateMemory(uint32_t size) {
	if(0 == NextFreeMemory) {
		NextFreeMemory = &FreeMemoryStart;
	}
	void* StartOfAllocatedMemory = NextFreeMemory;
	NextFreeMemory += size;
	return StartOfAllocatedMemory;
}

/*
 * Copyright (c) 2012 Ischus Limited www.ischus.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
 
