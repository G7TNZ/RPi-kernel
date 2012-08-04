/*
 * gpio.c
 * General purpose IO
 *  OK LED, other 3v3 bits input/output.
 *
 */

#include <stdint.h>

#include "headers/gpio.h"
#include "headers/asmStart.h"
#include "headers/general.h"

int dot_time = 1;
bool slowMorse = true;
int slow_time = 12;

void Gpio_SetPinDirection(int pin, bool input) {
	uint32_t address;
	switch((int)(pin / 10)) {
		default:
		case 0:
			address = GPFSEL0;
			break;
		case 1:
			address = GPFSEL1;
			break;
		case 2:
			address = GPFSEL2;
			break;
		case 3:
			address = GPFSEL3;
			break;
		case 4:
			address = GPFSEL4;
			break;
		case 5:
			address = GPFSEL5;
			break;
	}
	unsigned int data  = ReadFromMemory32(address + ARM_PHYSICAL_IO_BASE);
	data &= ~(FSEL_MASK << (pin * 3));
	data |= (input ? FSEL_INPUT : FSEL_OUTPUT) << (pin * 3);
	WriteToMemory32(data, address + ARM_PHYSICAL_IO_BASE);	
}
 
bool Gpio_Read(uint32_t pin) {
	unsigned int address;
	if(pin <= 31) {
		address = GPLEV0;
	} else {
		address = GPLEV1;
	}
	uint32_t result = ReadFromMemory32(address + ARM_PHYSICAL_IO_BASE);
	
	return (result & (1 << (pin % 32)))? true : false;
}

void Gpio_Write(uint32_t pin, bool setToOne) {
	uint32_t address;
	if(setToOne) {
		if(pin <= 31) {
			address = GPSET0;
		} else {
			address = GPSET1;
		}
	} else {
		if(pin <= 31) {
			address = GPCLR0;
		} else {
			address = GPCLR1;
		}
	}
	WriteToMemory32(1 << (pin % 32), address + ARM_PHYSICAL_IO_BASE);
}

void Gpio_FlashPin(int pin, int pattern, bool endWord, bool positiveLogic) {
	int code, numberOfBits;
	uint32_t setAddress, clrAddress;
	
	code = codes[pattern][0];
	numberOfBits = codes[pattern][1];
	
	if(pin <= 31) {
		setAddress = GPSET0;
		clrAddress = GPCLR0;
	} else {
		setAddress = GPSET1;
		clrAddress = GPCLR1;
	}
	
	uint32_t onAddress  = positiveLogic ? setAddress : clrAddress;
	uint32_t offAddress = positiveLogic ? clrAddress : setAddress;
  uint32_t data = 1 << (pin % 32);
  
	for(int i = numberOfBits-1; i >= 0 ; i--) {
		WriteToMemory32(data, onAddress + ARM_PHYSICAL_IO_BASE);
		CheckBit(code, i) ? Gpio_DashDelay() : Gpio_DotDelay();
		WriteToMemory32(data, offAddress + ARM_PHYSICAL_IO_BASE);
		if (i != 0) Gpio_DotDelay();
	}
	endWord ? Gpio_WordSpace() : Gpio_LetterSpace();
}

void Gpio_FlashStatusLed(int pattern, bool endWord) {
	Gpio_FlashPin(RPI_STATUS, pattern, endWord, false);
}

void Gpio_WordSpace() {
	if (slowMorse) {
		for(int counter = 0; counter < 1000000; counter++) {
			SmallDelay(slow_time);
		}
	} else {
		for(int counter = 0; counter < 7; counter++) {
			Gpio_DotDelay();
		}
	}
}

void Gpio_LetterSpace() {
	if (slowMorse) {
		for(int counter = 0; counter < 1000000; counter++) {
			SmallDelay(slow_time);
		}
	} else {
		Gpio_DashDelay();
	}
}

void Gpio_DashDelay() {
	Gpio_DotDelay();
	Gpio_DotDelay();
	Gpio_DotDelay();
}

void Gpio_DotDelay() {
	for(int counter = 0; counter < 1000000; counter++) {
		SmallDelay(dot_time);
	}
}

void Gpio_SetMorse(int speed, bool isSlow) {
	if ((slowMorse = isSlow)) {
		slow_time = speed;
		dot_time = 1;
	} else {
		dot_time = speed;
	}
}

bool Gpio_GetMorseIsSlow() {
	return slowMorse;
}

int Gpio_GetMorseSpeed() {
	return dot_time;
}

char* Gpio_ConvertToHexString(uint32_t number, char* convertedNumber, int pack) {
	int counter =0;
	char temp[9];
	do {
		switch (number%16) {
			default:
			case 0:
				temp[counter] = '0';
				break;
			case 1:
				temp[counter] = '1';
				break;
			case 2:
				temp[counter] = '2';
				break;
			case 3:
				temp[counter] = '3';
				break;
			case 4:
				temp[counter] = '4';
				break;
			case 5:
				temp[counter] = '5';
				break;
			case 6:
				temp[counter] = '6';
				break;
			case 7:
				temp[counter] = '7';
				break;
			case 8:
				temp[counter] = '8';
				break;
			case 9:
				temp[counter] = '9';
				break;
			case 10:
				temp[counter] = 'A';
				break;
			case 11:
				temp[counter] = 'B';
				break;
			case 12:
				temp[counter] = 'C';
				break;
			case 13:
				temp[counter] = 'D';
				break;
			case 14:
				temp[counter] = 'E';
				break;
			case 15:
				temp[counter] = 'F';
				break;
		}
		number = number/16;
		counter++;
	} while (0 != number);
	
	int start = 0;
	if (pack > counter) {
		start = pack - counter;
		for (int i = 0; i < start; i++) {
			convertedNumber[i] = '0';
		}
	}
	for (int i = 0; i < counter; i++) {
		convertedNumber[i + start] = temp[counter-i-1];
	}
	convertedNumber[counter + start] = '\0';
	return convertedNumber;
}

char* Gpio_ConvertToDecimalString(uint32_t number, char* convertedNumber) {
	int counter =0;
	char temp[9];
	do {
		switch (number%10) {
			default:
			case 0:
				temp[counter] = '0';
				break;
			case 1:
				temp[counter] = '1';
				break;
			case 2:
				temp[counter] = '2';
				break;
			case 3:
				temp[counter] = '3';
				break;
			case 4:
				temp[counter] = '4';
				break;
			case 5:
				temp[counter] = '5';
				break;
			case 6:
				temp[counter] = '6';
				break;
			case 7:
				temp[counter] = '7';
				break;
			case 8:
				temp[counter] = '8';
				break;
			case 9:
				temp[counter] = '9';
				break;
		}
		number = number/10;
		counter++;
	} while (0 != number);
	
	convertedNumber[counter] = '\0';
	for (int i = 0; i < counter; i++) {
		convertedNumber[i] = temp[counter-i-1];
	}
	return convertedNumber;
}

void Gpio_ReportNumber(const char* charString) {
	int pattern;
	for (int i = string_length(charString)-1; i >= 0; i--) {
		switch (charString[i]) {
			default:
				pattern = PAT_X;
				break;
			case '0':
				pattern = PAT_0;
				break;
			case '1':
				pattern = PAT_1;
				break;
			case '2':
				pattern = PAT_2;
				break;
			case '3':
				pattern = PAT_3;
				break;
			case '4':
				pattern = PAT_4;
				break;
			case '5':
				pattern = PAT_5;
				break;
			case '6':
				pattern = PAT_6;
				break;
			case '7':
				pattern = PAT_7;
				break;
			case '8':
				pattern = PAT_8;
				break;
			case '9':
				pattern = PAT_9;
				break;
			case 'A':
				pattern = PAT_A;
				break;
			case 'B':
				pattern = PAT_B;
				break;
			case 'C':
				pattern = PAT_C;
				break;
			case 'D':
				pattern = PAT_D;
				break;
			case 'E':
				pattern = PAT_E;
				break;
			case 'F':
				pattern = PAT_F;
				break;
		}
		Gpio_FlashStatusLed(pattern, (0 == i) ? END_OF_WORD : END_OF_LETTER);
	}
	if (slowMorse) { Gpio_WordSpace(); Gpio_WordSpace(); }
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
