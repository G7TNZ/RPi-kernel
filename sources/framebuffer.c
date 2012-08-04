/*
 * framebuffer.c
 * Implementation of the framebuffer for the BCM 2835
 *  GPU graphic screen functions.
 *
 */

#include "headers/framebuffer.h"
#include "headers/standardFont.h"
#include "headers/fontStructure.h"
#include "headers/mailbox.h"
#include "headers/gpio.h"
#include "headers/vectors.h"
#include "headers/colourStructure.h"
#include "headers/general.h"

struct FrameBufferInfo {
	int width;
	int height;
	int pitch;													// pixels per line
	int depth;													// bytes per pixel
	volatile unsigned char* framebuffer;
	int size;														// total number of bytes making up the screen.
	int cursor_x;												// pixel distance down
	int cursor_y;												// pixel distance across
	struct FontStructure fontArrays[2]; //Character pixels
};

struct FrameBuffer {
	uint32_t width;					//(pixels)
	uint32_t height;
	uint32_t virtual_width;
	uint32_t virtual_height;
	uint32_t pitch;					//GPU fills this in; set to zero
	uint32_t depth;					//Bits per pixel; set to 24
	uint32_t x;							//Offset in x direction, set to zero
	uint32_t y;							//Offset in y direction, set to zero
	uint32_t pointer; 			//GPU supplies frame buffer
	uint32_t size;					//GPU supplies
	uint16_t cmap[256];
};

struct Cursor {
	int x;
	int y;
};

struct FrameBufferInfo* frameBufferInfo;
int currentFont;
struct ColourStructure currentForegroundColour;
struct ColourStructure currentBackgroundColour;

// Local prototypes
void WriteCharacterRow(int position, uint32_t pattern, int size);
void WriteCharacter(unsigned char putCharacter);
void MoveScreenUpOneLine(void);
void CheckPosition(void);
void ClearBottomLineToBackground(void);

// Public functions
int InitFb(int mode) {
	// framebuffer structure located where lower 4 bits are zero.
	volatile struct FrameBuffer* frameBuffer = (volatile struct FrameBuffer*) &FramebufferMemory;
	switch (mode) {
		default:
		case MODE640X480X16:
			frameBuffer->width	= 640;
			frameBuffer->height = 480;
			frameBuffer->depth	= 16;
			break;
		case MODE640X480X24:
			frameBuffer->width	= 640;
			frameBuffer->height = 480;
			frameBuffer->depth	= 24;
			break;
		case MODE640X480X32:
			frameBuffer->width	= 640;
			frameBuffer->height = 480;
			frameBuffer->depth	= 32;
			break;
		case MODE1024X768X16:
			frameBuffer->width	= 1024;
			frameBuffer->height = 768;
			frameBuffer->depth	= 16;
			break;
		case MODE1024X768X24:
			frameBuffer->width	= 1024;
			frameBuffer->height = 768;
			frameBuffer->depth	= 24;
			break;
		case MODE1024X768X32:
			frameBuffer->width	= 1024;
			frameBuffer->height = 768;
			frameBuffer->depth	= 32;
			break;
		case MODE1280X1024X16:
			frameBuffer->width	= 1280;
			frameBuffer->height = 1024;
			frameBuffer->depth	= 16;
			break;
		case MODE1280X1024X24:
			frameBuffer->width	= 1280;
			frameBuffer->height = 1024;
			frameBuffer->depth	= 24;
			break;
		case MODE1280X1024X32:
			frameBuffer->width	= 1280;
			frameBuffer->height = 1024;
			frameBuffer->depth	= 32;
			break;
	}
	
	frameBuffer->virtual_width = frameBuffer->width;
	frameBuffer->virtual_height = frameBuffer->height;
	frameBuffer->pitch = 0;
	frameBuffer->x = 0;
	frameBuffer->y = 0;
	frameBuffer->pointer = 0;
	frameBuffer->size = 0;

	DataSynchronisationBarrier();
	MailboxWrite(((uint32_t)ArmPhysicalToVcMemoryL2((void*)frameBuffer) >> 4), CHANNEL_FB);
	uint32_t valueRead = MailboxRead(CHANNEL_FB);
	DataSynchronisationBarrier();
	
	frameBufferInfo->width = frameBuffer->width;
	frameBufferInfo->height = frameBuffer->height;
	frameBufferInfo->pitch = frameBuffer->pitch;
	frameBufferInfo->depth = frameBuffer->depth / 8;
	frameBufferInfo->framebuffer = (volatile unsigned char *) VcMemoryToArmPhysicalL2(frameBuffer->pointer);
	frameBufferInfo->size = frameBuffer->size;
	frameBufferInfo->cursor_x = 0;
	frameBufferInfo->cursor_y = 2;
	StandardFontInitialise();
	frameBufferInfo->fontArrays[0] = (struct FontStructure) standardFont;
	
	struct FontStructure nullFont;
	nullFont.fontname = "end";
	nullFont.width		= 0;
	nullFont.height		= 0;
	nullFont.start		= 0;
	nullFont.end			= 0;
	nullFont.map			= (void*) 0 ;
	frameBufferInfo->fontArrays[1] = (struct FontStructure) nullFont;	
	
	currentFont = 0;
	currentBackgroundColour.red		= 0x00; // make color black
	currentBackgroundColour.green	= 0x00;
	currentBackgroundColour.blue	= 0x00;
	currentBackgroundColour.red		= 0xD0; // intention is Sand.
	currentBackgroundColour.green	= 0xC0;
	currentBackgroundColour.blue	= 0x90;
	
	if (valueRead) { return -1; }
	if (0 == frameBuffer->pointer) { return -2; }
	return 0;
}

void Fb_ChangeFont(char* newFont) {
	// compare string with elements and set to the index.
	currentFont = 0; // default value.
}

void Fb_WriteCharacterInFont(unsigned char putCharacter, char* characterFont) {

}

void Fb_WriteNewLine(void) {
	Fb_WriteString("\n\r");
}

void Fb_WriteString(const char * putString) {
	int index = 0;
	while (0 != putString[index]) {
		Fb_WriteCharacter(putString[index++]);
	}
}

void Fb_WriteCharacter(unsigned char putCharacter) {
	switch (putCharacter) {
		case 0x00:
		case 0x01:
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x08:
		case 0x0C:
		case 0x0E:
		case 0x0F:
		case 0x10:
		case 0x11:
		case 0x12:
		case 0x13:
		case 0x14:
		case 0x15:
		case 0x16:
		case 0x17:
		case 0x18:
		case 0x19:
		case 0x1A:
		case 0x1C:
		case 0x1D:
		case 0x1E:
		case 0x1F:
			// No action at present.
			break;
		case 0x07:	// Bell
			break;
		case 0x09:	// Horizontal tab
			break;
		case 0x0A:	// Line down
				frameBufferInfo->cursor_y += ((frameBufferInfo->fontArrays[currentFont]).height) +1;
			break;
		case 0x0B:	// Vertical tab
			break;
		case 0x0D:	// Line return
			frameBufferInfo->cursor_x = 0;
			break;
		case 0x1B:	// Escape
			break;
		case 0x7F:	// Delete
			frameBufferInfo->cursor_x -= (((frameBufferInfo->fontArrays[currentFont]).width) * frameBufferInfo->depth);
			WriteCharacter(putCharacter);
			break;
		default:
			if (putCharacter <= (frameBufferInfo->fontArrays[currentFont]).end) {
				WriteCharacter(putCharacter);
				frameBufferInfo->cursor_x += (((frameBufferInfo->fontArrays[currentFont]).width) * frameBufferInfo->depth);
				CheckPosition();
			}
	}
}

void Fb_ClearScreen(void) {
	DataSynchronisationBarrier();
	for (unsigned int i = 0; i < frameBufferInfo->size; i++) {
		switch(i % 3) {
			// assumes 24 bit mode.
			case 0: frameBufferInfo->framebuffer[i] = currentBackgroundColour.red;
				break;
			case 1: frameBufferInfo->framebuffer[i] = currentBackgroundColour.green;
				break;
			case 2: frameBufferInfo->framebuffer[i] = currentBackgroundColour.blue;
				break;
		}
	}
	frameBufferInfo->cursor_x = 0;
	frameBufferInfo->cursor_y = 2;
}

void Fb_SetBackgroundColour(struct ColourStructure colour) {
	currentBackgroundColour.red		= colour.red;	
	currentBackgroundColour.green	= colour.green;	
	currentBackgroundColour.blue	= colour.blue;	
}

void Fb_SetForegroundColour(struct ColourStructure colour) {
	currentForegroundColour.red		= colour.red;	
	currentForegroundColour.green	= colour.green;	
	currentForegroundColour.blue	= colour.blue;	
}

void Fb_SetCursorPosition(int x_position, int y_position) {
	frameBufferInfo->cursor_x = x_position * frameBufferInfo->depth;
	frameBufferInfo->cursor_y = y_position;
}

struct Cursor Fb_GetCursorPosition(struct Cursor cursor) {
	cursor.x = frameBufferInfo->cursor_x;
	cursor.y = frameBufferInfo->cursor_y;
	return cursor;
}

/*
 * Local functions
 */

void MoveScreenUpOneLine(void) {
	unsigned int destination;
	unsigned int source;
	unsigned int offset = (frameBufferInfo->pitch * ((frameBufferInfo->fontArrays[currentFont]).height + 1));
	DataSynchronisationBarrier();
	for(destination = 0; destination < (frameBufferInfo->size - offset); destination++) {
		source = destination + offset;
		frameBufferInfo->framebuffer[destination] = frameBufferInfo->framebuffer[source];
	}
	ClearBottomLineToBackground();
}

void ClearBottomLineToBackground(void) {
	DataSynchronisationBarrier();
	for (unsigned int i = (frameBufferInfo->size - (frameBufferInfo->pitch * (frameBufferInfo->fontArrays[currentFont].height+2))); i <= frameBufferInfo->size; i++) {
		switch(i % 3) {
			// assumes 24 bit mode.
			case 0: frameBufferInfo->framebuffer[i] = currentBackgroundColour.red;
				break;
			case 1: frameBufferInfo->framebuffer[i] = currentBackgroundColour.green;
				break;
			case 2: frameBufferInfo->framebuffer[i] = currentBackgroundColour.blue;
				break;
		}
	}
}

void WriteCharacter(unsigned char putCharacter) {
	uint32_t rowPattern;
	int position = 0;
	int base_position = (frameBufferInfo->cursor_y * frameBufferInfo->pitch) + frameBufferInfo->cursor_x ; 
	DataSynchronisationBarrier();
	for (int row = 0; row < frameBufferInfo->fontArrays[currentFont].height; row++) {
		rowPattern = frameBufferInfo->fontArrays[currentFont]
									.map[((putCharacter - ((frameBufferInfo->fontArrays[currentFont]).start))
									   * frameBufferInfo->fontArrays[currentFont].width) + row];
		position = base_position + (row * frameBufferInfo->pitch);
		WriteCharacterRow(position, rowPattern, (frameBufferInfo->fontArrays[currentFont]).width);
	}
	position = base_position + ((frameBufferInfo->fontArrays[currentFont].height) * frameBufferInfo->pitch);
	for (int i = 0; i </*=*/ (frameBufferInfo->fontArrays[currentFont]).width; i++) {
		frameBufferInfo->framebuffer[position++] = currentBackgroundColour.red; // Inter-row pixels
		frameBufferInfo->framebuffer[position++] = currentBackgroundColour.green;
		frameBufferInfo->framebuffer[position++] = currentBackgroundColour.blue;	
	}
}

void CheckPosition(void) {
	if (frameBufferInfo->cursor_x > (frameBufferInfo->pitch - (frameBufferInfo->fontArrays[currentFont]).width - 1)) {
		frameBufferInfo->cursor_x = 0;
		frameBufferInfo->cursor_y += ((frameBufferInfo->fontArrays[currentFont]).height) + 1;
	}
	if (frameBufferInfo->cursor_y >= (frameBufferInfo->size / frameBufferInfo->pitch) - ((frameBufferInfo->fontArrays[currentFont]).height + 2) ) {
		MoveScreenUpOneLine();
		frameBufferInfo->cursor_y -= ((frameBufferInfo->fontArrays[currentFont]).height) + 1;
	}
	if(frameBufferInfo->cursor_x < 0) { 										// Later make it go up a line
		frameBufferInfo->cursor_x = 0;
	}
}
void WriteCharacterRow(int position, uint32_t pattern, int size) {
	DataSynchronisationBarrier();
	for(int i = size-1; i >= 0; i--) {
		if (CheckBit(pattern, i)) {
			frameBufferInfo->framebuffer[position++] = currentForegroundColour.red;
			frameBufferInfo->framebuffer[position++] = currentForegroundColour.green;
			frameBufferInfo->framebuffer[position++] = currentForegroundColour.blue;
		} else {
			frameBufferInfo->framebuffer[position++] = currentBackgroundColour.red;
			frameBufferInfo->framebuffer[position++] = currentBackgroundColour.green;
			frameBufferInfo->framebuffer[position++] = currentBackgroundColour.blue;
		}
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
