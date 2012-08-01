/*
 * mailbox.h
 *
 */
 
#ifndef _H_MAILBOX
#define _H_MAILBOX

#include <stdint.h>
#include "generalIO.h"

#define MAILBOX_BASE		0x0000B880 

// Offsets for mailboxes
#define CHANNEL_POWER		0x00
#define CHANNEL_FB			0x01			// The frame buffer uses channel 1
#define CHANNEL_VUART		0x02
#define CHANNEL_VCHIQ		0x03
#define CHANNEL_LEDS		0x04
#define CHANNEL_BUTTONS	0x05
#define CHANNEL_TOUCH		0x06

#define MAILBOX0 				0x00
#define MAILBOX1 				0x20

// Offsets for all mailbox registers
#define MAILBOX_READ		0x00
#define MAILBOX_PEEK		0x10	// read channel without removing data from it.
#define MAILBOX_SENDER	0x14	// sender ID (bottom 2-bits only)
#define MAILBOX_STATUS	0x18
#define MAILBOX_CONFIG	0x1C
#define MAILBOX_WRITE		0x20

// Status flags
#define MAILBOX_FULL	0x80000000	// Set if no space to write to mailbox
#define MAILBOX_EMPTY	0x40000000	// Set if nothing to read from mailbox

// Masks
#define MASK_MAILBOX_CHANNEL	0x0000000F
#define MASK_MAILBOX_DATA 		0xFFFFFFF0

// functions
#define MailboxAddress(mailboxNumber, mailboxOffset) (ARM_PHYSICAL_IO_BASE + MAILBOX_BASE + mailboxOffset + mailboxNumber)
#define MailboxData(data, mailboxChannel) ((data << 4) & (mailboxChannel & MASK_MAILBOX_CHANNEL))

uint32_t	MailboxStatus();
uint32_t	MailboxRead(uint32_t mailboxID);
uint32_t	MailboxReadWithChannel(uint32_t mailboxNumber, uint32_t mailboxID);
void			MailboxWrite(uint32_t value, uint32_t mailboxID);
void			MailboxWriteWithChannel(uint32_t value, uint32_t mailboxNumber, uint32_t mailboxID);

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
