/*
 * atags.c
 *
 */

#include <stdint.h>
#include "headers/framebuffer.h"
#include "headers/general.h"
#include "headers/asmStart.h"
#include "headers/atags.h"
#include "headers/atagStructures.h"

#define clk(element) (element * sizeof(CommandLineKeys))

/*
 *	Local prototypes
 *
 */
 
void GetAtagCore(uint32_t address, uint32_t atagSize);
void GetAtagMemory(uint32_t address, uint32_t atagSize);
void GetAtagVideoText(uint32_t address, uint32_t atagSize);
void GetAtagRamDisk(uint32_t address, uint32_t atagSize);
void GetAtagInitRd2(uint32_t address, uint32_t atagSize);
void GetAtagSerialNumber(uint32_t address, uint32_t atagSize);
void GetAtagBoardRevision(uint32_t address, uint32_t atagSize);
void GetAtagVideoLfb(uint32_t address, uint32_t atagSize);
void GetAtagCommandLine(uint32_t address, uint32_t atagSize);

/*
 *	Public methods
 *
 */
 
void Atags_Init(void) {
	bootParameters.machineType	= LinuxMachineType;
	bootParameters.atagsAddress = AtagsAddress;
	
	uint32_t atagAddress = AtagsAddress;
	uint32_t atagSize;
	uint32_t atagCode;
	
	do {
		atagSize	= ReadFromMemory32(atagAddress);
		atagAddress += 4;
		atagCode	= ReadFromMemory32(atagAddress);
		atagAddress += 4;
		switch (atagCode) {
			default:
			case ATAGNONE:
				break;
			case ATAGCORE:
				GetAtagCore(atagAddress, atagSize);
				break;
			case ATAGMEMORY:
				GetAtagMemory(atagAddress, atagSize);
				break;
			case ATAGVIDEOTEXT:
				GetAtagVideoText(atagAddress, atagSize);
				break;
			case ATAGRAMDISK:
				GetAtagRamDisk(atagAddress, atagSize);
				break;
			case ATAGINITRD2:
				GetAtagInitRd2(atagAddress, atagSize);
				break;
			case ATAGSERIALNUMBER:
				GetAtagSerialNumber(atagAddress, atagSize);
				break;
			case ATAGBOARDREVISION:
				GetAtagBoardRevision(atagAddress, atagSize);
				break;
			case ATAGVIDEOLFB:
				GetAtagVideoLfb(atagAddress, atagSize);
				break;
			case ATAGCOMMANDLINE:
				GetAtagCommandLine(atagAddress, atagSize);
				break;
		}
		atagAddress = ((atagSize - 2) * 4);
	} while (ATAGNONE != atagCode);
	
	// test block
	CommandLineKeys* clk = (CommandLineKeys*) AllocateMemory(3 * sizeof(CommandLineKeys));
	
	(clk+clk(0))->key		= "MyKey";
	(clk+clk(0))->value	= "KeyValue";
	(clk+clk(1))->key		= "SecondKey";
	(clk+clk(1))->value	= "SecondValue";
	(clk+clk(2))->key		= "";
	(clk+clk(2))->value	= "";
	bootParameters.commandLineParameters = clk;
	
}

/*
 * Local functions
 *
 */

void GetAtagCore(uint32_t address, uint32_t atagSize) {
	if (2 == atagSize) return;
		bootParameters.flags	= ReadFromMemory32(address);
		address += 4;
		bootParameters.pageSize	= ReadFromMemory32(address);
		address += 4;
		bootParameters.rootDevice	= ReadFromMemory32(address);
		address += 4;
}

void GetAtagMemory(uint32_t address, uint32_t atagSize) {
	bootParameters.memorySize	= ReadFromMemory32(address);
	address += 4;
	bootParameters.memoryStart	= ReadFromMemory32(address);
	address += 4;
}

void GetAtagVideoText(uint32_t address, uint32_t atagSize) {

}

void GetAtagRamDisk(uint32_t address, uint32_t atagSize) {

}

void GetAtagInitRd2(uint32_t address, uint32_t atagSize) {

}

void GetAtagSerialNumber(uint32_t address, uint32_t atagSize) {

}

void GetAtagBoardRevision(uint32_t address, uint32_t atagSize) {

}

void GetAtagVideoLfb(uint32_t address, uint32_t atagSize) {

}

void GetAtagCommandLine(uint32_t address, uint32_t atagSize) {
	for (int i = 0; i < ((atagSize - 2) * 4); i++) {
		
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
