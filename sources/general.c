/*
 * general.c
 *
 */
 
#include <stdint.h>
#include "headers/general.h"
#include "headers/gpio.h"
#include "headers/allocateMemory.h"

#include "headers/console.h"
 
/*
 * Public methods
 *
 */
 
//until bss is zeroed.
void GeneralInitialise(void) {
	isInitialised = false;
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

// provides the error notification for undefined interrupt
void undefinedError(void) {
	Gpio_SetMorse(1, false);
	Gpio_FlashStatusLed(PAT_U, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_N, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_D, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_E, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_F, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_I, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_N, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_E, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_D, END_OF_WORD);
	Gpio_FlashStatusLed(PAT_E, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_R, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_R, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_O, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_R, END_OF_WORD);
}

// provides the error notification for the data abort interrupt
void abortError(void) {
	Gpio_SetMorse(1, false);
	Gpio_FlashStatusLed(PAT_A, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_B, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_O, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_R, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_T, END_OF_WORD);
	Gpio_FlashStatusLed(PAT_E, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_R, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_R, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_O, END_OF_LETTER);
	Gpio_FlashStatusLed(PAT_R, END_OF_WORD);
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
 
