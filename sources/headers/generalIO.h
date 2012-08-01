/*
 * generalIO.h
 *
 */
 
#ifndef _H_GENERALIO
#define _H_GENERALIO

// Base addresses 
#define ARM_PHYSICAL_IO_BASE	0x20000000	// Raw
#define ARM_VIRTUAL_IO_BASE		0xF2000000	// ARM MMU mapped
#define VC_IO_BASE						0x7E000000	// VC MMU mapped

// Flashing patterns for pins.
#define PATTERN_S				0		//E
#define PAT_E						0
#define PATTERN_L				1		//T
#define PAT_T						1
#define PATTERN_SS			2		//I
#define PAT_I						2
#define PATTERN_SL			3		//A
#define PAT_A						3
#define PATTERN_LS			4		//N
#define PAT_N						4
#define PATTERN_LL			5		//M
#define PAT_M						5
#define PATTERN_SSS			6		//S
#define PAT_S						6
#define PATTERN_SSL			7		//U
#define PAT_U						7
#define PATTERN_SLS			8		//R
#define PAT_R						8
#define PATTERN_SLL			9		//W
#define PAT_W						9
#define PATTERN_LSS			10	//D
#define PAT_D						10
#define PATTERN_LSL			11	//K
#define PAT_K						11
#define PATTERN_LLS			12	//G
#define PAT_G						12
#define PATTERN_LLL			13	//O
#define PAT_O						13
#define PATTERN_SSSS		14	//H
#define PAT_H						14
#define PATTERN_SSSL		15	//V
#define PAT_V						15
#define PATTERN_SSLS		16	//F
#define PAT_F						16
#define PATTERN_SSLL		17	//		
#define PATTERN_SLSS		18	//L
#define PAT_L						18
#define PATTERN_SLSL		19	//				
#define PATTERN_SLLS		20	//P
#define PAT_P						20
#define PATTERN_SLLL		21	//J
#define PAT_J						21
#define PATTERN_LSSS		22	//B
#define PAT_B						22
#define PATTERN_LSSL		23	//X
#define PAT_X						23
#define PATTERN_LSLS		24	//C
#define PAT_C						24
#define PATTERN_LSLL		25	//Y
#define PAT_Y						25
#define PATTERN_LLSS		26	//Z
#define PAT_Z						26
#define PATTERN_LLSL		27	//Q
#define PAT_Q						27
#define PATTERN_LLLS		28	//
#define PATTERN_LLLL		29	//						
#define PATTERN_SLSLSL	30	//Period
#define PAT_PERIOD			30			
#define PATTERN_LLSSLL	31	//Comma
#define PAT_COMMA				31		
#define PATTERN_SSLLSS	32	//?
#define PAT_QUESTION		32				
#define PATTERN_LSSLS		33	///
#define PAT_SOLIDUS			33			
#define PATTERN_SLLSLS	34	//@
#define PAT_AT					34	
#define PATTERN_SLLLL		35	//1
#define PAT_1						35
#define PATTERN_SSLLL		36	//2
#define PAT_2						36
#define PATTERN_SSSLL		37	//3
#define PAT_3						37
#define PATTERN_SSSSL		38	//4
#define PAT_4						38
#define PATTERN_SSSSS		39	//5
#define PAT_5						39
#define PATTERN_LSSSS		40	//6
#define PAT_6						40
#define PATTERN_LLSSS		41	//7
#define PAT_7						41
#define PATTERN_LLLSS		42	//8
#define PAT_8						42
#define PATTERN_LLLLS		43	//9
#define PAT_9						43
#define PATTERN_LLLLL		44	//0
#define PAT_0						44

typedef int bool;
#define true 1
#define false 0

uint32_t	ArmPhysicalToVcIO(void* ArmAddress);
uint32_t	ArmPhysicalToVcMemoryNoL2(void* ArmAddress);
uint32_t	ArmPhysicalToVcMemoryL2(void* ArmAddress);
void*			VcMemoryToArmPhysicalNoL2(uint32_t ArmAddress);
void*			VcMemoryToArmPhysicalL2(uint32_t ArmAddress);

int codes [45][2]; // The character definitions

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
 
