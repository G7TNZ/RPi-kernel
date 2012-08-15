/*
 * string.c
 *
 */

#include <stdint.h>
#include "headers/string.h"



/*
 * Public functions
 */

int String_length(const char *str) {
	const char *s = str;
	while(*s) ++s;
	return s-str;
}

char* String_ConvertToHexString(uint32_t number, char *convertedNumber, int pack) {
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

char* String_ConvertToDecimalString(uint32_t number, char *convertedNumber) {
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
