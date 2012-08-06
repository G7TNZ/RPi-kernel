/*
 * gpio.h
 *
 */
 
#ifndef _H_GPIO
#define _H_GPIO

#include <stdint.h>

#include "generalIO.h"

#define GPIO_BASE 0x00200000

// Port function select bits
#define FSEL_MASK			0b111
#define	FSEL_INPUT		0b000
#define	FSEL_OUTPUT		0b001
#define	FSEL_ALT0			0b100
#define	FSEL_ALT0			0b100
#define	FSEL_ALT1			0b101
#define	FSEL_ALT2			0b110
#define	FSEL_ALT3			0b111
#define	FSEL_ALT4			0b011
#define	FSEL_ALT5			0b010

// IO offsets
#define GPIO_PINS			0x00100000 // check wiringPi
#define CLOCK_BASE		0x00101000 // check wiringPi
#define GPIO_PWM			0x0020C000

// Control Register offsets
#define GPFSEL0		0x00200000 // Define function of pins  0-9.
#define GPFSEL1		0x00200004 // Define function of pins 10-19.
#define GPFSEL2		0x00200008 // Define function of pins 20-29.
#define GPFSEL3		0x0020000C // Define function of pins 30-39.
#define GPFSEL4		0x00200010 // Define function of pins 40-49.
#define GPFSEL5		0x00200014 // Define function of pins 50-53.
#define GPSET0		0x0020001C // Output set  0-31
#define GPSET1		0x00200020 // Output set 32-53
#define GPCLR0		0x00200028 // Output cleared  0-31
#define GPCLR1		0x0020002C // Output cleared 32-53
#define GPLEV0		0x00200034 // Pin  0-31 high/low?
#define GPLEV1		0x00200038 // Pin 32-53 high/low?
#define GPEDS0		0x00200040 // Edge detected  0-31
#define GPEDS1		0x00200044 // Edge detected 32-53
#define GPREN0		0x0020004C // Rising edge detected  0-31
#define GPREN1		0x00200050 // Rising edge detected  32-53
#define GPFEN0		0x00200058 // Falling edge detected  0-31
#define GPFEN1		0x0020005C // Falling edge detected  32-53
#define GPHEN0		0x00200064 // Define GPEDSn to be triggered on high  0-31
#define GPHEN1		0x00200068 // Define GPEDSn to be triggered on high 32-53
#define GPLEN0		0x00200070 // Define GPEDSn to be triggered on low  0-31
#define GPLEN1		0x00200074 // Define GPEDSn to be triggered on low 32-53
#define GPAREN0		0x0020007C // Define GPEDSn to be triggered on async rising  0-31
#define GPAREN1		0x00200080 // Define GPEDSn to be triggered on async rising 32-53
#define GPAFEN0		0x00200088 // Define GPEDSn to be triggered on async falling  0-31
#define GPAFEN1		0x0020008C // Define GPEDSn to be triggered on async falling 32-53
#define GPUD			0x00200094 // Sets state pull up/down resistors selcted by GPPUDCLKn
#define GPPUDCLK0	0x00200098 // Assert GPUD for  0-31
#define	GPPUDCLK1 0x0020009C // Assert GPUD for 32-53

// Individual pin names
// LED
#define RPI_OK					16	// OK LED
#define RPI_STATUS			16	// OK LED

// General purpose
#define RPI_GPIO0_P11		17	// General purpose 0
#define RPI_GPIO1_P12		18	// General purpose 1
#define RPI_GPIO2_P13		21	// General purpose 2
#define RPI_GPIO3_P15		22	// General purpose 3
#define RPI_GPIO4_P16		23	// General purpose 4
#define RPI_GPIO5_P18		24	// General purpose 5
#define RPI_GPIO6_P22		25	// General purpose 6
#define RPI_GPIO7_P7		4		// General purpose 7
#define RPI_GPIOGCLK_P7	4		// General purpose 7

#define POSITIVE_LOGIC true
#define NEGATIVE_LOGIC false
#define END_OF_WORD true
#define END_OF_LETTER false

#define INPUT true
#define OUTPUT false

#define SET true
#define N_CLEAR true
#define ONE true
#define N_ZERO true
#define CLEAR false
#define N_SET false
#define ZERO false
#define N_ONE false

void	Gpio_SetPinDirection(int pin, bool input);
bool	Gpio_Read(uint32_t pin);
void	Gpio_Write(uint32_t pin, bool setToOne);
void	Gpio_FlashPin(int pin, int pattern, bool endWord, bool positiveLogic);
void	Gpio_FlashStatusLed(int pattern, bool endWord);
void	Gpio_WordSpace();
void 	Gpio_LetterSpace();
void	Gpio_DashDelay();
void	Gpio_DotDelay();
void	Gpio_SetMorse(int speed, bool isSlow);
int		Gpio_GetMorseSpeed();
bool	Gpio_GetMorseIsSlow();
void	Gpio_ReportNumber(const char* charString);
char* Gpio_ConvertToHexString(uint32_t number, char* characters, int pack);
char* Gpio_ConvertToDecimalString(uint32_t number, char* characters);

#endif

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
 
