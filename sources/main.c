/*
 * main.c
 * The main kernel for raspberry pi bare metal
 *
 */
 
#include <stdio.h>

#include "headers/asmStart.h"
#include "headers/atags.h"
#include "headers/allocateMemory.h"
#include "headers/colourStructure.h"
#include "headers/console.h"
#include "headers/framebuffer.h"
#include "headers/general.h"
#include "headers/gpio.h"
#include "headers/main.h"
#include "headers/string.h"

void WriteCmdLineStrings(char *atags);

int start_main(void) {

	GeneralInitialise();

	Gpio_SetMorse(36, true);
	Gpio_WordSpace();
	
	Gpio_SetMorse(1, false);
	
	Gpio_SetPinDirection(RPI_STATUS, OUTPUT);
	SmallDelay(5);
	Gpio_Write(RPI_STATUS, SET);		// Setting turns LED off.
	Gpio_DashDelay();
	
	Gpio_FlashStatusLed(PAT_H, END_OF_WORD);
//	Atags_Init();
	
	int result;
	int mode = MODE1024X768X24;
	do {
		result = InitFb(mode);
	} while(0 != result);

	Fb_ClearScreen();
	struct ColourStructure colour;
	colour.red		= 0xD0;						// Sand 				D0C090
	colour.green	= 0xC0;						// Button gray 	D4D0C8
	colour.blue		= 0x90;
	Fb_SetBackgroundColour(colour);
	colour.red		= 0x30;
	colour.green	= 0x30;
	colour.blue		= 0x30;
	Fb_SetForegroundColour(colour);
	
	char putString[] = "Hello World!\n\r\n\rReady> ";
	Fb_WriteLine(putString);
			
	Gpio_SetMorse(1, false);
	Gpio_FlashStatusLed(PAT_V, END_OF_WORD);
	
/* Start debug
	
	uint32_t* memory1 = AllocateMemory(0x10);
	char hexString[9];
	Fb_WriteString("memory1: ");
	Fb_WriteLine(String_ConvertToHexString((uint32_t) memory1, hexString, 8));
	FreeAllocatedMemory(memory1);
	
	Console_WriteMemoryBlockHex((uint32_t) &__bss_end__, (uint32_t) ((uint32_t*)(&__bss_end__) + 0x20));
	
// End debug
*/

	Gpio_SetPinDirection(RPI_GPIO7_P7, INPUT);
		int morseBit;
		while(1) {
		morseBit = Gpio_Read(RPI_GPIO7_P7);
		if (0 == morseBit) {
			Gpio_Write(RPI_STATUS, N_SET);
		} else {
			Gpio_Write(RPI_STATUS, N_CLEAR);
		}
	}

	return 0;
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
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
