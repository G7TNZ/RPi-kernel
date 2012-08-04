/*
 * console.c
 *
 */

#include "headers/framebuffer.h"
#include "headers/console.h"
#include "headers/gpio.h"

void Console_WriteMemoryBlockHex(uint32_t start, uint32_t end) {
	char convertedNumber[9];
	char* atags = (char*) start;
	for (int index = 0; index < (end - start); index++) {
		if (0 == (index % 16)) {
			Fb_WriteString("\n\r");
			Fb_WriteString(Gpio_ConvertToHexString(start+ index, convertedNumber, 4));
			Fb_WriteString(":");
		} else if (0 == (index % 8)) {
			Fb_WriteCharacter(' ');
		}
		Fb_WriteCharacter(' ');
		Fb_WriteString(Gpio_ConvertToHexString((int) atags[index], convertedNumber, 2));
	}
}

/*
 * Local functions
 */



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
