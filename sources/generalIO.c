/*
 * generalIO.c
 *
 */
 
 #include <stdint.h>
 
int codes [45][2] = { 
	{0b000000,1}, {0b000001,1}, {0b000000,2}, {0b000001,2}, {0b000010,2}, 	//  0 - 4
	{0b000011,2}, {0b000000,3}, {0b000001,3}, {0b000010,3}, {0b000011,3}, 	//  5 - 9
	{0b000100,3}, {0b000101,3}, {0b000110,3}, {0b000111,3}, {0b000000,4}, 	// 10 - 14
	{0b000001,4}, {0b000010,4}, {0b000011,4}, {0b000100,4}, {0b000101,4}, 	// 15 - 19
	{0b000110,4}, {0b000111,4}, {0b001000,4}, {0b001001,4}, {0b001010,4}, 	// 20 - 24
	{0b001011,4}, {0b001100,4}, {0b001101,4}, {0b001110,4}, {0b001111,4}, 	// 25 - 29
	{0b010101,6}, {0b110011,6}, {0b001100,6}, {0b010010,5}, {0b011010,6}, 	// 30 - 34
	{0b001111,5}, {0b000111,5}, {0b000011,5}, {0b000001,5}, {0b000000,5}, 	// 35 - 39
	{0b010000,5}, {0b011000,5}, {0b011100,5}, {0b011110,5}, {0b011111,5} }; // 41 - 44

uint32_t	ArmPhysicalToVcIO (void* ArmAddress) {
	return	((uint32_t) ArmAddress) + 0x5E000000;
}

void*			VcIOToArmPhysical (uint32_t ArmAddress) {
	return	(void *)(ArmAddress - 0x5E000000);
}

uint32_t	ArmPhysicalToVcMemoryNoL2 (void* ArmAddress) {
	return	((uint32_t) ArmAddress) + 0xC0000000;
}

void*			VcMemoryToArmPhysicalNoL2 (uint32_t ArmAddress) {
	return	(void *)(ArmAddress - 0xC0000000);
}

uint32_t	ArmPhysicalToVcMemoryL2 (void* ArmAddress) {
	return	((uint32_t) ArmAddress) + 0x40000000;
}

void*			VcMemoryToArmPhysicalL2 (uint32_t ArmAddress) {
	return	(void *)(ArmAddress - 0x40000000);
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
 
