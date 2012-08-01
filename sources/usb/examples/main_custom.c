/*
	LPCUSB, an USB device driver for LPC microcontrollers	
	Copyright (C) 2006 Bertrik Sikken (bertrik@sikken.nl)

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.
	3. The name of the author may not be used to endorse or promote products
	   derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
	This is a very simple custom device (not belonging to a specific USB
	class). It implements primitive read and write in the ARM memory space.
	
	Each transfer is initiated by a control transfer to inform the device
	about the address and size of the following data transfer.
	The data transfer takes place over a bulk endpoint (BULK_IN_EP for
	reads and BULK_OUT_EP for writes).

	This example can be used to measure USB transfer speed.
*/

#include "type.h"
#include "usbdebug.h"

#include "console.h"
#include "usbapi.h"
#include "startup.h"


#define BULK_IN_EP		0x82
#define BULK_OUT_EP		0x05

#define MAX_PACKET_SIZE	64

#define LE_WORD(x)		((x)&0xFF),((x)>>8)


static const U8 abDescriptors[] = {

/* Device descriptor */
	0x12,              		
	DESC_DEVICE,       		
	LE_WORD(0x0200),		// bcdUSB	
	0xFF,              		// bDeviceClass
	0x00,              		// bDeviceSubClass
	0x00,              		// bDeviceProtocol
	MAX_PACKET_SIZE0,  		// bMaxPacketSize
	LE_WORD(0xFFFF),		// idVendor
	LE_WORD(0x0004),		// idProduct
	LE_WORD(0x0100),		// bcdDevice
	0x01,              		// iManufacturer
	0x02,              		// iProduct
	0x03,              		// iSerialNumber
	0x01,              		// bNumConfigurations

// configuration
	0x09,
	DESC_CONFIGURATION,
	LE_WORD(0x20),  		// wTotalLength
	0x01,  					// bNumInterfaces
	0x01,  					// bConfigurationValue
	0x00,  					// iConfiguration
	0x80,  					// bmAttributes
	0x32,  					// bMaxPower

// interface
	0x09,   				
	DESC_INTERFACE, 
	0x00,  		 			// bInterfaceNumber
	0x00,   				// bAlternateSetting
	0x02,   				// bNumEndPoints
	0xFF,   				// bInterfaceClass
	0x00,   				// bInterfaceSubClass
	0x00,   				// bInterfaceProtocol
	0x00,   				// iInterface

// bulk in
	0x07,   		
	DESC_ENDPOINT,   		
	BULK_IN_EP,				// bEndpointAddress
	0x02,   				// bmAttributes = BULK
	LE_WORD(MAX_PACKET_SIZE),// wMaxPacketSize
	0,						// bInterval   		

// bulk out
	0x07,   		
	DESC_ENDPOINT,   		
	BULK_OUT_EP,			// bEndpointAddress
	0x02,   				// bmAttributes = BULK
	LE_WORD(MAX_PACKET_SIZE),// wMaxPacketSize
	0,						// bInterval   		

// string descriptors
	0x04,
	DESC_STRING,
	LE_WORD(0x0409),

	// manufacturer string
	0x0E,
	DESC_STRING,
	'L', 0, 'P', 0, 'C', 0, 'U', 0, 'S', 0, 'B', 0,

	// product string
	0x1A,
	DESC_STRING,
	'M', 0, 'e', 0, 'm', 0, 'o', 0, 'r', 0, 'y', 0, 'A', 0, 'c', 0, 'c', 0, 'e', 0, 's', 0, 's', 0,

	// serial number string
	0x12,
	DESC_STRING,
	'D', 0, 'E', 0, 'A', 0, 'D', 0, 'C', 0, '0', 0, 'D', 0, 'E', 0,
	
	// terminator
	0
};


typedef struct {
	U32		dwAddress;
	U32		dwLength;
} TMemoryCmd;


static TMemoryCmd	MemoryCmd;
static U8			abVendorReqData[sizeof(TMemoryCmd)];


static void _HandleBulkIn(U8 bEP, U8 bEPStatus)
{
	int iChunk;
	
	iChunk = MIN(MAX_PACKET_SIZE, MemoryCmd.dwLength);
	if (iChunk == 0) {
		DBG("done\n");
		return;
	}
	
	// send next part
	USBHwEPWrite(bEP, (U8 *)MemoryCmd.dwAddress, iChunk);
	
	MemoryCmd.dwAddress += iChunk;
	MemoryCmd.dwLength -= iChunk;

	// limit address range to prevent abort
	MemoryCmd.dwAddress &= ~(-512 * 1024);
}


static void _HandleBulkOut(U8 bEP, U8 bEPStatus)
{
	int iChunk;
	
	// get next part
	iChunk = USBHwEPRead(bEP, NULL, 0);
	
	MemoryCmd.dwAddress += iChunk;
	MemoryCmd.dwLength -= iChunk;	

	if (MemoryCmd.dwLength == 0) {
		DBG("done\n");
	}
}


/*************************************************************************
	HandleVendorRequest
	===================
		Handles vendor specific requests
		
	Control transfer fields:
	* request:	0x01 = prepare memory read
				0x02 = prepare memory write
	* index:	ignored
	* value:	ignored
	* data:		U32 dwAddress
				U32 dwLength
		
**************************************************************************/
static BOOL HandleVendorRequest(TSetupPacket *pSetup, int *piLen, U8 **ppbData)
{
	TMemoryCmd	*pCmd;
	
	pCmd = (TMemoryCmd *)*ppbData;

	switch (pSetup->bRequest) {
	
	// prepare read
	case 0x01:
		MemoryCmd = *pCmd;
		DBG("READ: addr=%X, len=%d\n", MemoryCmd.dwAddress, MemoryCmd.dwLength);
		// send initial packet
		_HandleBulkIn(BULK_IN_EP, 0);
		*piLen = 0;
		break;
		
	// prepare write
	case 0x02:
		MemoryCmd = *pCmd;
		DBG("WRITE: addr=%X, len=%d\n", MemoryCmd.dwAddress, MemoryCmd.dwLength);
		*piLen = 0;
		break;

	default:
		DBG("Unhandled class %X\n", pSetup->bRequest);
		return FALSE;
	}
	return TRUE;
}




#define BAUD_RATE	115200


/*************************************************************************
	main
	====
**************************************************************************/
int main(void)
{
	// PLL and MAM
	Initialize();

	// init DBG
	ConsoleInit(60000000 / (16 * BAUD_RATE));

	DBG("Initialising USB stack\n");
	
	// initialise stack
	USBInit();
	
	// register device descriptors
	USBRegisterDescriptors(abDescriptors);

	// override standard request handler
	USBRegisterRequestHandler(REQTYPE_TYPE_VENDOR, HandleVendorRequest, abVendorReqData);

	// register endpoints
	USBHwRegisterEPIntHandler(BULK_IN_EP, _HandleBulkIn);
	USBHwRegisterEPIntHandler(BULK_OUT_EP, _HandleBulkOut);

	DBG("Starting USB communication\n");

	// connect to bus
	USBHwConnect(TRUE);

	// call USB interrupt handler continuously
	while (1) {
		USBHwISR();
	}
	
	return 0;
}

