;
; ENC28J60 Driver for SBC-3
; Written by Daryl Rictor (c) 2010
;

	.include	"enceth.inc"
	.include	"sbc3.inc"
	.import	_uip_buf
	.import	_uip_appdata
	.import     _uip_len
	.importzp	ptr1,ptr2
	.export	_init_ether 
	.export	_send_ether 
	.export	_rcv_ether 

;
; constants
;

SPIeth	=  	$08		; uses SBC-3 SPI Port 3 '00001000'


;
; 65SPI register addresses (using a modified 65SPI for SBC-3)
;

SPIDR		:=	$080020	; SPI data port
SPISR		:=	$080021	; SPI status
SPICR		:=	$080021	; SPI control
SPIDIV	:=	$080022	; SPI SCLK divisor (6 bits)
SPISSR	:=	$080023	; SPI Slave Select register
SPIISR	:=	$080024	; SPI Interrupt status register


.code

;############################## Internal Functions ############################

;*********************************************************************
; Read ENC28J60 Control Register
; Address is X, return value in A
;
EthReadReg:
		lda	#255-SPIeth		;
		AND	SPISSR		; select eth SS
		STA	SPISSR
		TXA
		and	#ADDR_MASK		; mask address
		STA	SPIDR			; read address command
@1:		LDA	SPISR			; 
		bpl	@1			; wait for tx to end
		LDA	SPIDR			; clr SPISR bit 7
		lda	#$00			; dummy command
		STA	SPIDR			; send it
		txa
		and	#$80
		beq	@3
@2:		LDA	SPISR			; 
		bpl	@2			; wait for tx to end
		LDA	SPIDR			; clr SPISR bit 7
		lda	#$00			; dummy read for MAC and MII regs
		STA	SPIDR			; send it
@3:		LDA	SPISR			; 
		bpl	@3			; wait for tx to end
		LDA	SPIDR			; read eth data
		pha				; save parm in
		lda	#SPIeth		; 
		ORA	SPISSR		; deselect eth
		STA	SPISSR
		pla
		rts

;*********************************************************************
; Read ENC28J60 Control Register
; Address is X, return value in A
;
EthReadRaw:
		lda	#255-SPIeth		;
		AND	SPISSR		; select eth SS
		STA	SPISSR
		TXA
		STA	SPIDR			; read address command
@1:		LDA	SPISR			; 
		bpl	@1			; wait for tx to end
		LDA	SPIDR			; clr SPISR bit 7
		lda	#$00			; dummy command
		STA	SPIDR			; send it
@2:		LDA	SPISR			; 
		bpl	@2			; wait for tx to end
		LDA	SPIDR			; read eth data
		pha				; save parm in
		lda	#SPIeth		; 
		ORA	SPISSR		; deselect eth
		STA	SPISSR
		pla
		rts

;*********************************************************************
; Write ENC28J60 Control Register
; Address is X, value in A
;
EthWriteReg:
		pha				; save value
		lda	#255-SPIeth		;
		AND	SPISSR		; select eth SS
		STA	SPISSR
		TXA
		and	#ADDR_MASK		; mask address
		ora	#$40			; write control reg
		STA	SPIDR			; read address command
@1:		LDA	SPISR			; 
		bpl	@1			; wait for tx to end
		LDA	SPIDR			; clr SPISR bit 7
		pla				; get value
		STA	SPIDR			; send it
@2:		LDA	SPISR			; 
		bpl	@2			; wait for tx to end
		LDA	SPIDR			; clr SPISR bit 7
		lda	#SPIeth		; 
		ORA	SPISSR		; deselect eth
		STA	SPISSR
		rts

;*********************************************************************
; Write ENC28J60 raw
; command+Address is X, value in A
;
EthWriteRaw:
		pha				; save value
		lda	#255-SPIeth		;
		AND	SPISSR		; select eth SS
		STA	SPISSR
		TXA
		STA	SPIDR			; read address command
@1:		LDA	SPISR			; 
		bpl	@1			; wait for tx to end
		LDA	SPIDR			; clr SPISR bit 7
		pla				; get value
		STA	SPIDR			; send it
@2:		LDA	SPISR			; 
		bpl	@2			; wait for tx to end
		LDA	SPIDR			; clr SPISR bit 7
		lda	#SPIeth		; 
		ORA	SPISSR		; deselect eth
		STA	SPISSR
		rts

;*********************************************************************
; Write ENC28J60 Page Register
; Page Address is X
;
EthWritePage:
		lda	#255-SPIeth		;
		AND	SPISSR		; select eth SS
		STA	SPISSR
		lda	#$BF			; bit field clear ECON1 command
		STA	SPIDR			; read address command
@1:		LDA	SPISR			; 
		bpl	@1			; wait for tx to end
		LDA	SPIDR			; clr SPISR bit 7
		lda	#$03			; clear bits 0 & 1
		STA	SPIDR			; send it
@2:		LDA	SPISR			; 
		bpl	@2			; wait for tx to end
		LDA	SPIDR			; read eth data
		lda	#SPIeth		; 
		ORA	SPISSR		; deselect eth
		STA	SPISSR
		txa
		rol				; shift 
		rol
		rol
		rol
		and	#$03			; mask page bits
		beq	@5			; if page=0, we're done
		pha
		lda	#255-SPIeth		;
		AND	SPISSR		; select eth SS
		STA	SPISSR
		lda	#$9F			; bit field set ECON1
		STA	SPIDR			; 
@3:		LDA	SPISR			; 
		bpl	@3			; wait for tx to end
		LDA	SPIDR			; clr SPISR bit 7
		pla				; get value
		STA	SPIDR			; send it
@4:		LDA	SPISR			; 
		bpl	@4			; wait for tx to end
		LDA	SPIDR			; clr SPISR bit 7
		lda	#SPIeth		; 
		ORA	SPISSR		; deselect eth
		STA	SPISSR
@5:		rts

;*********************************************************************
; Read buffer
; ptr1 = data pointer
; ptr2 = len
ReadBuffer:
		lda	#255-SPIeth		;
		AND	SPISSR		; select eth SS
		STA	SPISSR
		lda	#$3A			; read buffer mem command
		STA	SPIDR			; 
@1:		LDA	SPISR			; 
		bpl	@1			; wait for tx to end
		LDA	SPIDR			; clr SPISR bit 7
@2:		lda	#$00			; dummy command
		STA	SPIDR			; send it
@3:		LDA	SPISR			; 
		bpl	@3			; wait for tx to end
		LDA	SPIDR			; read data
		sta	(ptr1)		; store it
		rep	#$20			; 16 bit mode
		inc	ptr1			; inc pointer
		dec	ptr2              ; dec counter
		sep	#$20			; 8 bit mode
		bne	@2			; repeat until 0
		lda	#SPIeth		; 
		ORA	SPISSR		; deselect eth
		STA	SPISSR
		rts

;*********************************************************************
; Write buffer
; 
WriteBuffer:
		lda	#255-SPIeth		;
		AND	SPISSR		; select eth SS
		STA	SPISSR
		lda	#$7A			; write buffer mem command
		STA	SPIDR			; 
@1:		LDA	SPISR			; 
		bpl	@1			; wait for tx to end
		LDA	SPIDR			; clr SPISR bit 7
@2:		lda	(ptr1)		; get data
		STA	SPIDR			; send it
@3:		LDA	SPISR			; 
		bpl	@3			; wait for tx to end
		LDA	SPIDR			; clr SPISR bit 7
		rep	#$20			; 16 bit mode
		inc	ptr1			; inc pointer
		dec	ptr2              ; dec counter
		sep	#$20			; 8 bit mode
		bne	@2			; repeat until 0
		lda	#SPIeth		; 
		ORA	SPISSR		; deselect eth
		STA	SPISSR
		rts


;*********************************************************************
; PhyRead 
; address in X
; returns int in a,x
;
;PhyRead:
;		phx
;		ldx	#MIREGADR		; page 2	
;		jsr	EthWritePage	; 
;		pla
;		jsr	EthWriteReg		; write address to MIREGADR
;		ldx	#MICMD
;		lda	#MICMD_MIIRD	; read command
;		jsr	EthWriteReg
;		ldx	#MISTAT		; page 3
;		jsr	EthWritePage	
;@1:		jsr	EthReadReg		; MI status
;		and	#MISTAT_BUSY	
;		bne	@1			; wait for not busy
;		ldx	#MICMD		; page 2
;		jsr	EthWritePage
;		lda	#$00
;		jsr	EthWriteReg		; stop reading
;		ldx	#MIRDL
;		jsr	EthReadReg
;		pha
;		ldx	#MIRDH
;		jsr	EthReadReg
;		pha
;		ldx	#ERDPTL		; page 0
;		jsr	EthWritePage
;		plx
;		pla
;		rts

;*********************************************************************
; PHYWrite
; Address in X, data in a,y 
;
PhyWrite:
		pha
		phx
		ldx	#MIREGADR		; page 2	
		jsr	EthWritePage	; 
		pla				; address
		jsr	EthWriteReg		; write address to MIREGADR
		pla
		LDX	#MIWRL
		jsr	EthWriteReg
		tya
		LDX	#MIWRH
		jsr	EthWriteReg
		ldx	#MISTAT		; page 3
		jsr	EthWritePage	
@1:		jsr	EthReadReg		; MI status
		and	#MISTAT_BUSY	
		bne	@1			; wait for not busy
		ldx	#ERDPTL		; page 0
		jsr	EthWritePage
		rts

;*********************************************************************
; delay ms delay milliseconds in A reg
;
delay_ms:		phx				; 
			phy				;  1MHz clk = $01C0 
@1:			ldy	#$06			;  2MHz clk = $0288 
			ldx	#$90			;  4MHz clk = $0416 
@2:			dex				;  8MHz clk = $0734 
			bne	@2			; 10MHz clk = $08C3
			dey				; 14MHz clk = $0BE0
			bne	@2                ; 14.318MHz = $0C24
			dec                     ; 7.159 Mhz = $0690
			bne	@1
			ply
			plx
			rts


;############################## External Functions ############################

;*********************************************************************
; _init_ether
; 
_init_ether:
		ldx	#ERDPTL		; page 0
		jsr	EthWritePage

		; perform system reset
		lda	#255-SPIeth		;
		AND	SPISSR		; select eth SS
		STA	SPISSR
		lda	#ENC_SOFT_RESET
		STA	SPIDR			; soft reset command
@1:		LDA	SPISR			; 
		bpl	@1			; wait for tx to end
		LDA	SPIDR			; clr SPISR bit 7
		lda	#SPIeth		; 
		ORA	SPISSR		; deselect eth
		STA	SPISSR

		lda	#$01
		jsr	delay_ms		; wait 1 ms

		ldx	#ESTAT
@3:		jsr	EthReadReg		; wait for end of reset
		and	#ESTAT_CLKRDY
		beq	@3

		; do bank 0 stuff
		; initialize receive buffer
		; 16-bit transfers, must write low byte first
		lda	#<RXSTART_INIT
		ldx	#>RXSTART_INIT
		sta	NextPacketPtr
		stx	NextPacketPtr+1

		; set receive buffer start address
		lda	#<RXSTART_INIT
		ldx	#ERXSTL
		jsr	EthWriteReg
		lda	#>RXSTART_INIT
		ldx	#ERXSTH
		jsr	EthWriteReg
		
		; set receive read pointer address
		lda	#<RXSTART_INIT
		ldx	#ERXRDPTL
		jsr	EthWriteReg
		lda	#>RXSTART_INIT
		ldx	#ERXRDPTH
		jsr	EthWriteReg

		; set receive write pointer address
		lda	#<RXSTART_INIT
		ldx	#ERXWRPTL
		jsr	EthWriteReg
		lda	#>RXSTART_INIT
		ldx	#ERXWRPTH
		jsr	EthWriteReg

		; set receive buffer end
		; ERXND defaults to 0x1FFF (end of ram)
		lda	#<RXSTOP_INIT
		ldx	#ERXNDL
		jsr	EthWriteReg
		lda	#>RXSTOP_INIT
		ldx	#ERXNDH
		jsr	EthWriteReg

		; set transmit buffer start
		; ETXST defaults to 0x0000 (beginnging of ram)
		lda	#<TXSTART_INIT
		ldx	#ETXSTL
		jsr	EthWriteReg
		lda	#>TXSTART_INIT
		ldx	#ETXSTH
		jsr	EthWriteReg

		; do bank 1 stuff
		LDX	#ERXFCON
		JSR	EthWritePage
		lda	#$A1			; default setting
		jsr	EthWriteReg

		; do bank 2 stuff
		; enable MAC receive
		LDX	#MACON1
		JSR	EthWritePage
		lda	#MACON1_MARXEN + MACON1_TXPAUS + MACON1_RXPAUS
		jsr	EthWriteReg

		; bring MAC out of reset
		ldx	#MACON2	;*** this reg not in PDF????
		lda	#$00
		jsr	EthWriteReg

		; enable automatic padding and CRC operations
		ldx	#ENC_BIT_FIELD_SET + (MACON3 & $1F) 
		lda	#MACON3_PADCFG0 + MACON3_TXCRCEN + MACON3_FRMLNEN
		jsr	EthWriteRaw

		; set inter-frame gap (non-back-to-back)
		ldx	#MAIPGL
		lda	#$12
		jsr	EthWriteReg
		ldx	#MAIPGH
		lda	#$0C
		jsr	EthWriteReg

		; set inter-frame gap (back-to-back)
		ldx	#MABBIPG
		lda	#$12
		jsr	EthWriteReg

		; Set the maximum packet size which the controller will accept
		ldx	#MAMXFLL
		lda	#<MAX_FRAMELEN
		jsr	EthWriteReg
		ldx	#MAMXFLH
		lda	#>MAX_FRAMELEN
		jsr	EthWriteReg

		; do bank 3 stuff
		; write MAC address
		; NOTE: MAC address in ENC28J60 is byte-backward
		ldx	#MAADR5
		jsr	EthWritePage
		lda	MYMAC
		jsr	EthWriteReg
		ldx	#MAADR4
		lda	MYMAC+1
		jsr	EthWriteReg
		ldx	#MAADR3
		lda	MYMAC+2
		jsr	EthWriteReg
		ldx	#MAADR2
		lda	MYMAC+3
		jsr	EthWriteReg
		ldx	#MAADR1
		lda	MYMAC+4
		jsr	EthWriteReg
		ldx	#MAADR0
		lda	MYMAC+5
		jsr	EthWriteReg

		; no loopback of transmitted frames
		ldx	#PHCON2			; moves to page 2
		lda	#<PHCON2_HDLDIS
		ldy	#>PHCON2_HDLDIS
		jsr	PhyWrite			; returns on page 0

		; switch to bank 0 (done above)
		; enable interrutps
		ldx	#ENC_BIT_FIELD_SET + EIE
		lda	#EIE_INTIE + EIE_PKTIE
		jsr	EthWriteRaw

		; enable packet reception
		ldx	#ENC_BIT_FIELD_SET + ECON1
		lda	#ECON1_RXEN
		jsr	EthWriteRaw
		rts

;*********************************************************************
; _send_ether
; 
_send_ether:
		;  test minimum packet length
		lda	#$3C
		ldx	_uip_len+1
		bne	@05
		cmp	_uip_len
		bcc	@05			; len > 3C
		sta	_uip_len
@05:		
		; set _uip pointers
		lda	#<_uip_buf
		ldx	#>_uip_buf
		sta	ptr1
		stx	ptr1+1

		lda	_uip_len
		ldx	_uip_len+1
		sta	ptr2
		stx	ptr2+1

		; Set the write pointer to start of transmit buffer area
		ldx	#EWRPTL
		lda	#<TXSTART_INIT
		jsr	EthWriteReg
		ldx	#EWRPTH
		lda	#>TXSTART_INIT
		jsr	EthWriteReg

		; Set the TXND pointer to correspond to the packet size given
		clc
		lda	ptr2
		adc	#<TXSTART_INIT
		tay
		lda	ptr2+1
		adc	#>TXSTART_INIT		
		pha
		ldx	#ETXNDL
		tya
		jsr	EthWriteReg
		ldx	#ETXNDH
		pla
		jsr	EthWriteReg

		; write per-packet control byte
		ldx	#ENC_WRITE_BUF_MEM
		lda	#$00
		jsr	EthWriteRaw

		; we send header and user data seperately
		sec
		lda	ptr2
		sbc	#54			; ETH hdr len + IP hdr len
		pha				 
		lda	ptr2+1
		sbc	#0
		pha

		; copy the packet headers into the transmit buffer
		lda	#54
		sta	ptr2
		stz	ptr2+1
		jsr	WriteBuffer

		; set data pointer to the uip_appdata pointer
		lda	_uip_appdata
		sta	ptr1
		lda	_uip_appdata+1
		sta	ptr1+1

		; set user data len
		plx
		pla
		sta	ptr2
		stx	ptr2+1

		; copy the packet into the transmit buffer
		jsr	WriteBuffer
	
		; send the contents of the transmit buffer onto the network
		ldx	#ENC_BIT_FIELD_SET + ECON1
		lda	#ECON1_TXRTS
		jsr	EthWriteRaw
		rts

;*********************************************************************
; _rcv_ether
; buffer pointer in a,x, maxlen on stack
;
_rcv_ether:
		; check if a packet has been received and buffered
		ldx	#EPKTCNT
		jsr	EthWritePage		; page 1
		jsr	EthReadReg
		cmp	#$00
		bne	@1
		ldx	#ERDPTL
		jsr	EthWritePage		; page 0
		stz	_uip_len
		stz	_uip_len+1			; mark no data
		lda	#$0
		tax
		rts
@1:
		; set _uip pointers
		lda	#<_uip_buf
		ldx	#>_uip_buf
		sta	ptr1
		stx	ptr1+1

		; Set the read pointer to the start of the received packet
		ldx	#ERDPTL
		jsr	EthWritePage		; page 0
		lda	NextPacketPtr
		jsr	EthWriteReg
		ldx	#ERDPTH
		lda	NextPacketPtr+1
		jsr	EthWriteReg

		; read the next packet pointer
		ldx	#ENC_READ_BUF_MEM
		jsr	EthReadRaw
		sta	NextPacketPtr
		ldx	#ENC_READ_BUF_MEM
		jsr	EthReadRaw
		sta	NextPacketPtr+1

		; read the packet length
		ldx	#ENC_READ_BUF_MEM
		jsr	EthReadRaw
		sta	ptr2
		ldx	#ENC_READ_BUF_MEM
		jsr	EthReadRaw
		sta	ptr2+1

		; dec len by 4 to ignore crc
		sec
		lda	ptr2
		sbc	#$04
		sta	ptr2
		sta	_uip_len
		lda	ptr2+1
		sbc	#$00
		sta	ptr2+1
		sta	_uip_len+1			; save length

		; read and ignore the receive status
		ldx	#ENC_READ_BUF_MEM
		jsr	EthReadRaw
		ldx	#ENC_READ_BUF_MEM
		jsr	EthReadRaw

		; copy the packet from the receive buffer
		jsr	ReadBuffer

		; Move the RX read pointer to the start of the next received packet
		; This frees the memory we just read out
		ldx	#ERXRDPTL
		lda	NextPacketPtr
		jsr	EthWriteReg
		ldx	#ERXRDPTH
		lda	NextPacketPtr+1
		jsr	EthWriteReg

		; decrement the packet counter indicate we are done with this packet
		ldx	#ENC_BIT_FIELD_SET + ECON2
		lda	#ECON2_PKTDEC
		jsr	EthWriteRaw

		ldx	_uip_len+1
		lda	_uip_len
		rts

;*********************************************************************
;EthOvfRec
;
EthOvfRec:

		; not implemented

		rts

;************************************************************************

.data
NextPacketPtr: 	.byte  0,0			; word
MYMAC:		.byte  0,6,5,8,1,6
