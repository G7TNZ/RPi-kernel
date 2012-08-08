/*
 * general.c
 *
 */
 
 #include <stdint.h>
 #include "headers/general.h"
 #include "headers/gpio.h"
 #include "headers/asmStart.h"
 
/*
 * Local prototypes
 *
 */

void createHeader(uint32_t *address, uint32_t size, uint32_t allocated);
void createInitialHeader(uint32_t *address);
/*
 * Public methods
 *
 */
 
int string_length(const char* str) {
	const char* s = str;
	while(*s) ++s;
	return s-str;
}


// provides the error notification for the divide functions
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

// Dynamic allocation of memory
bool isInitialised = false;
#define MAGICALLOCATED 0x3a5fe82b
uint32_t* AllocateMemory(uint32_t size) {
	uint32_t* headerNextPointer = &FreeMemoryStart + 4;
	
	if (0 == size) return (uint32_t*) 0x80000000;	// cause exception if used?
	if(!isInitialised) {
		createInitialHeader(headerNextPointer);
	}
	
	bool isFound = false;
	while ((0 != *headerNextPointer) && !isFound) {	// not at end of list or finished
		if (0 == *(headerNextPointer+4)) { 						// not allocated
			uint32_t sizeOfFreeSpace = (*headerNextPointer - (uint32_t)(headerNextPointer) -8);
			if (size <= sizeOfFreeSpace) { 							// There is space here
				isFound = true;
				if ((size - sizeOfFreeSpace) >= 0x10) {		// Insert new header: more than is needed
					createHeader(headerNextPointer, size, false);
				} 
			}
			headerNextPointer = (uint32_t*)(*headerNextPointer);
		}
		if (!isFound) { 															// At the last header without finding space
			createHeader(headerNextPointer, size, false);
		}
	}
	return (headerNextPointer + 8);
}

bool FreeAllocatedMemory(uint32_t* address) {
	if (MAGICALLOCATED == *(address - 4)) {
		*(address - 4) = 0;		
	}

	return true;
}

/*
 * Local methods
 *
 */

// Create a memory header
void createHeader(uint32_t *address, uint32_t size, uint32_t allocated) {
	*(address + size + 4)	= (uint32_t)(address - 4);					// previous
	*(address + size + 8)	= (uint32_t)(*address);							// next
	*(address + size + 12) = allocated ? MAGICALLOCATED : 0;	// allocated
	*address = (uint32_t)(address + size +8);									// prior header 
}

// Create the first header
void createInitialHeader(uint32_t *address) {
	*(address - 4) = 0;
	*address = 0;
	*(address + 4) = 0;
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
 
