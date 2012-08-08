#
# morseAscii.s
#  Morse to Ascii tables
#
#

.data

.global digit1
digit1:
.byte			0x45, 0x54, 0x00, 0x00		@ E,T

.global digit2
digit2:
.byte			0x49, 0x41, 0x4E, 0x4D		@ I,A,N,M

.global digit3
digit3:
.byte			0x53, 0x55, 0x52, 0x57		@ S,U,R,W
.byte			0x44, 0x4B, 0x47, 0x4F		@ D,K,G,O

.global digit4
digit4:
.byte			0x48, 0x56, 0x46, 0xDC		@ H,V,F,Uu
.byte			0x4C, 0xC1, 0x50, 0x4A		@ L,Aa,P,J
.byte			0x42, 0x58, 0x43, 0x59		@ B,X,C,Y
.byte			0x5A, 0x51, 0xD6, 0x00		@ Z,Q,Ou,Ch

.global digit5
digit5:
.byte			0x35, 0x34, 0x20, 0x33		@ 5,4,space,3
.byte			0x3E, 0x3C, 0x7B, 0x32		@ >,<,{,2
.byte			0x26, 0xE0, 0x2B, 0xF0		@ &,up,+,r.alt,
.byte			0x0A, 0xC0, 0xA3, 0x31		@ down,Ag,£,1
.byte			0x36, 0x3D, 0x2F, 0x2A		@ 6,=,/,*
.byte			0xF9, 0xFF, 0x28, 0xF1		@ insert,n/a,(,l.alt
.byte			0x37, 0x7D, 0xF4, 0xD1		@ 7,},r.cntrl,n~
.byte			0x38, 0x0D, 0x39, 0x30		@ 8,CR,9,0

.global digit6
digit6:
.byte			0xE9, 0xE8, 0x7C, 0xFF		@ pg.down,pg.up,|,n/a
.byte			0xC9, 0x25, 0xFF, 0xFF		@ Ea,%,n/a,n/a
.byte			0xFF, 0xFF, 0xFF, 0xFF		@ n/a,n/a,n/a,n/a
.byte			0x3F, 0x5F, 0x23, 0xFF		@ ?,_,#,n/a
.byte			0xFF, 0xFF, 0x22, 0xFF		@ n/a,n,a,",n/a
.byte			0xB6, 0x2E, 0x28, 0x58		@ pilcrow,.,(,[
.byte			0x09, 0xFF, 0x40, 0x00		@ r.tab,n/a,@,n/a
.byte			0xE3, 0xE2, 0x27, 0xFF		@ left,right,',n/a
.byte			0xFF, 0x2D, 0xFF, 0x5E		@ n/a,-,n/a,^
.byte			0xFF, 0xFF, 0xFF, 0x5D		@ n/a,n/a,n/a,]
.byte			0xFF, 0xEF, 0x3B, 0x21		@ n/a,CntlAltDel,;,!
.byte			0xF5, 0x29, 0xFF, 0xFF		@ l.cntl,),n/a,n/a
.byte			0x1B, 0xF3, 0xF2, 0x2C		@ escape,l.shft,r.shft,comma
.byte			0xF7, 0xF6, 0xFA, 0x60		@ scr.lk,num.lk,prt.scr,`
.byte			0x3A, 0xE6, 0xE5, 0x7E		@ :,home,l.tab,~
.byte			0x5C, 0xE7, 0xF8, 0x7F		@ \,end,cap.lk,r.del

# digit7 and digit8 best to do with switch.
#		digit7 contains f1-f12 and $
#		digit8 has only <del = bs = 08.

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
