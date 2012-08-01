#
# vectors.s
#  startup and other assembler functions.
#
#
.text

.global start_asm
start_asm:
		mrs			r4, cpsr
		bic			r4, r4, #0x1f

		# FIQ mode
		orr			r3, r4, #0x11
		msr			cpsr_c, r3
		ldr			sp, =exc_stack

		# IRQ mode
		orr			r3, r4, #0x12
		msr			cpsr_c, r3
		ldr			sp, =exc_stack

		# ABORT mode
		orr			r3, r4, #0x17
		msr			cpsr_c, r3
		ldr			sp, =exc_stack

		# UNDEFINED mode
		orr			r3, r4, #0x1b
		msr			cpsr_c, r3
		ldr			sp, =exc_stack

		# switch to supervisor mode
		orr			r3, r4, #0x13
		msr			cpsr_c, r3
		ldr			sp, =temp_stack

		#Create the first stack frame
		mov			fp, #0x00
		mov			ip, sp
		push		{fp, ip, lr, pc}
		sub			fp, ip, #4

		# precautionary.
		bl			DataMemoryBarrier
		bl			DataSynchronisationBarrier

		# go for the c-code now
		bl			start_kernel
SafetyLoop:
		b				SafetyLoop

.global WriteToMemory32
WriteToMemory32:
		str			r0,[r1]
		bx			lr

.global ReadFromMemory32
ReadFromMemory32:
		ldr			r0,[r0]
		bx			lr 

.global SmallDelay
SmallDelay:
loop:
		subs		r0,r0,#0x00000001
		bne			loop
		bx			lr

.global VoidCall
VoidCall:
		bx			lr

.global DataMemoryBarrier
DataMemoryBarrier:
		mov			r0, #0x0000
		mcr			p15, 0, r0, c7, c10, 5
		bx			lr

.global MemoryBarrier
MemoryBarrier:
		mcr			p15, 0, ip, c7, c5, 0			@ invalidate I cache
		mcr			p15, 0, ip, c7, c5, 6			@ invalidate BTB
		mcr			p15, 0, ip, c7, c10, 4		@ drain write buffer
		mcr			p15, 0, ip, c7, c5, 4			@ prefetch flush
		mov			pc, lr

.global DataSynchronisationBarrier
DataSynchronisationBarrier:
		stmfd		sp!,{r0-r8,r12,lr}				@ Store registers
		mcr			p15, 0, ip, c7, c5, 0			@ invalidate I cache
		mcr			p15,0, ip, c7, c5, 6			@ invalidate BTB
		mcr			p15,  0, ip, c7, c10, 4		@ drain write buffer
		mcr			p15, 0, ip, c7, c10, 4		@ prefetch flush
		ldmfd		sp!,{r0-r8,r12,pc}				@ restore registers	 and return

.space			0x100
temp_stack:
.space			1024
.global exc_stack
exc_stack:

.global supervisor_sp
supervisor_sp:
.space			4

.data
.p2align 4
.global MemorySpace
MemorySpace:
.space			0x400, 00

.global EndMemory
EndMemory:
.byte				0xAA


.end

#
# Copyright (c) 2012 Ischus Limited www.ischus.com
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software
# and associated documentation files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
# BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
