/*
 * atags.h
 *
 */

#ifndef _H_ATAGS
#define _H_ATAGS

#include <stdint.h>

typedef struct {
	char*	key;
	char*	value;
} CommandLineKeys;

struct BootParameters {
	uint32_t	machineType;
	uint32_t	atagsAddress;
	int				dmaChannnels;
	int				fbWidth;
	int				fbHeight;
	int				fbDepth;
	char			macAddress[18];
	uint32_t	serialNumber;
	uint32_t	boardRevision;
	CommandLineKeys* commandLineParameters;	
} bootParameters;

struct bootParameters;

void Atags_Init(void);
 
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
