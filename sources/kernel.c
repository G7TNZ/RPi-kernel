/*
 * kernel.c
 * The main kernel for raspberry pi bare metal
 *
 */
 
#include <stdio.h>

#include "headers/kernel.h"
#include "headers/framebuffer.h"
#include "headers/gpio.h"
#include "headers/vectors.h"
#include "headers/colourStructure.h"
#include "headers/console.h"

void WriteTags(char* atags);

int start_kernel(void) {
	
	Gpio_SetMorse(36, true);
	Gpio_WordSpace();
	
	Gpio_SetPinDirection(RPI_STATUS, OUTPUT);
	SmallDelay(5);
	Gpio_Write(RPI_STATUS, SET);		// Setting turns LED off.
	Gpio_DashDelay();
	
	int result;
	do {
		result = InitFb();
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
	Fb_WriteString(putString);
	Fb_WriteString("show 0 50\n\r");
	Cn_WriteMemoryBlockHex(0, 0x50);					// Vector block
	Fb_WriteString("\n\r\n\rReady> show 100 21F\n\r");
	Cn_WriteMemoryBlockHex(0x100, 0x21F);			// Atag block
	Fb_WriteString("\n\r\n\rReady> print atags\n\r\n\r");
	char* atags = (char*) 0x12C;
	WriteTags(atags);													// Atag as character list
	Fb_WriteString("\n\rFinished.\n\r\n\rReady> ");
	
	Gpio_SetMorse(1, false);
	while(1) {
		Gpio_FlashStatusLed(PAT_V, END_OF_WORD);
	}

	return 0;
}

void WriteTags(char* atags) {
	int counter = 0;
	while(0 != atags[counter]) {
		if (' ' == atags[counter]) {
			Fb_WriteString("\n\r");
		} else {
			Fb_WriteCharacter(atags[counter]);
		}
		counter++;		
	}
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
