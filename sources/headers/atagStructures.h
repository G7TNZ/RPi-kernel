/*
 * atagStructures.h
 *
 */

#ifndef _H_ATAGSTRUCTURES
#define _H_ATAGSTRUCTURES

#include <stdint.h>
#include "types.h"

typedef struct {
	uint32_t	flags;
	uint32_t	pageSize;
	uint32_t	rootDevice;
} AtagCore;

typedef struct {
	uint32_t	size;
	uint32_t	start;
} AtagMemory;

typedef struct {
	uint8_t		x;
	uint8_t		y;
	uint16_t	page;
	uint8_t		mode;
	uint8_t		columns;
	uint16_t	egaBx;	
	uint8_t		lines;
	uint8_t		isVga;
	uint16_t	points;	
} AtagVideotext;

typedef struct {
	uint32_t	flags;
	uint32_t	size;
	uint32_t	start;
} AtagRamdisk;

typedef struct {
	uint32_t	start;
	uint32_t	size;
} AtagInitrd2;

typedef struct {
	uint32_t	low;
	uint32_t	high;
} AtagSerialNumber;

typedef struct {
	uint32_t	revision;
} AtagBoardRevision;

typedef struct {
	uint16_t	lfbWidth;
	uint16_t	lfbHeight;
	uint16_t	lfbDepth;
	uint16_t	lfbLineLength;
	uint32_t	lfbBase;
	uint32_t	lfbSize;	
	uint8_t		redSize;
	uint8_t		redPosition;
	uint8_t		greenSize;
	uint8_t		greenPosition;
	uint8_t		blueSize;
	uint8_t		bluePosition;	
	uint8_t		rsvdSize;
	uint8_t		rsvdPosition;
} AtagVideoLfb;

typedef struct {
	char*			string;
} AtagCommandLine;

typedef struct {
	uint32_t size;
	uint32_t type;
	union {
		AtagCore					core;
		AtagMemory				memory;
		AtagVideotext			videoText;
		AtagRamdisk				ramDisk;
		AtagInitrd2				initrd2;
		AtagSerialNumber	serialNumber;
		AtagBoardRevision	boardRevision;
		AtagVideoLfb			videoLfb;
		AtagCommandLine		commandLine;
	}tagData;
} Atag;

enum AtagCodes {
	ATAGNONE					= 0,
	ATAGCORE					= 5441001,
	ATAGMEMORY				= 5441002,
	ATAGVIDEOTEXT			= 5441003,
	ATAGRAMDISK				= 5441004,
	ATAGINITRD2				= 5442005,
	ATAGSERIALNUMBER	= 5441006,
	ATAGBOARDREVISION	= 5441007,
	ATAGVIDEOLFB			= 5441008,
	ATAGCOMMANDLINE		= 5441009
};

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
