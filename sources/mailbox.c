/*
 * mailbox.c
 * Implementation of the mailboxes for the BCM 2835
 *
 */
 
#include "headers/mailbox.h"
#include "headers/vectors.h"
 
uint32_t MailboxStatus() {
	return ReadFromMemory32(MailboxAddress(MAILBOX0, MAILBOX_STATUS));
}

uint32_t MailboxRead(uint32_t mailboxID) {
	return MailboxReadWithChannel(MAILBOX0, mailboxID);
}

uint32_t MailboxReadWithChannel(uint32_t mailboxNumber, uint32_t mailboxID) {
	uint32_t readValue = 0;
	do {
		while (0 != (MailboxStatus() & MAILBOX_EMPTY));
		DataSynchronisationBarrier();
		readValue = ReadFromMemory32(MailboxAddress(mailboxNumber, MAILBOX_READ ));
	} while ((readValue & MASK_MAILBOX_CHANNEL) != (mailboxID & MASK_MAILBOX_CHANNEL));
	
	return ((readValue & MASK_MAILBOX_DATA) >> 4);
}

void MailboxWrite(uint32_t value, uint32_t mailboxid) {
	MailboxWriteWithChannel(value, MAILBOX0, mailboxid);
}

void MailboxWriteWithChannel(uint32_t value, uint32_t mailboxNumber, uint32_t mailboxid) {
	DataSynchronisationBarrier();
	while (0 != (MailboxStatus() & MAILBOX_FULL));
	WriteToMemory32((value << 4) | (mailboxid & MASK_MAILBOX_CHANNEL), MailboxAddress(mailboxNumber, MAILBOX_WRITE));
	DataSynchronisationBarrier();
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
