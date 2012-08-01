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

#include "type.h"
#include "usbdebug.h"

#include "console.h"
#include "usbapi.h"
#include "startup.h"


#define INTR_IN_EP		0x81

#define MAX_PACKET_SIZE	64

#define LE_WORD(x)		((x)&0xFF),((x)>>8)

#define REPORT_SIZE			4

static U8	abClassReqData[4];
static U8	abReport[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
static int	_iIdleRate = 0;
static int	_iFrame = 0;



// see the joystick example from the usb.org HID Descriptor Tool
static U8 abReportDesc[] = {
	0x05, 0x01,
	0x15, 0x00,
	0x09, 0x04,
	0xA1, 0x01,
	0x05, 0x02,
	0x09, 0xBB,
	0x15, 0x81,
	0x25, 0x7F,
	0x75, 0x08,
	0x95, 0x01,
	0x81, 0x02,
	0x05, 0x01,
	0x09, 0x01,
	0xA1, 0x00,
	0x09, 0x30,
	0x09, 0x31,
	0x95, 0x02,
	0x81, 0x02,
	0xC0,
	0x09, 0x39,
	0x15, 0x00,
	0x25, 0x03,
	0x35, 0x00,
	0x46, 0x0E, 0x01,
	0x65, 0x14,
	0x75, 0x04,
	0x95, 0x01,
	0x81, 0x02,
	0x05, 0x09,
	0x19, 0x01,
	0x29, 0x04,
	0x15, 0x00,
	0x25, 0x01,
	0x75, 0x01,
	0x95, 0x04,
	0x55, 0x00,
	0x65, 0x00,
	0x81, 0x02,
	0xC0
};


static const U8 abDescriptors[] = {

/* Device descriptor */
	0x12,              		
	DESC_DEVICE,       		
	LE_WORD(0x0110),		// bcdUSB	
	0x00,              		// bDeviceClass
	0x00,              		// bDeviceSubClass
	0x00,              		// bDeviceProtocol
	MAX_PACKET_SIZE0,  		// bMaxPacketSize
	LE_WORD(0xFFFF),		// idVendor
	LE_WORD(0x0001),		// idProduct
	LE_WORD(0x0100),		// bcdDevice
	0x01,              		// iManufacturer
	0x02,              		// iProduct
	0x03,              		// iSerialNumber
	0x01,              		// bNumConfigurations

// configuration
	0x09,
	DESC_CONFIGURATION,
	LE_WORD(0x22),  		// wTotalLength
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
	0x01,   				// bNumEndPoints
	0x03,   				// bInterfaceClass = HID
	0x00,   				// bInterfaceSubClass
	0x00,   				// bInterfaceProtocol
	0x00,   				// iInterface

// HID descriptor
	0x09, 
	DESC_HID_HID, 			// bDescriptorType = HID
	LE_WORD(0x0110),		// bcdHID
	0x00,   				// bCountryCode
	0x01,   				// bNumDescriptors = report
	DESC_HID_REPORT,   		// bDescriptorType
	LE_WORD(sizeof(abReportDesc)),

// EP descriptor
	0x07,   		
	DESC_ENDPOINT,   		
	INTR_IN_EP,				// bEndpointAddress
	0x03,   				// bmAttributes = INT
	LE_WORD(MAX_PACKET_SIZE),// wMaxPacketSize
	10,						// bInterval   		

// string descriptors
	0x04,
	DESC_STRING,
	LE_WORD(0x0409),

	// manufacturer string
	0x0E,
	DESC_STRING,
	'L', 0, 'P', 0, 'C', 0, 'U', 0, 'S', 0, 'B', 0,

	// product string
	0x12,
	DESC_STRING,
	'P', 0, 'r', 0, 'o', 0, 'd', 0, 'u', 0, 'c', 0, 't', 0, 'X', 0,

	// serial number string
	0x12,
	DESC_STRING,
	'D', 0, 'E', 0, 'A', 0, 'D', 0, 'C', 0, '0', 0, 'D', 0, 'E', 0,
	
	// terminator
	0
};


/*************************************************************************
	HandleClassRequest
	==================
		HID class request handler
		
**************************************************************************/
static BOOL HandleClassRequest(TSetupPacket *pSetup, int *piLen, U8 **ppbData)
{
	U8	*pbData = *ppbData;

	switch (pSetup->bRequest) {
	
   	// get_idle
	case HID_GET_IDLE:
		DBG("GET IDLE, val=%X, idx=%X\n", pSetup->wValue, pSetup->wIndex);
		pbData[0] = (_iIdleRate / 4) & 0xFF;
		*piLen = 1;
		break;

	// set_idle:
	case HID_SET_IDLE:
		DBG("SET IDLE, val=%X, idx=%X\n", pSetup->wValue, pSetup->wIndex);
		_iIdleRate = ((pSetup->wValue >> 8) & 0xFF) * 4;
		break;

	default:
		DBG("Unhandled class %X\n", pSetup->bRequest);
		return FALSE;
	}
	return TRUE;
}


#define BAUD_RATE	115200


/*************************************************************************
	HIDHandleStdReq
	===============
		Standard request handler for HID devices.
		
	This function tries to service any HID specific requests.
		
**************************************************************************/
static BOOL HIDHandleStdReq(TSetupPacket *pSetup, int *piLen, U8 **ppbData)
{
	U8	bType, bIndex;

	if ((pSetup->bmRequestType == 0x81) &&			// standard IN request for interface
		(pSetup->bRequest == REQ_GET_DESCRIPTOR)) {	// get descriptor
		
		bType = GET_DESC_TYPE(pSetup->wValue);
		bIndex = GET_DESC_INDEX(pSetup->wValue);
		switch (bType) {

		case DESC_HID_REPORT:
			// report
			*ppbData = abReportDesc;
			*piLen = sizeof(abReportDesc);
			break;

		case DESC_HID_HID:
		case DESC_HID_PHYSICAL:
		default:
		    // search descriptor space
		    return USBGetDescriptor(pSetup->wValue, pSetup->wIndex, piLen, ppbData);
		}
		
		return TRUE;
	}
	return FALSE;
}


static void HandleFrame(U16 wFrame)
{
	static int iCount;

	_iFrame++;
	if ((_iFrame > 1000)) {
		// send report (dummy data)
		abReport[0] = (iCount >> 8) & 0xFF;
		abReport[1] = (iCount) & 0xFF;
		iCount++;
		USBHwEPWrite(INTR_IN_EP, abReport, REPORT_SIZE);
		_iFrame = 0;
	}
}


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

	// register HID standard request handler
	USBRegisterCustomReqHandler(HIDHandleStdReq);

	// register class request handler
	USBRegisterRequestHandler(REQTYPE_TYPE_CLASS, HandleClassRequest, abClassReqData);

	// register endpoint
	USBHwRegisterEPIntHandler(INTR_IN_EP, NULL);

	// register frame handler
	USBHwRegisterFrameHandler(HandleFrame);

	DBG("Starting USB communication\n");

	// connect to bus
	USBHwConnect(TRUE);

	// call USB interrupt handler continuously
	while (1) {
		USBHwISR();
	}
	
	return 0;
}

